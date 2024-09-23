#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "HIDControl/CmdControlSyncTime.h"
#include "HIDControl/ConfigLoader.h"

#include <QCloseEvent>
#include <QDateTime>
#include <QMessageBox>

#include "Config.h"
#include "HIDControl/Utils.h"
#include <QComboBox>
#include <QJsonDocument>
#include <QString>
#include <QVariant>
#include <QtConcurrent>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QThreadPool::globalInstance()->setMaxThreadCount(6);

    init_ui();
    init_connect();
    reload_device();
    connect(ui->le_vid, &QLineEdit::textChanged, this, [=](QString msg) {
        Config c;
        c.vid = msg;
        c.save();
    });

    connect(ui->le_pid, &QLineEdit::textChanged, this, [=](QString msg) {
        Config c;
        c.pid = msg;
        c.save();
    });

    connect(ui->btn_filter, &QPushButton::clicked, this, [=]() {

    });
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event) {}

void MainWindow::reload_device() {
    auto fill = [](QString input, int len = 4) -> QString {
        QString ret = input;
        while (ret.length() < 4) {
            ret = "0" + ret;
        };
        return ret;
    };
    auto devices = fetch_devices();
    ui->cb_devices->clear();
    for (auto d : devices) {
        qDebug() << d;
        QJsonObject p = d.toObject();
        QString info;
        info += fill(QString::number(p["vendor_id"].toInt(), 16));
        info += " ";
        info += fill(QString::number(p["product_id"].toInt(), 16));
        info += " ";
        info += p["product_string"].toString();
        info += " ";
        info += p["manufacturer_string"].toString();
        QJsonObject payload;
        QJsonDocument doc(p);
        bool founded = false;
        auto size = ui->cb_devices->size();
        for (auto i = 0; i < size.width(); i++) {
            auto item_text = ui->cb_devices->itemText(i);
            if (item_text == info) {
                founded = true;
            }
        }
        if (!founded) {
            ui->cb_devices->addItem(info,
                                    QVariant(QString::fromUtf8(doc.toJson())));
        }
    }
}

void MainWindow::load_path(int vid, int pid) {
    auto devices = fetch_devices();
    ui->cb_paths->clear();
    for (auto d : devices) {
        QJsonObject p = d.toObject();
        if (p["vendor_id"].toInt() == vid && p["product_id"].toInt() == pid) {
            QString path = p["path"].toString();
            QJsonDocument doc(p);
            ui->cb_paths->addItem(path,
                                  QVariant(QString::fromUtf8(doc.toJson())));
        }
    }
}

void MainWindow::load_info(QString path, QVariant payload) {
    auto fill = [](QString input, int len = 4) -> QString {
        QString ret = input;
        while (ret.length() < 4) {
            ret = "0" + ret;
        };
        return ret;
    };
    auto doc = QJsonDocument::fromJson(payload.toString().toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return;
    }
    auto obj = doc.object();
    ui->lb_path->setText(obj["path"].toString());
    ui->lb_vid->setText(fill(QString::number(obj["vendor_id"].toInt(), 16)));
    ui->lb_pid->setText(fill(QString::number(obj["product_id"].toInt(), 16)));
    ui->lb_product->setText(obj["product_string"].toString());
    ui->lb_manufacturer->setText(obj["manufacturer_string"].toString());
    ui->lb_interface_num->setText(
        QString::number(obj["interface_number"].toInt()));
    ui->lb_release_num->setText(QString::number(obj["release_number"].toInt()));

    ui->lb_usage->setText(QString::number(obj["usage"].toInt()));
    ui->lb_usage_page->setText(QString::number(obj["usage_page"].toInt()));
    ui->lb_serial_num->setText(obj["serial_number"].toString());

    hid_device *dev =
        hid_open_path(obj["path"].toString().toStdString().data());
    ui->lb_feature_len->setText(QString::number(dev->feature_report_length));
    ui->lb_output_len->setText(QString::number(dev->output_report_length));
    ui->lb_input_len->setText(QString::number(dev->input_report_length));
}

void MainWindow::init_connect() {
    connect(ui->cb_devices, &QComboBox::currentTextChanged, this, [=]() {
        QVariant v = ui->cb_devices->currentData();
        QJsonDocument doc = QJsonDocument::fromJson(v.toString().toUtf8());
        if (doc.isNull() || !doc.isObject()) {
            return;
        }
        auto obj = doc.object();
        auto vid = obj["vendor_id"].toInt();
        auto pid = obj["product_id"].toInt();

        load_path(vid, pid);
    });
    connect(ui->btn_reload_devices, &QPushButton::clicked, this,
            [=]() { reload_device(); });
    connect(ui->cb_paths, &QComboBox::currentTextChanged, this, [=]() {
        auto d = ui->cb_paths->currentData();
        load_info(ui->cb_paths->currentText(), d);
    });
    connect(ui->btn_filter, &QPushButton::clicked, this, [=]() {
        auto svid = ui->le_vid->text();
        auto spid = ui->le_pid->text();
        if (svid.startsWith("0x")) {
            svid = svid.replace("0x", "");
        }
        if (spid.startsWith("0x")) {
            spid = spid.replace("0x", "");
        }
        auto vid = svid.toInt(nullptr, 16);
        auto pid = spid.toInt(nullptr, 16);

        auto delete_func = [=]() -> bool {
            auto size = ui->cb_devices->size();
            for (auto i = 0; i < size.width(); i++) {
                auto d = ui->cb_devices->itemData(i);
                QJsonDocument doc =
                    QJsonDocument::fromJson(d.toString().toUtf8());
                if (doc.isNull() || !doc.isObject()) {
                    continue;
                }
                auto obj = doc.object();
                auto filter_vid = obj["vendor_id"].toInt();
                auto filter_pid = obj["product_id"].toInt();
                if (filter_vid != vid || filter_pid != pid) {
                    ui->cb_devices->removeItem(i);
                    return true;
                }
            }
            return false;
        };
        for (;;) {
            if (!delete_func()) {
                break;
            }
        }
        //        ui->cb_devices->setCurrentIndex(0);
    });
}

QJsonArray MainWindow::fetch_devices() {
    QJsonArray ret;
    hid_init();
    hid_device_info *pDevs = NULL, *pCur_Devs = NULL;
    pDevs = hid_enumerate(0, 0);
    pCur_Devs = pDevs;

    for (; pCur_Devs != nullptr; pCur_Devs = pCur_Devs->next) {
        hid_device *hid_device = hid_open_path(pCur_Devs->path);
        if (!hid_device) {
            continue;
        }
        QJsonObject obj;
        obj["path"] = pCur_Devs->path;
        obj["vendor_id"] = (int)pCur_Devs->vendor_id;
        obj["product_id"] = (int)pCur_Devs->product_id;
        obj["serial_number"] =
            QString::fromWCharArray(pCur_Devs->serial_number);
        obj["release_number"] = (int)pCur_Devs->release_number;
        obj["manufacturer_string"] =
            QString::fromWCharArray(pCur_Devs->manufacturer_string);
        obj["product_string"] =
            QString::fromWCharArray(pCur_Devs->product_string);
        obj["usage_page"] = (int)pCur_Devs->usage_page;
        obj["usage"] = (int)pCur_Devs->usage;
        obj["interface_number"] = (int)pCur_Devs->interface_number;
        ret.push_back(obj);
    }
    return ret;
}

void MainWindow::init_ui() {
    Config c;
    ui->le_vid->setText(c.vid);
    ui->le_pid->setText(c.pid);
}
