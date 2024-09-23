#ifndef HIDINTERFACE_H
#define HIDINTERFACE_H

#include "hidapi.h"
#include <QObject>

class HidControl : public QObject {
    Q_OBJECT
public:
    HidControl(hid_device *dev) : _dev(dev) { feature_report_lenght = 520; }
    virtual ~HidControl() = default;

public:
    bool write(char *data, int len);
    int read(char *data, int len);
    bool set_feature(char *data, int len);
    int get_feature(char *data, int len);

public:
    static int feature_report_lenght;

private:
    hid_device *_dev = nullptr;
};

#endif // HIDINTERFACE_H
