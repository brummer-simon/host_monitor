# Generic c++ project make template.
# Author: Simon Brummer <simon.brummer@posteo.de>

# --- Global Project settings ---
PROJECT_NAME     := host_monitor
PROJECT_TYPE     := libshared
PROJECT_CONFIG   := gtest
VERSION_MAJOR    := 1
VERSION_MINOR    := 0
VERSION_REVISION := 0

include etc/make/header

# --- Sources files ---
SRC := \
	Endpoint.cpp \
	TestConnection.cpp \
	HostMonitor.cpp \
	Version.cpp

GTEST_SRC := \
	VersionTest.cpp \
	HostMonitorTest.cpp \
	HostMonitorObserverTest.cpp \
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

# --- Library settings ---
LIBS_DEBUG := \

LIBS_RELEASE := \

LIBS_GTEST := \
	-L$(OUT_DIR) \
	-l$(BIN_DEBUG_BASENAME) \
	-lgtest \
	-lpthread

# --- Execution Arguments ---
RUN_ARGS := \

DEBUG_ARGS := \

# --- Distribution settings ---
DIST_INC_DIR := /usr/include
DIST_BIN_DIR := /usr/bin
DIST_LIB_DIR := /usr/lib

# --- Misc. commands and tools ---
DEBUGGER  := gdbgui

# Include actual make targets
include etc/make/targets
