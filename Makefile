# Generic c++ project make template.
# Author: Simon Brummer <simon.brummer@posteo.de>

# --- Global Project settings ---
PROJECT_NAME     := host_monitor
PROJECT_TYPE     := libshared
PROJECT_CONFIG   := release
VERSION_MAJOR    := 1
VERSION_MINOR    := 1
VERSION_REVISION := 0

include etc/make/header.mk

# --- Sources files ---
SRC := \
	Endpoint.cpp \
	HostMonitor.cpp \
	TestConnection.cpp \
	Version.cpp

GTEST_SRC := \
	HostMonitorObserverTest.cpp \
	HostMonitorTest.cpp \
	VersionTest.cpp \
	main.cpp

# --- Compiler settings ---
CC := g++

STD := -std=c++17

INCLUDES := \
	-I$(INC_DIR)

DEFINES := \
	$(VERSION_DEFINES)

WARNINGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Werror \
	-Wtype-limits \
	-Wstrict-overflow=5 \
	-fstrict-overflow \
	-Wsign-compare \
	-Wconversion

CPPFLAGS := \

CPPFLAGS_DEBUG := \
	-Og \
	-ggdb

CPPFLAGS_RELEASE := \
	-O2 \
	-DNDEBUG

CPPFLAGS_GTEST := \
	-Og \
	-ggdb

# --- Linker settings ---
LDFLAGS := \

LDFLAGS_DEBUG := \

LDFLAGS_RELEASE := \

LDFLAGS_GTEST := \
	-Wl,-rpath="$(ABS_OUT_DIR)" \

# --- Library settings ---
LIBS_DEBUG := \

LIBS_RELEASE := \

LIBS_GTEST := \
	-L"$(ABS_OUT_DIR)" \
	-l$(BIN_BASENAME) \
	-lgtest \
	-lpthread

# --- Execution Arguments ---
RUN_ARGS := \

TEST_ARGS := \

# ---Install settings ---
INSTALL_INC_DIR := /usr/local/include
INSTALL_BIN_DIR := /usr/local/bin
INSTALL_LIB_DIR := /usr/local/lib

# Include actual make targets
include etc/make/targets.mk
