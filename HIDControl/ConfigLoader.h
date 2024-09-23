#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <QMap>
#include <QString>
#include <QVector>
#include <string>
#include <vector>

struct Dongle {
    QString _vid;
    QString _pid;
    QString _usage_page;
    QString _usage;
    int _retry_cnt;
    int _packet_time;
};

// struct Plugins {
//     bool _enable;
//     QString _vid;
//     QString _pid;
//     QString _plugin_vid;
//     QString _plugin_pid;
//     QString _firmware;
// };

// struct PluginConfigGeeHy : public Plugins {};

struct ConfigData {
    int _retry_cnt;
    int _packet_time;

    bool _develop_mode;
    bool _factory_mode;

    QString _app_title;
    QString _config_version;
    QString _device_type;
    QString _name;
    QString _vid;
    QString _pid;
    QString _usage_page;
    QString _usage;
    QString _upgrade_method;
    QString _version;
    QString _pass;
    QString _language;

    QVector<Dongle> _dongles;
};

class ConfigLoader {
public:
    ConfigLoader() = default;

    bool load();
    bool load(const QString &path);

    bool save();
    bool save(const QString &path);

public:
    static ConfigLoader *instance();

public:
    ConfigData *_data = nullptr;

private:
    static ConfigLoader *_instance;
    QString _config_name = "config.json";
};

#endif // CONFIG_LOADER_H
