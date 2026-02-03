cmake_minimum_required(VERSION 3.21.1)

project(NEROApp LANGUAGES CXX)

# Find Qt6 first before version checks
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Mqtt Protobuf)

qt_standard_project_setup(REQUIRES 6.8)

# Consolidate all sources in one place
set(PROJECT_SOURCES
    # Headers - Constants
    src/constants/fault_statuses.h

    # Headers - Controllers
    src/controllers/chargingcontroller.h
    src/controllers/homecontroller.h
    src/controllers/offviewcontroller.h
    src/controllers/efficiencycontroller.h
    src/controllers/headercontroller.h
    src/controllers/buttoncontroller.h
    src/controllers/navigationcontroller.h
    src/controllers/flappybirdcontroller.h
    src/controllers/speedcontroller.h
    src/controllers/snakecontroller.h
    src/controllers/tankcontroller.h

    # Headers - Models
    src/models/model.h
    src/models/raspberry_model.h
    src/models/mqtt_client.h

    # Headers - Debug Mode
    src/modes/debug_mode/debug_utils.h

    # Headers - Utils
    src/utils/data_type_names.h
    src/utils/server_data.h
    src/utils/attributestatus.h
    src/utils/descriptioninfo.h

    # Implementation - Controllers
    src/controllers/efficiencycontroller.cpp
    src/controllers/chargingcontroller.cpp
    src/controllers/homecontroller.cpp
    src/controllers/headercontroller.cpp
    src/controllers/offviewcontroller.cpp
    src/controllers/buttoncontroller.cpp
    src/controllers/navigationcontroller.cpp
    src/controllers/flappybirdcontroller.cpp
    src/controllers/speedcontroller.cpp
    src/controllers/snakecontroller.cpp
    src/controllers/tankcontroller.cpp

    # Implementation - Models
    src/models/model.cpp
    src/models/raspberry_model.cpp
    src/models/mqtt_client.cpp

    # Implementation - Debug Mode
    src/modes/debug_mode/debug_utils.cpp

    # Implementation - Utils
    src/utils/server_data.cpp
    src/utils/descriptioninfo.cpp

    # Main
    src/main.cpp

    # rss
    application.qrc
)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
# Create executable with all sources consolidated
qt_add_executable(NEROApp ${PROJECT_SOURCES})

# Add protobuf
qt_add_protobuf(NEROApp
    PROTO_FILES
    src/proto/serverdata/serverdata.proto
)


# Link libraries
target_link_libraries(NEROApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Network
    Qt6::Mqtt
    Qt6::Quick
    Qt6::Protobuf
)

set(CMAKE_OBJECT_PATH_MAX 500)

# include the qml config
include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)
include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
include(${CMAKE_CURRENT_SOURCE_DIR}/insight)

# Installation
include(GNUInstallDirs)
install(TARGETS NEROApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
