#include "HidControl.h"

int HidControl::feature_report_lenght = 0;

bool HidControl::write(char *data, int len) {
    if (!_dev) {
        return false;
    }

    char *d = new char[len];
    memset(d, 0, len);
    memcpy(d, data, len);

    auto ret =
        hid_write(_dev, (const unsigned char *)d, len) == -1 ? false : true;

    if (d) {
        delete[] d;
    }
    return ret;
}

int HidControl::read(char *data, int len) {
    if (!_dev) {
        return -1;
    }
    auto ret = hid_read_timeout(_dev, (unsigned char *)data, len, 100);
    return ret > 0 ? ret : -1;
}

bool HidControl::set_feature(char *data, int len) {
    if (!_dev) {
        return false;
    }

    if (len > feature_report_lenght) {
        return false;
    }
    char *d = new char[feature_report_lenght];
    memset(d, 0, feature_report_lenght);
    memcpy(d, data, len);
    auto ret = hid_send_feature_report(_dev, (const unsigned char *)d,
                                       feature_report_lenght) == -1
                   ? false
                   : true;
    if (d) {
        delete[] d;
    }
    return ret;
}

int HidControl::get_feature(char *data, int len) {
    if (!_dev) {
        return -1;
    }

    auto ret = hid_get_feature_report(_dev, (unsigned char *)data, len);
    return ret > 0 ? ret : -1;
}
