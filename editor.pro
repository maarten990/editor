TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editor_core $$UI
$${UI}.depends = editor_core