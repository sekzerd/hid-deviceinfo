#ifndef CMD_CONTROL_GEEHY_H
#define CMD_CONTROL_GEEHY_H

#include "CmdControl.h"
#include "HidControl.h"
#include <QMutex>
#include <QObject>
#include <QString>
#include <QThread>
#include <QVariant>
#include <string>
#include <vector>

class CmdControlSyncTime : public CmdControl {

public:
    CmdControlSyncTime();
    virtual ~CmdControlSyncTime();

public slots:
    bool set_params(const QMap<QString, QVariant> &params) override;
    bool reset() override;
    void run() override;
    bool setup() override;
    bool on_cmd_out(Cmd cmd) override;
    void attach_device(hid_device *dev) override;

private:
    bool _stop = false;
    bool _start = false;

    HidControl *_hid_control = nullptr;
};

#endif // CMD_CONTROL_GEEHY_H
