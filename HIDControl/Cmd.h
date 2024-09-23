#ifndef CMD_H
#define CMD_H

#include <QHash>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QString>
#include <QVector>

struct Cmd {
    enum class CmdType {
        FEATURE_IN,
        FEATURE_OUT,

        SET,
        GET,
        NONE,
    };
    QString name;
    CmdType type = CmdType::NONE;
    int before_sleep = 0;
    int after_sleep = 0;
    size_t id = 0;
    std::string send_data;
    std::string receive_data;

    bool operator==(const Cmd &other) const {
        return name == other.name && type == other.type && id == other.id &&
               send_data == other.send_data &&
               receive_data == other.receive_data;
    }

    uint qHash() const {
        return ::qHash(name.toUtf8()) ^ ::qHash(before_sleep) ^
               ::qHash(after_sleep) ^ ::qHash(id) ^
               ::qHash(QString::fromStdString(send_data).toUtf8()) ^
               ::qHash(QString::fromStdString(receive_data).toUtf8());
    }
    Cmd() {
        qRegisterMetaType<QList<Cmd>>("QList<Cmd>");
        qRegisterMetaType<Cmd>("Cmd");
    }
};
#endif // CMD_H
