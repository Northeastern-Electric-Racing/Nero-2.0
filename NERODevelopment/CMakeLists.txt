cmake_minimum_required(VERSION 3.21.1)

project(NEROApp LANGUAGES CXX C)

# Find Qt6 first before version checks
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Mqtt Protobuf)

qt_standard_project_setup(REQUIRES 6.8)

# ============================================================
# DOOM Integration — doomgeneric engine
# Source: https://github.com/ozkl/doomgeneric
#
# Strategy: Check for a local copy in third_party/ first. If none
# exists, use FetchContent to auto-download from GitHub.
# This ensures offline builds work if someone has cloned the repo
# manually, while still being zero-friction for fresh clones.
# ============================================================
set(DOOMGENERIC_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/doomgeneric/doomgeneric)

if(EXISTS ${DOOMGENERIC_LOCAL_DIR}/doomgeneric.h)
    # Local copy found — use it directly, skip download
    set(DOOMGENERIC_DIR ${DOOMGENERIC_LOCAL_DIR})
    message(STATUS "DOOM: Using local source at ${DOOMGENERIC_DIR}")
else()
    # No local copy — download via FetchContent
    message(STATUS "DOOM: No local source found, downloading via FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        doomgeneric
        GIT_REPOSITORY https://github.com/ozkl/doomgeneric.git
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(doomgeneric)
    set(DOOMGENERIC_DIR ${doomgeneric_SOURCE_DIR}/doomgeneric)
    message(STATUS "DOOM: Downloaded to ${DOOMGENERIC_DIR}")
endif()

file(GLOB DOOM_SOURCES ${DOOMGENERIC_DIR}/*.c)

# Remove ALL platform backends (doomgeneric_*.c) — we provide our own in doomcontroller.cpp
# The core engine file is doomgeneric.c (no underscore suffix) so it is kept
list(FILTER DOOM_SOURCES EXCLUDE REGEX "doomgeneric_[a-z].*\\.c$")

# Remove SDL and Allegro sound/music backends — we don't have those libraries
list(FILTER DOOM_SOURCES EXCLUDE REGEX "i_sdl.*\\.c$")
list(FILTER DOOM_SOURCES EXCLUDE REGEX "i_allegro.*\\.c$")

# Suppress warnings in third-party DOOM code (unmodified upstream C from the 90s)
set_source_files_properties(${DOOM_SOURCES} PROPERTIES COMPILE_FLAGS "-w")

# ============================================================

# Consolidate all sources in one place
set(PROJECT_SOURCES
    # Headers - Constants
    src/constants/fault_statuses.h

    # Headers - Controllers
    src/controllers/chargingcontroller.h
    src/controllers/homecontroller.h
    src/controllers/offviewcontroller.h
    src/controllers/endurancecontroller.h
    src/controllers/headercontroller.h
    src/controllers/buttoncontroller.h
    src/controllers/navigationcontroller.h
    src/controllers/flappybirdcontroller.h
    src/controllers/speedcontroller.h
    src/controllers/snakecontroller.h
    src/controllers/game2048controller.h
    src/controllers/tankcontroller.h
    src/controllers/doomcontroller.h

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
    src/controllers/endurancecontroller.cpp
    src/controllers/chargingcontroller.cpp
    src/controllers/homecontroller.cpp
    src/controllers/headercontroller.cpp
    src/controllers/offviewcontroller.cpp
    src/controllers/buttoncontroller.cpp
    src/controllers/navigationcontroller.cpp
    src/controllers/flappybirdcontroller.cpp
    src/controllers/speedcontroller.cpp
    src/controllers/snakecontroller.cpp
    src/controllers/game2048controller.cpp
    src/controllers/tankcontroller.cpp
    src/controllers/doomcontroller.cpp

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

# Create executable with all sources + DOOM engine
qt_add_executable(NEROApp ${PROJECT_SOURCES} ${DOOM_SOURCES})

# Add doomgeneric headers to include path
target_include_directories(NEROApp PRIVATE ${DOOMGENERIC_DIR})

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

# Copy WAD file to build directory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources/doom/DOOM1.WAD)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/doom/DOOM1.WAD
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    message(STATUS "DOOM: WAD file copied to build directory")
endif()

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
