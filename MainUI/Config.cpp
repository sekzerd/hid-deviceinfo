#include "Config.h"
#include <QDir>
#include <QSettings>

Config::Config() {
    QString userDir = QDir::homePath();
    QString path = userDir + "/sino_timesync.ini";
    QSettings s(path, QSettings::IniFormat);
    vid = s.value("vid", "0x258A").toString();
    pid = s.value("pid", "0x0138").toString();
}

void Config::save() {
    QString userDir = QDir::homePath();
    QString path = userDir + "/sino_timesync.ini";
    QSettings s(path, QSettings::IniFormat);
    s.setValue("vid", vid);
    s.setValue("pid", pid);
    s.sync();
}
