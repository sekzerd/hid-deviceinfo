#include "CmdControl.h"
#include <QtDebug>
#include <algorithm>
#include <fstream>

CmdControl::CmdControl() {}

void CmdControl::attach_device(hid_device *dev) { _dev = dev; }

bool CmdControl::setup() { return true; }
