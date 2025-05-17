# Core Qt modules
QT += core gui charts sql printsupport texttospeech
QT += multimedia multimediawidgets network pdf concurrent serialport
QT += quick quickwidgets location positioning
QT += multimedia multimediawidgets
QT += multimedia
# Modern C++ support
CONFIG += c++17

# Suppress warnings
QMAKE_CXXFLAGS += -Wno-unused-variable

# Widgets for Qt5+
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Include QXlsx
include($$PWD/QXlsx/QXlsx.pri)

# Windows-specific configurations
win32 {
    # OpenCV configuration
    OPENCV_PATH = C:/opencv/OpenCV-MinGW-Build-OpenCV-4.5.5-x64
    INCLUDEPATH += $${OPENCV_PATH}/include

    LIBS += -L$${OPENCV_PATH}/x64/mingw/lib \
            -lopencv_core455 \
            -lopencv_imgproc455 \
            -lopencv_features2d455 \
            -lopencv_highgui455 \
            -lopencv_imgcodecs455 \
            -lopencv_dnn455 \
            -lopencv_videoio455 \
            -lopencv_video455 \
            -lopencv_objdetect455 \
            -lopencv_calib3d455 \
            -lopencv_dnn455

    # Performance optimizations
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    QMAKE_CXXFLAGS += -O3 -march=native

    # Copy OpenCV DLLs
    QMAKE_POST_LINK += cmd /c copy /y \"$${OPENCV_PATH}\\x64\\mingw\\bin\\*.dll\" \"$$OUT_PWD\"

    # Create calls directory for recordings
    CALLS_DIR = $$OUT_PWD/calls
    QMAKE_POST_LINK += $$quote(mkdir $$replace(CALLS_DIR, /, \\) 2> nul || exit 0)
} else {
    # Unix/Linux/Mac configurations
    CALLS_DIR = $$OUT_PWD/calls
    QMAKE_POST_LINK += mkdir -p $$CALLS_DIR

    # Audio backends for Linux
    unix:!macx {
        PKGCONFIG += alsa pulseaudio
    }

    # Mac specific audio
    macx {
        QT += avfoundation
        LIBS += -framework AVFoundation -framework CoreAudio
    }
}

# AI/ML specific additions
CONFIG += link_pkgconfig
DEFINES += ENABLE_AI_ANALYSIS

# For HTTPS API calls to AI services
openssl {
    LIBS += -lssl -lcrypto
}

win32 {
    # ... existing code ...

    # Add multimedia DLLs to be copied
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /Y /Q \"$$[QT_INSTALL_BINS]\\Qt6Multimedia*.dll\" \"$$OUT_PWD\")
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /Y /Q \"$$[QT_INSTALL_BINS]\\Qt6Network*.dll\" \"$$OUT_PWD\")
}

win32 {
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /Y /Q \"$$[QT_INSTALL_BINS]\\Qt6Multimedia.dll\" \"$$OUT_PWD\")
}

# Source files
SOURCES += \
    Arduino.cpp \
    CONSULTATIONS.cpp \
    Consultationwindow.cpp \
    SettingsDialog.cpp \
    arduin.cpp \
    client.cpp \
    connection.cpp \
    dashboard.cpp \
    employe.cpp \
    employeinterface.cpp \
    gestion_projet.cpp \
    login.cpp \
    main.cpp \
    mainmap.cpp \
    mainwindow.cpp \
    modification_consultation.cpp \
    modifierressourcedialog.cpp \
    modifyclient.cpp \
    projet.cpp \
    ressources.cpp \
    window.cpp

HEADERS += \
    Arduino.h \
    dashboard.h \
    login.h \
    CONSULTATIONS.h \
    Consultationwindow.h \
    SettingsDialog.h \
    arduin.h \
    client.h \
    connection.h \
    employe.h \
    employeinterface.h \
    gestion_projet.h \
    mainwindow.h \
    modification_consultation.h \
    modifierressourcedialog.h \
    modifyclient.h \
    projet.h \
    ressources.h \
    window.h

FORMS += \
    Consultationwindow.ui \
    EmployeInterface.ui \
    SettingsDialog.ui \
    client.ui \
    dashboard.ui \
    employeinterface.ui \
    gestion_projet.ui \
    login.ui \
    mainwindow.ui \
    ressources.ui

RESOURCES += \
    icons.qrc \
    images.qrc

DISTFILES += \
    images/object.png

# Installation paths
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Manual DLL deployment for Windows (alternative to deployment.pri)
win32 {
    # Copy Qt multimedia DLLs
    QMAKE_POST_LINK += $$quote(cmd /c if not exist $$OUT_PWD\\audio (mkdir $$OUT_PWD\\audio))
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /Y /Q \"$$[QT_INSTALL_BINS]\\Qt5Multimedia*.dll\" \"$$OUT_PWD\")
    QMAKE_POST_LINK += $$quote(cmd /c xcopy /Y /Q \"$$[QT_INSTALL_BINS]\\Qt5Network*.dll\" \"$$OUT_PWD\")
}
