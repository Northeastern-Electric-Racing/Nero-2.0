cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(NEROApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)


if (Qt6_VERSION VERSION_GREATER_EQUAL 6.5)
    qt_standard_project_setup()
endif()

set(PROJECT_SOURCES
    src/controllers/chargingcontroller.h
    src/constants/fault_statuses.h
    src/controllers/homecontroller.h
    src/controllers/efficiencycontroller.h
    src/models/mock_model.h
    src/models/model.h
    src/models/raspberry_model.h
    src/modes/debug_mode/debug_utils.h

    src/controllers/efficiencycontroller.cpp
    src/controllers/chargingcontroller.cpp
    src/controllers/homecontroller.cpp
    src/main.cpp
    src/models/mock_model.cpp
    src/models/model.cpp
    src/models/raspberry_model.cpp
    src/modes/debug_mode/debug_utils.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Mqtt)

qt_add_executable(NEROApp ${PROJECT_SOURCES}
    src/models/mqttclient.h src/models/mqttclient.cpp)

qt_add_resources(NEROApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
include_directories("src/models")


target_link_libraries(NEROApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Network
    Qt6::Mqtt
    Qt6::Quick
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


