#include "CmdControlSyncTime.h"
#include "CmdControl.h"
#include "CmdControlSyncTime.h"
#include "HidControl.h"
#include <QString>
#include <QtConcurrent/QtConcurrent>
#include <QtDebug>
#include <algorithm>
#include <fstream>

CmdControlSyncTime::CmdControlSyncTime() {
    _hid_control = new HidControl(_dev);
    _hid_control->feature_report_lenght = 0;
}

CmdControlSyncTime::~CmdControlSyncTime() {
    if (_hid_control) {
        delete _hid_control;
    }
}

bool CmdControlSyncTime::set_params(const QMap<QString, QVariant> &params) {
    try {

        return true;
    } catch (...) {
        return false;
    }
}

bool CmdControlSyncTime::reset() { return setup(); }

void CmdControlSyncTime::run() {
    //    _cmd_thread = new CmdThread(_dev);
    //    _cmd_thread->run();
    //    auto conn = connect(_cmd_thread, &CmdThread::on_cmd_in, this, [=](Cmd
    //    cmd) {
    //        if (cmd.type == Cmd::CmdType::GET) {
    //            auto msg = QString::fromStdString(cmd.receive_data);
    //            emit on_cmd_in(cmd);
    //        }
    //    });
}

bool CmdControlSyncTime::setup() {
    if (!_dev) {
        return false;
    }

    return true;
}

bool CmdControlSyncTime::on_cmd_out(Cmd cmd) {
    if (!_hid_control) {
        return false;
    }
    char *buff = new char[cmd.send_data.size()];
    memset(buff, 0, cmd.send_data.size());
    const int recv_len = 64;
    QThread::msleep(static_cast<unsigned long>(cmd.before_sleep));
    switch (cmd.type) {
    case Cmd::CmdType::FEATURE_IN: {
        //        qDebug() << "featrue in one";
        memcpy(buff, cmd.send_data.data(), cmd.send_data.size());
        auto receive_len =
            _hid_control->get_feature(buff, cmd.send_data.size());
        receive_len = receive_len >= 0 ? receive_len : 0;
        cmd.receive_data = std::string(buff, receive_len);
        emit on_cmd_in(cmd);
        break;
    }
    case Cmd::CmdType::FEATURE_OUT: {
        //        qDebug() << "featrue out one";
        memcpy(buff, cmd.send_data.data(), cmd.send_data.size());
        _hid_control->set_feature(buff, cmd.send_data.size());
        break;
    }
    case Cmd::CmdType::SET: {
        //        qDebug() << "set one";
        _hid_control->write((char *)cmd.send_data.data(), cmd.send_data.size());
        break;
    }
    case Cmd::CmdType::GET: {
        //        qDebug() << "get one";
        memcpy(buff, cmd.send_data.data(), cmd.send_data.size());
        auto receive_len =
            _hid_control->read(buff, HidControl::feature_report_lenght);
        receive_len = receive_len >= 0 ? receive_len : 0;
        cmd.receive_data = std::string(buff, receive_len);
        emit on_cmd_in(cmd);
        break;
    }
    default: {
        break;
    }
    }
    return true;
}

void CmdControlSyncTime::attach_device(hid_device *dev) {
    _dev = dev;
    if (_hid_control) {
        delete _hid_control;
    }
    _hid_control = new HidControl(_dev);
}
