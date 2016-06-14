TEMPLATE = subdirs
CONFIG += ordered debug
SUBDIRS = editor_core $$UI
$${UI}.depends = editor_core
