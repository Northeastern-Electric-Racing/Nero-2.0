cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(NEROApp LANGUAGES CXX)

# Find Qt6 first before version checks
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Mqtt Protobuf)

# Windows-specific AutoMoc fixes
if(WIN32)
    # Force response files for long command lines on Windows
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS ON)
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES ON)
    set(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")

    # Set AutoMoc to use response files much earlier on Windows
    set(CMAKE_AUTOGEN_COMMAND_LINE_LENGTH_MAX 2000)

    # Reduce AutoMoc parallel processing on Windows to avoid command line issues
    set(CMAKE_AUTOGEN_PARALLEL 1)
endif()

# Enable AutoMoc after Windows-specific settings
set(CMAKE_AUTOMOC ON)

# Fix Qt version check - use Qt6Core_VERSION instead of Qt6_VERSION
if (Qt6Core_VERSION VERSION_GREATER_EQUAL "6.5")
    qt_standard_project_setup()
endif()

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
    src/models/mock_model.h
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
    src/models/mock_model.cpp
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
)

# Create executable with all sources consolidated
qt_add_executable(NEROApp ${PROJECT_SOURCES})

# Add resources
qt_add_resources(NEROApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

# Add protobuf
qt_add_protobuf(NEROApp
    PROTO_FILES
    src/proto/serverdata/serverdata.proto
)

# Fix include directories - remove the broken "src/**" syntax
target_include_directories(NEROApp PRIVATE
    include
    src
    src/constants
    src/controllers
    src/models
    src/modes/debug_mode
    src/utils
    ${CMAKE_CURRENT_BINARY_DIR}  # For AutoMoc generated files
)

# Windows-specific AutoMoc target properties
if(WIN32)
    set_target_properties(NEROApp PROPERTIES
        AUTOGEN_COMMAND_LINE_LENGTH_MAX 2000
        AUTOGEN_PARALLEL 1
        AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/NEROApp_autogen"
    )
endif()

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

# Optional components
if (BUILD_QDS_COMPONENTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

if (LINK_INSIGHT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS NEROApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
