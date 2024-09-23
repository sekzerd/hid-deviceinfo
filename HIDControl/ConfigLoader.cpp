#include "ConfigLoader.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QtDebug>

ConfigLoader *ConfigLoader::_instance = nullptr;

ConfigLoader *ConfigLoader::instance() {
    if (!_instance) {
        _instance = new ConfigLoader();
    }
    return _instance;
}

bool ConfigLoader::load() {
    auto appDir = QCoreApplication::applicationDirPath();
    qDebug() << "cur" << appDir;
    return load(appDir + "/" + _config_name);
}

bool ConfigLoader::load(const QString &path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (jsonDoc.isNull()) {
        return false;
    }

    if (!jsonDoc.isObject()) {
        return false;
    }
    QVector<Dongle> dongles;
    auto dongle = jsonDoc["dongles"].toArray();
    for (auto i : dongle) {
        Dongle d;
        d._vid = i.toObject().value("vid").toString();
        d._pid = i.toObject().value("pid").toString();
        d._usage_page = i.toObject().value("usage_page").toString();
        d._usage = i.toObject().value("usage").toString();
        d._retry_cnt = i.toObject().value("retry_cnt").toInt();
        d._packet_time = i.toObject().value("packet_time").toInt();
        dongles.push_back(d);
    }
    if (_data) {
        delete _data;
        _data = nullptr;
    }
    _data = new ConfigData();
    _data->_dongles = dongles;
    _data->_vid = jsonDoc["vid"].toString();
    _data->_pid = jsonDoc["pid"].toString();
    _data->_config_version = jsonDoc["config_version"].toString();
    _data->_device_type = jsonDoc["device_type"].toString();
    _data->_name = jsonDoc["name"].toString();
    _data->_usage_page = jsonDoc["usage_page"].toString();
    _data->_usage = jsonDoc["usage"].toString();
    _data->_retry_cnt = jsonDoc["retry_cnt"].toInt();
    _data->_packet_time = jsonDoc["packet_time"].toInt();
    _data->_version = jsonDoc["version"].toString();
    _data->_pass = jsonDoc["pass"].toString();
    _data->_factory_mode = jsonDoc["factory_mode"].toBool();
    _data->_language = jsonDoc["language"].toString();
    _data->_app_title = jsonDoc["app_title"].toString();
    _data->_develop_mode = jsonDoc["develop_mode"].toBool();
    _data->_upgrade_method = jsonDoc["upgrade_method"].toString();

    return true;
}

bool ConfigLoader::save() {
    auto appDir = QCoreApplication::applicationDirPath();
    return save(appDir + "/" + _config_name);
}
bool ConfigLoader::save(const QString &path) {
    if (!_data) {
        return false;
    }
    QJsonArray dongles;

    for (auto i = 0; i < _data->_dongles.size(); ++i) {
        QJsonObject obj;
        obj["vid"] = _data->_dongles[i]._vid;
        obj["pid"] = _data->_dongles[i]._pid;
        obj["usage_page"] = _data->_dongles[i]._usage_page;
        obj["usage"] = _data->_dongles[i]._usage;
        obj["retry_cnt"] = _data->_dongles[i]._retry_cnt;
        obj["packet_time"] = _data->_dongles[i]._packet_time;
        dongles.push_back(obj);
    }

    QJsonObject data;
    data["config_version"] = _data->_config_version;
    data["device_type"] = _data->_device_type;
    data["name"] = _data->_name;
    data["vid"] = _data->_vid;
    data["pid"] = _data->_pid;
    data["usage_page"] = _data->_usage_page;
    data["usage"] = _data->_usage;
    data["retry_cnt"] = _data->_retry_cnt;
    data["packet_time"] = _data->_packet_time;
    data["version"] = _data->_version;

    data["pass"] = _data->_pass;
    data["factory_mode"] = _data->_factory_mode;
    data["language"] = _data->_language;
    data["app_title"] = _data->_app_title;
    data["develop_mode"] = _data->_develop_mode;
    data["upgrade_method"] = _data->_upgrade_method;

    data["dongles"] = dongles;
    QJsonDocument jsonDoc(data);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(jsonDoc.toJson());
    file.close();
    return true;
}
