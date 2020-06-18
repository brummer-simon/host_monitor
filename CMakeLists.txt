# Setup Project and Build
cmake_minimum_required(VERSION 3.16)

project(host_monitor
    VERSION   1.1.0
    LANGUAGES CXX
)

# Specify public headers
list(APPEND ${PROJECT_NAME}_INC
    include/Endpoint.hpp
    include/HostMonitor.hpp
    include/HostMonitorObserver.hpp
    include/Version.hpp
)

# Specify source files
list(APPEND ${PROJECT_NAME}_SRC
    src/Endpoint.cpp
    src/HostMonitor.cpp
    src/TestConnection.cpp
    src/Version.cpp
)

# Specify test sources
list(APPEND ${PROJECT_NAME}_TEST_SRC
    test/main.cpp
    test/VersionTest.cpp
    test/HostMonitorTest.cpp
    test/HostMonitorObserverTest.cpp
)

# Setup build
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME}
    SHARED
        "${${PROJECT_NAME}_SRC}"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        VERSION_MINOR=${PROJECT_VERSION_MINOR}
        VERSION_PATCH=${PROJECT_VERSION_PATCH}
        VERSION=${PROJECT_VERSION}
)

target_compile_options(${PROJECT_NAME}
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wtype-limits
        -Wstrict-overflow=5
        -fstrict-overflow
        -Wsign-compare
        -Wconversion
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        PUBLIC_HEADER "${${PROJECT_NAME}_INC}"
        VERSION       "${PROJECT_VERSION}"
        SOVERSION     "${PROJECT_VERSION}"
)

# Setup GTest
enable_testing()
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_test
    "${${PROJECT_NAME}_TEST_SRC}"
)

target_link_libraries(${PROJECT_NAME}_test
    PUBLIC
        ${PROJECT_NAME}
        ${GTEST_LIBRARIES}
)

gtest_add_tests(
    TARGET
        ${PROJECT_NAME}_test
)

# Setup deployment
install(
    TARGETS
        ${PROJECT_NAME}

    LIBRARY DESTINATION
        /usr/lib

    ARCHIVE DESTINATION
        /usr/lib

    PUBLIC_HEADER DESTINATION
        /usr/include/${PROJECT_NAME}

    CONFIGURATIONS
        Release
)


