HEADERS += \
    $$PWD/GdeltCalloutData.h \
    $$PWD/GdeltEventLayer.h \
    $$PWD/AppInfo.h \
    $$PWD/GEOINTMonitor.h \
    $$PWD/NominatimPlaceLayer.h \
    $$PWD/WikimapiaPlaceLayer.h

SOURCES += \
    $$PWD/GdeltCalloutData.cpp \
    $$PWD/GdeltEventLayer.cpp \
    $$PWD/NominatimPlaceLayer.cpp \
    $$PWD/WikimapiaPlaceLayer.cpp \
    $$PWD/main.cpp \
    $$PWD/GEOINTMonitor.cpp

RESOURCES += \
    $$PWD/qml/qml.qrc \
    $$PWD/Resources/Resources.qrc

#-------------------------------------------------------------------------------

win32 {
    include (Win/Win.pri)
}

macx {
    include (Mac/Mac.pri)
}

ios {
    include (iOS/iOS.pri)
}

android {
    include (Android/Android.pri)
}
