# Setup project paths and names.
# Author: Simon Brummer <simon.brummer@posteo.de>

# -- Setup project directories ---
INC_DIR         := include
SRC_DIR         := src
GTEST_DIR       := gtest
OUT_DIR         := bin
OBJ_DEBUG_DIR   := $(OUT_DIR)/obj_debug
OBJ_RELEASE_DIR := $(OUT_DIR)/obj_release
OBJ_GTEST_DIR   := $(OUT_DIR)/obj_gtest
DOC_DIR         := doc
DOC_HTML_DIR    := $(DOC_DIR)/html
ETC_DIR         := etc
ETC_MAKE_DIR    := $(ETC_DIR)/make
ETC_SBIN_DIR    := $(ETC_DIR)/sbin
ETC_DOC_DIR     := $(ETC_DIR)/doxygen

ABS_PROJECT_DIR := $(shell readlink -f .)
ABS_OUT_DIR     := $(ABS_PROJECT_DIR)/$(OUT_DIR)

# --- Setup project Type and artifact name ---
# 1) Evaluate given PROJECT_TYPE and PROJECT_CONFIG
ifeq ($(PROJECT_TYPE), binary)
	# Binary configuration
	BIN_EXTENSION := elf
	TYPE_NAME     := binary
	USE_LINKER    := true
else ifeq ($(PROJECT_TYPE), libstatic)
	# Static lib configuration
	BIN_PREFIX    := lib
	BIN_EXTENSION := a
	TYPE_NAME     := static library
	USE_ARCHIVER  := true
else ifeq ($(PROJECT_TYPE), libshared)
	# Shared lib configuration
	BIN_PREFIX    := lib
	BIN_EXTENSION := so
	TYPE_NAME     := shared library
	USE_LINKER    := true
	MAKE_SYMLINKS := true
else ifeq ($(PROJECT_TYPE), headeronly)
	# Header only lib configuration
	TYPE_NAME     := header only library
endif

# 2) Create build artifact names with and without version info
PREFIX  := $(BIN_PREFIX)
VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_REVISION)

BIN_DEBUG_BASENAME      := $(PROJECT_NAME)_dbg
BIN_DEBUG_NO_VERSION    := $(PREFIX)$(BIN_DEBUG_BASENAME).$(BIN_EXTENSION)
BIN_DEBUG_MAJOR_VERSION := $(BIN_DEBUG_NO_VERSION).$(VERSION_MAJOR)
BIN_DEBUG_MINOR_VERSION := $(BIN_DEBUG_MAJOR_VERSION).$(VERSION_MINOR)
BIN_DEBUG_FULL_VERSION  := $(BIN_DEBUG_MINOR_VERSION).$(VERSION_REVISION)

BIN_RELEASE_BASENAME      := $(PROJECT_NAME)
BIN_RELEASE_NO_VERSION    := $(PREFIX)$(BIN_RELEASE_BASENAME).$(BIN_EXTENSION)
BIN_RELEASE_MAJOR_VERSION := $(BIN_RELEASE_NO_VERSION).$(VERSION_MAJOR)
BIN_RELEASE_MINOR_VERSION := $(BIN_RELEASE_MAJOR_VERSION).$(VERSION_MINOR)
BIN_RELEASE_FULL_VERSION  := $(BIN_RELEASE_MINOR_VERSION).$(VERSION_REVISION)

BIN_GTEST := $(OUT_DIR)/$(PROJECT_NAME)_gtest.elf

# Select Binary name based on Project Type
ifeq ($(PROJECT_TYPE), binary)
	BIN_DEBUG   := $(OUT_DIR)/$(BIN_DEBUG_NO_VERSION)
	BIN_RELEASE := $(OUT_DIR)/$(BIN_RELEASE_NO_VERSION)
else ifeq ($(PROJECT_TYPE), libstatic)
	BIN_DEBUG   := $(OUT_DIR)/$(BIN_DEBUG_NO_VERSION)
	BIN_RELEASE := $(OUT_DIR)/$(BIN_RELEASE_NO_VERSION)
else ifeq ($(PROJECT_TYPE), libshared)
	BIN_DEBUG   := $(OUT_DIR)/$(BIN_DEBUG_FULL_VERSION)
	BIN_RELEASE := $(OUT_DIR)/$(BIN_RELEASE_FULL_VERSION)
endif

# Select binary based on Project configuration
ifeq ($(PROJECT_CONFIG), debug)
	BIN := $(BIN_DEBUG)
	BIN_BASENAME := $(BIN_DEBUG_BASENAME)
else ifeq ($(PROJECT_CONFIG), release)
	BIN := $(BIN_RELEASE)
	BIN_BASENAME := $(BIN_RELEASE_BASENAME)
endif

# 3) Version number defines used to propagate version info into the build
VERSION_DEFINES := -DVERSION_MAJOR=$(VERSION_MAJOR) \
                   -DVERSION_MINOR=$(VERSION_MINOR) \
                   -DVERSION_REVISION=$(VERSION_REVISION) \
                   -DVERSION=$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_REVISION)

# 4) Define additional files
DOC_CFG_TEMPLATE := $(ETC_DOC_DIR)/doxygen_template_cfg
DOC_CFG          := $(ETC_DOC_DIR)/doxygen_cfg

# 5) Setup misc tools
AR        := gcc-ar
DEBUGGER  := gdbgui
DOCTOOL   := doxygen
CMD_MKDIR := mkdir -p
CMD_RM    := rm -rf
CMD_CP    := cp
CMD_LN    := ln -sf
CMD_CHMOD := chmod 755 -R
