QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets
}

lessThan(QT_MAJOR_VERSION, 5){
    SOURCES += qt5cryptographichash.cpp
    HEADERS += qt5cryptographichash.h
}

TARGET = passwd-gen
TEMPLATE = app
SOURCES += main.cpp mainwindow.cpp
HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

#INCLUDEPATH += .
CONFIG += debug_and_release build_all

# deploy on generic linux (N900)
target.path = /usr/bin
icon64.path = /opt/usr/share/icons/hicolor/64x64/hildon
icon64.files = icons/64x64/$${TARGET}.png
icon32.path = /opt/usr/share/icons/hicolor/32x32/hildon
icon32.files = icons/32x32/$${TARGET}.png
desktopfile.path = /usr/share/applications/hildon
desktopfile.files = $${TARGET}.desktop
INSTALLS += target icon32x icon64x desktopfile

# deploy on android
android: ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
OTHER_FILES += android/AndroidManifest.xml
