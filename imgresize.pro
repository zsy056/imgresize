FORMS = main_window.ui tab_page.ui resize_dialog.ui
HEADERS = main_window.h tab_page.h resize_dialog.h
SOURCES = main.cpp main_window.cpp tab_page.cpp resize_dialog.cpp

TRANSLATIONS = imgresize_cn.ts
RESOURCES = resources.qrc

CONFIG += qt debug
QMAKE_CXXFLAGS += -std=c++11
