#include <chrono>
#include <sys/sysinfo.h>

#include <KConfigGroup>
#include <klocalizedstring.h>
#include <KRunner/RunnerSyntax>

#include "krunner_uptime.h"

// `std::chrono::days` is only in C++20
#define HOURS_PER_DAY 24

UptimeRunner::UptimeRunner(QObject *parent, const KPluginMetaData &data)
	: AbstractRunner(parent, data)
{}

void UptimeRunner::init()
{
    this->reloadConfiguration();
}

void push_time_component(QString &so_far, int &nth, const char *singular, const char *plural, int amount)
{
    if (amount == 0) {
        return;
    }

    if (nth == 2) {
        so_far = so_far.prepend(i18nc("Separator between last two items in a list", " and "));
    } else if (nth > 2) {
        so_far = so_far.prepend(i18nc("Separator between non-last-two items in a list", ", "));
    }
    nth += 1;

    so_far = so_far.prepend(i18np(singular, plural, amount));
}

void UptimeRunner::match(KRunner::RunnerContext &context)
{
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        return;
    }

    auto total = std::chrono::seconds(info.uptime);
    auto hours = std::chrono::duration_cast<std::chrono::hours>(total);
    total -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(total);
    total -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(total);

    QString text;
    int nth = 1;
    push_time_component(text, nth, "%1 second", "%1 seconds", seconds.count());
    push_time_component(text, nth, "%1 minute", "%1 minutes", minutes.count());
    push_time_component(text, nth, "%1 hour",   "%1 hours",   hours.count() % HOURS_PER_DAY);
    push_time_component(text, nth, "%1 day",    "%1 days",    hours.count() / HOURS_PER_DAY);

    KRunner::QueryMatch match(this);
    match.setText(text);
    match.setIconName(QStringLiteral("clock-symbolic"));
    match.setCategoryRelevance(context.query() == this->trigger
            ? KRunner::QueryMatch::CategoryRelevance::Highest
            : KRunner::QueryMatch::CategoryRelevance::Moderate);
    context.addMatch(match);
}

void UptimeRunner::reloadConfiguration()
{
    KConfigGroup c = this->config();
    this->trigger = c.readEntry("trigger", i18nc("The default trigger word", "uptime"));
    this->setTriggerWords({ this->trigger });

    QList<KRunner::RunnerSyntax> syntaxes;
    KRunner::RunnerSyntax syntax(this->trigger, i18n("Displays the current system uptime"));
    syntaxes.append(syntax);
    this->setSyntaxes(syntaxes);
}

K_PLUGIN_CLASS_WITH_JSON(UptimeRunner, "krunner_uptime.json")

#include "krunner_uptime.moc"
