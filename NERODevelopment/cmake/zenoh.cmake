# ============================================================
# Zenoh — the transport NERO reads telemetry from
#
# zenoh-cpp is a header-only C++17 wrapper around zenoh-c, which is a Rust
# library shipped as a prebuilt binary. An installed zenoh-c always wins, and
# that is how the car build gets it: Odysseus's nerlibzenoh-c package builds
# it into the Buildroot sysroot. A desktop build downloads the release
# matching the host instead. Same local-copy-first shape the DOOM engine above
# uses, and it keeps Rust off developer machines.
#
# Defines zenoh::cxx: the headers, the backend switch, and the zenoh-c library.
# That is all upstream's zenohcxx::zenohc target is, and spelling it out here
# keeps zenoh-cpp's own tests, examples, docs and install rules out of our
# build.
# ============================================================

include(FetchContent)

set(ZENOH_VERSION "1.10.1" CACHE STRING "Version of the zenoh-c/zenoh-cpp pair to build against")

# find_package caches zenohc_DIR, so once a release has been downloaded into
# the build tree a re-configure would find that copy and take it for a system
# install. Settle the question once and remember the answer.
if(NOT DEFINED ZENOH_C_SOURCE)
    # An install from zenoh-c's own CMake build, which ships package files
    find_package(zenohc ${ZENOH_VERSION} QUIET)
    # Buildroot builds zenoh-c with cargo alone, so the sysroot gets only the
    # library and headers
    find_path(ZENOHC_INCLUDE_DIR zenoh.h)
    find_library(ZENOHC_LIBRARY zenohc)

    if(zenohc_FOUND)
        set(ZENOH_C_SOURCE "system" CACHE INTERNAL "Where zenoh-c comes from")
    elseif(ZENOHC_INCLUDE_DIR AND ZENOHC_LIBRARY)
        set(ZENOH_C_SOURCE "library" CACHE INTERNAL "Where zenoh-c comes from")
    elseif(CMAKE_CROSSCOMPILING)
        # A prebuilt bypasses the sysroot and its C library, which is exactly
        # what a cross build exists to link against
        message(FATAL_ERROR "Zenoh: zenoh-c ${ZENOH_VERSION} is not in the sysroot. Install it for the target"
                            " first (on the car, Odysseus's nerlibzenoh-c package) and re-run CMake.")
    else()
        set(ZENOH_C_SOURCE "release" CACHE INTERNAL "Where zenoh-c comes from")
    endif()
endif()

if(ZENOH_C_SOURCE STREQUAL "system")
    find_package(zenohc ${ZENOH_VERSION} REQUIRED)
    message(STATUS "Zenoh: using installed zenoh-c (${zenohc_DIR})")
    set(ZENOH_C_TARGET zenohc::lib)
elseif(ZENOH_C_SOURCE STREQUAL "library")
    # zenoh-c's build writes the version it was built at into this header;
    # without package files it is the only place left to check it
    set(zenoh_c_version "")
    if(EXISTS ${ZENOHC_INCLUDE_DIR}/zenoh_configure.h)
        file(STRINGS ${ZENOHC_INCLUDE_DIR}/zenoh_configure.h zenoh_c_version REGEX "^#define ZENOH_C \"")
        string(REGEX REPLACE "^#define ZENOH_C \"([^\"]*)\".*" "\\1" zenoh_c_version "${zenoh_c_version}")
    endif()
    if(NOT zenoh_c_version OR zenoh_c_version VERSION_LESS ZENOH_VERSION)
        message(FATAL_ERROR "Zenoh: found zenoh-c ${zenoh_c_version} at ${ZENOHC_LIBRARY}, but the zenoh-cpp"
                            " ${ZENOH_VERSION} headers need zenoh-c ${ZENOH_VERSION} or newer.")
    endif()
    message(STATUS "Zenoh: using installed zenoh-c ${zenoh_c_version} (${ZENOHC_LIBRARY})")

    add_library(zenohc_installed UNKNOWN IMPORTED)
    set_target_properties(zenohc_installed PROPERTIES
        IMPORTED_LOCATION ${ZENOHC_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${ZENOHC_INCLUDE_DIR}
    )
    set(ZENOH_C_TARGET zenohc_installed)
else()
    # Release triples, from https://github.com/eclipse-zenoh/zenoh-c/releases
    if(WIN32)
        if(MSVC)
            set(zenoh_c_triple "x86_64-pc-windows-msvc")
        else()
            set(zenoh_c_triple "x86_64-pc-windows-gnu")
        endif()
    elseif(APPLE)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
            set(zenoh_c_triple "aarch64-apple-darwin")
        else()
            set(zenoh_c_triple "x86_64-apple-darwin")
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
            set(zenoh_c_triple "aarch64-unknown-linux-gnu")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7")
            set(zenoh_c_triple "armv7-unknown-linux-gnueabihf")
        else()
            set(zenoh_c_triple "x86_64-unknown-linux-gnu")
        endif()
    else()
        message(FATAL_ERROR "Zenoh: no prebuilt zenoh-c for ${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}."
                            " Install zenoh-c ${ZENOH_VERSION} and re-run CMake.")
    endif()

    message(STATUS "Zenoh: downloading zenoh-c ${ZENOH_VERSION} for ${zenoh_c_triple}")
    FetchContent_Declare(zenohc_prebuilt
        URL https://github.com/eclipse-zenoh/zenoh-c/releases/download/${ZENOH_VERSION}/zenoh-c-${ZENOH_VERSION}-${zenoh_c_triple}-standalone.zip
        DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    )
    FetchContent_MakeAvailable(zenohc_prebuilt)
    find_package(zenohc ${ZENOH_VERSION} REQUIRED PATHS ${zenohc_prebuilt_SOURCE_DIR} NO_DEFAULT_PATH)

    # The downloaded copy is not on any library search path, so link it
    # statically and leave the build output a single self-contained binary
    set(ZENOH_C_TARGET zenohc::static)
endif()

# SOURCE_SUBDIR names a directory with no CMakeLists.txt, which makes
# FetchContent fetch the headers without configuring zenoh-cpp as a subproject.
# Headers are the same on every platform, so a cross build fetches them too.
FetchContent_Declare(zenohcxx
    URL https://github.com/eclipse-zenoh/zenoh-cpp/archive/refs/tags/${ZENOH_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    SOURCE_SUBDIR headers-only
)
FetchContent_MakeAvailable(zenohcxx)

add_library(zenoh_cxx INTERFACE)
target_include_directories(zenoh_cxx SYSTEM INTERFACE ${zenohcxx_SOURCE_DIR}/include)
target_compile_definitions(zenoh_cxx INTERFACE ZENOHCXX_ZENOHC)
target_link_libraries(zenoh_cxx INTERFACE ${ZENOH_C_TARGET})
add_library(zenoh::cxx ALIAS zenoh_cxx)
