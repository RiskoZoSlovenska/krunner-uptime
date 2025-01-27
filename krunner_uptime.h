#pragma once

#include <KRunner/AbstractRunner>
#include <KRunner/RunnerContext>
#include <KRunner/QueryMatch>

class UptimeRunner : public KRunner::AbstractRunner
{
	Q_OBJECT

public:
	UptimeRunner(QObject *parent, const KPluginMetaData &data);

	void match(KRunner::RunnerContext &context) override;
	void reloadConfiguration() override;

protected:
	void init() override;

private:
	QString trigger;
};
