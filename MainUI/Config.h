#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>

class Config {
public:
    Config();

public:
    void save();

public:
    QString vid;
    QString pid;
};

#endif // CONFIG_H
