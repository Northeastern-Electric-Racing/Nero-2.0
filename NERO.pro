QT += quick

SOURCES += \
        controllers/chargingcontroller.cpp \
        controllers/efficiencycontroller.cpp \
        controllers/homecontroller.cpp \
        main.cpp \
        models/mock_model.cpp \
        models/model.cpp \
        models/raspberry_model.cpp \
        modes/debug_mode/debug_utils.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources

TRANSLATIONS += \
    NERO_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    controllers/chargingcontroller.h \
    controllers/efficiencycontroller.h \
    constants/fault_statuses.h \
    controllers/homecontroller.h \
    models/mock_model.h \
    models/model.h \
    models/raspberry_model.h \
    modes/debug_mode/debug_utils.h
