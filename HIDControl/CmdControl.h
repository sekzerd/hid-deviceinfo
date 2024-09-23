#ifndef CMD_CONTROL_H
#define CMD_CONTROL_H

#include "Cmd.h"
#include "hidapi.h"
#include <QObject>
#include <QQueue>
#include <QString>
#include <QThread>
#include <QVariant>
#include <QVector>

class CmdControl : public QThread {
    Q_OBJECT

public:
    CmdControl();
    virtual ~CmdControl() = default;

public slots:
    virtual void attach_device(hid_device *dev);
    virtual bool setup();

public slots:
    virtual bool set_params(const QMap<QString, QVariant> &params) = 0;
    virtual bool reset() = 0;
    virtual void run() = 0;
    virtual bool on_cmd_out(Cmd cmd) = 0;

signals:
    void on_cmd_in(Cmd cmd);

protected:
    hid_device *_dev = nullptr;
};

#endif // CMD_CONTROL_H
