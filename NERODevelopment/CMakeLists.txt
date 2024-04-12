cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(NEROApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)


if (Qt6_VERSION VERSION_GREATER_EQUAL 6.5)
    qt_standard_project_setup()
endif()

set(PROJECT_SOURCES
    src/constants/fault_statuses.h

    src/controllers/chargingcontroller.h
    src/controllers/homecontroller.h
    src/controllers/offviewcontroller.h
    src/controllers/efficiencycontroller.h
    src/controllers/headercontroller.h
    src/controllers/debugtablecontroller.h
    src/controllers/offviewcontroller.h
    src/controllers/buttoncontroller.h
    src/controllers/navigationcontroller.h
    src/controllers/flappybirdcontroller.h
    src/controllers/keyboardcontroller.h
    src/controllers/configurationcontroller.h
    src/controllers/debuggraphcontroller.h

    src/models/mock_model.h
    src/models/model.h
    src/models/raspberry_model.h
    src/models/mqtt_client.h

    src/modes/debug_mode/debug_utils.h

    src/utils/data_type_names.h
    src/utils/server_data.h
    src/utils/attributestatus.h
    src/utils/descriptioninfo.h

    src/controllers/efficiencycontroller.cpp
    src/controllers/chargingcontroller.cpp
    src/controllers/homecontroller.cpp
    src/controllers/headercontroller.cpp
    src/controllers/debugtablecontroller.cpp
    src/controllers/offviewcontroller.cpp
    src/controllers/buttoncontroller.cpp
    src/controllers/navigationcontroller.cpp
    src/controllers/flappybirdcontroller.cpp
    src/controllers/keyboardcontroller.cpp
    src/controllers/configurationcontroller.cpp
    src/controllers/debuggraphcontroller.cpp

    src/models/mock_model.cpp
    src/models/model.cpp
    src/models/raspberry_model.cpp
    src/models/mqtt_client.cpp

    src/modes/debug_mode/debug_utils.cpp

    src/utils/server_data.cpp
    src/utils/descriptioninfo.cpp

    src/main.cpp

    src/controllers/speedcontroller.h
    src/controllers/speedcontroller.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Mqtt Protobuf Widgets)

qt_add_executable(NEROApp ${PROJECT_SOURCES})

qt_add_resources(NEROApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

qt_add_protobuf(NEROApp
    PROTO_FILES
    src/proto/serverdata/serverdata.proto
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
include_directories("src/**")


target_link_libraries(NEROApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Network
    Qt6::Mqtt
    Qt6::Quick
    Qt6::Protobuf
    Qt6::Widgets
)

if (BUILD_QDS_COMPONENTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

if (LINK_INSIGHT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif ()

include(GNUInstallDirs)
install(TARGETS NEROApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


