QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFileMVD
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    qtmyfile_mvd.cpp \
    renamedialog.cpp \
    exportdialog.cpp \
    filesystemmodel_checkbox.cpp \
    exportdetaildialog.cpp \
    main.cpp

HEADERS  += \
    qtmyfile_mvd.h \
    renamedialog.h \
    exportdialog.h \
    filesystemmodel_checkbox.h \
    exportdetaildialog.h

FORMS    += \
    qtmyfile_mvd.ui \
    renamedialog.ui \
    exportdialog.ui \
    exportdetaildialog.ui
