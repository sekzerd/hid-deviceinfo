#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HIDControl/CmdControl.h"
#include "HIDControl/HidControl.h"

#include <QAbstractButton>
#include <QAtomicInt>
#include <QButtonGroup>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QListWidgetItem>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event) override;
    void reload_device();
    void load_path(int vid, int pid);
    void load_info(QString path, QVariant payload);
    void init_connect();

private:
    Ui::MainWindow *ui;

private:
    QJsonArray fetch_devices();
    //    QMap<QString, hid_device *> fetch_devices();
    void init_ui();
    void clear_ui();

private:
    hid_device *_dev = nullptr;
    CmdControl *_cmd_control = nullptr;
    QString _current_path = QDir::currentPath() + "/";
    QString _config_path = _current_path + "config.json";
};

#endif // MAINWINDOW_H
