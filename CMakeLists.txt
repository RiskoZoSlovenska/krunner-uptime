cmake_minimum_required(VERSION 3.16)
project(krunner-uptime
    VERSION 1.0.0
    DESCRIPTION "A KDE Plasma 6 KRunner plugin that displays the system uptime."
    HOMEPAGE_URL https://github.com/RiskoZoSlovenska)
set(CMAKE_PROJECT_AUTHOR "RiskoZoSlovenska")

set(KF6_MIN_VERSION 6.0.0)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)  # Include the Extra-CMake-Modules project
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS I18n Config Runner)

set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CI_CPACK_OS}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

if(${CI_CPACK_OS} STREQUAL "ubuntu")
    # Ubuntu
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CMAKE_PROJECT_AUTHOR})
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libkf6runner6 (>=6.0.0)")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/postinst;${CMAKE_SOURCE_DIR}/postrm")
else()
    # Fedora
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_REQUIRES "kf6-krunner >= 6.0.0")
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/postinst")
    set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/postrm")
endif()

# Export variables for use in the CI
file(WRITE "${CMAKE_BINARY_DIR}/cmake-vars.sh"
    "export CPACK_PACKAGE_FILE_NAME='${CPACK_PACKAGE_FILE_NAME}' \
    ")

include(CPack)

kcoreaddons_add_plugin(krunner_uptime SOURCES krunner_uptime.cpp INSTALL_NAMESPACE "kf6/krunner")
target_link_libraries(krunner_uptime
    KF6::I18n
    KF6::ConfigCore
    KF6::Runner
)
