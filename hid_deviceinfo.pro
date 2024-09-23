TEMPLATE = subdirs

CONFIG += warn_off no_batch


SUBDIRS += HIDControl
SUBDIRS += MainUI


MainUI.depends += HIDControl



