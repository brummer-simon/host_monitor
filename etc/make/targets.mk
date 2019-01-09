# Make Target definition
# Author: Simon Brummer <simon.brummer@posteo.de>

include $(ETC_MAKE_DIR)/sanity_checks.mk

# Setup phony and silent targets
.PHONY: run_sanity_checks \
        create_project_structure \
        prebuild \
        prebuild_doc \
        postbuild \
        clean_debug \
        clean_release \
        clean_gtest \
        clean_doc \
        clean_all \
        install_include \
        install_debug \
        install_release \
        uninstall_include \
        uninstall_debug \
        uninstall_release \
        help

.SILENT: create_project_structure \
         prebuild_doc \
         clean_debug \
         clean_release \
         clean_gtest \
         clean_doc \
         clean_all \
         exec_debug_bin \
         exec_release_bin \
         exec_gtest_bin \
         install_include \
         install_debug \
         install_release \
         uninstall_include \
         help

# The usual make black magic goes here...
# 1) Append Project Type specific flags for compilation and linkage
ifeq ($(PROJECT_TYPE), libshared)
CPPFLAGS_DEBUG   += -fPIC
CPPFLAGS_RELEASE += -fPIC
LDFLAGS_DEBUG    += -shared
LDFLAGS_RELEASE  += -shared
endif

# 2) Define pattern rules for each build configuration
# 2.1) Debug build pattern rule
$(OBJ_DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(info $(info )Compiling: "$<"")
	$(CC) -c $(STD) $(CPPFLAGS) $(CPPFLAGS_DEBUG) $(INCLUDES) $(DEFINES) $(WARNINGS) $< -o $@

# 2.2) Release build pattern rule
$(OBJ_RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(info $(info )Compiling: "$<"")
	$(CC) -c $(STD) $(CPPFLAGS) $(CPPFLAGS_RELEASE) $(INCLUDES) $(DEFINES) $(WARNINGS) $< -o $@

# 2.3) Unit test build pattern rule
$(OBJ_GTEST_DIR)/%.o: $(GTEST_DIR)/%.cpp
	$(info $(info )Compiling: "$<"")
	$(CC) -c $(STD) $(CPPFLAGS) $(CPPFLAGS_GTEST) $(INCLUDES) $(DEFINES) $(WARNINGS) $< -o $@

# 2) Generate Object file names via substitution.
OBJ       := $(SRC:%.cpp=%.o)
GTEST_OBJ := $(GTEST_SRC:%.cpp=%.o)

# 3) Generate Paths to the plain object, source files and dist locations.
OBJ_DEBUG   := $(OBJ:%=$(OBJ_DEBUG_DIR)/%)
OBJ_RELEASE := $(OBJ:%=$(OBJ_RELEASE_DIR)/%)
OBJ_GTEST   := $(GTEST_OBJ:%=$(OBJ_GTEST_DIR)/%)

INSTALL_INC_DIR := $(INSTALL_INC_DIR)/$(PROJECT_NAME)

# 4) Linker/Archiver targets.
$(BIN_DEBUG): $(OBJ_DEBUG)
ifdef USE_LINKER
	$(info $(info )Linking "$@":)
	$(CC) $(LDFLAGS) $(LDFLAGS_DEBUG) $(OBJ_DEBUG) -o $(BIN_DEBUG) $(LIBS_DEBUG)
endif
ifdef USE_ARCHIVER
	$(info $(info )Archiving "$@":)
	$(AR) crf $(BIN_DEBUG) $(OBJ_DEBUG)
endif
ifdef MAKE_SYMLINKS
	$(CMD_LN) $(ABS_OUT_DIR)/$(BIN_DEBUG_FULL_VERSION)  $(ABS_OUT_DIR)/$(BIN_DEBUG_MINOR_VERSION)
	$(CMD_LN) $(ABS_OUT_DIR)/$(BIN_DEBUG_MINOR_VERSION) $(ABS_OUT_DIR)/$(BIN_DEBUG_MAJOR_VERSION)
	$(CMD_LN) $(ABS_OUT_DIR)/$(BIN_DEBUG_MAJOR_VERSION) $(ABS_OUT_DIR)/$(BIN_DEBUG_NO_VERSION)
endif

$(BIN_RELEASE): $(OBJ_RELEASE)
ifdef USE_LINKER
	$(info $(info )Linking: "$@"")
	$(CC) $(LDFLAGS) $(LDFLAGS_RELEASE) $(OBJ_RELEASE) -o $(BIN_RELEASE) $(LIBS_RELEASE)
endif
ifdef USE_ARCHIVER
	$(info $(info )Archiving "$@":)
	$(AR) crf $(BIN_RELEASE) $(OBJ_RELEASE)
endif
ifdef MAKE_SYMLINKS
	$(CMD_LN) $(ABS_OUT_DIR)/$(BIN_RELEASE_FULL_VERSION)  $(ABS_OUT_DIR)/$(BIN_RELEASE_MINOR_VERSION)
	$(CMD_LN) $(ABS_OUT_DIR)/$(BIN_RELEASE_MINOR_VERSION) $(ABS_OUT_DIR)/$(BIN_RELEASE_MAJOR_VERSION)
	$(CMD_LN) $(ABS_OUT_DIR)/$(BIN_RELEASE_MAJOR_VERSION) $(ABS_OUT_DIR)/$(BIN_RELEASE_NO_VERSION)
endif

$(BIN_GTEST): $(OBJ_GTEST)
	$(info $(info )Linking: "$@")
	$(CC) $(LDFLAGS) $(LDFLAGS_GTEST) $(OBJ_GTEST) -o $(BIN_GTEST) $(LIBS_GTEST)

# 5) Basic build targets
create_project_structure:
	$(CMD_MKDIR) $(INC_DIR)
	$(CMD_MKDIR) $(SRC_DIR)
	$(CMD_MKDIR) $(GTEST_DIR)
	$(CMD_MKDIR) $(OUT_DIR)
	$(CMD_MKDIR) $(OBJ_DEBUG_DIR)
	$(CMD_MKDIR) $(OBJ_RELEASE_DIR)
	$(CMD_MKDIR) $(OBJ_GTEST_DIR)
	$(CMD_MKDIR) $(DOC_DIR)
	$(CMD_MKDIR) $(DOC_HTML_DIR)
	$(CMD_MKDIR) $(ETC_DIR)
	$(CMD_MKDIR) $(ETC_MAKE_DIR)
	$(CMD_MKDIR) $(ETC_SBIN_DIR)
	$(CMD_MKDIR) $(ETC_DOC_DIR)
	$(CMD_CHMOD) $(ETC_SBIN_DIR)

prebuild: run_sanity_checks create_project_structure
	$(info Building $(TYPE_NAME) "$(PROJECT_NAME)".)

postbuild:
	$(info )

prebuild_doc: run_sanity_checks create_project_structure
	$(info Update Documentation)
	cat $(DOC_CFG_TEMPLATE) > $(DOC_CFG)
	sed -i "s/^PROJECT_NAME =.*/PROJECT_NAME = $(PROJECT_NAME)/g" $(DOC_CFG)
	sed -i "s/^PROJECT_NUMBER =.*/PROJECT_NUMBER = $(VERSION)/g"  $(DOC_CFG)

# Targets for all configurations
build_debug: prebuild $(BIN_DEBUG) postbuild

build_release: prebuild $(BIN_RELEASE) postbuild

build_gtest: prebuild $(BIN) $(BIN_GTEST) postbuild

build_debug_gtest: prebuild $(BIN_DEBUG) $(BIN_GTEST) postbuild

build_doc: prebuild_doc
	$(DOCTOOL) $(DOC_CFG)

clean_debug:
	$(CMD_RM) $(OBJ_DEBUG_DIR) $(OUT_DIR)/$(BIN_DEBUG_NO_VERSION)*

clean_release:
	$(CMD_RM) $(OBJ_RELEASE_DIR) $(OUT_DIR)/$(BIN_RELEASE_NO_VERSION)*

clean_gtest:
	$(CMD_RM) $(OBJ_GTEST_DIR) $(BIN_GTEST)

clean_doc:
	$(CMD_RM) $(DOC_HTML_DIR)

exec_debug_bin: build_debug
	$(info Executing: $(BIN_DEBUG) $(RUN_ARGS))
	$(BIN_DEBUG) $(RUN_ARGS)

exec_release_bin: build_release
	$(info Executing: $(BIN_RELEASE) $(RUN_ARGS))
	$(BIN_RELEASE) $(RUN_ARGS)

exec_gtest_bin: build_gtest
	$(info Executing: $(BIN_GTEST) $(TEST_ARGS))
	$(BIN_GTEST) $(TEST_ARGS)

exec_debugger_debug: build_debug
	$(info Executing in Debugger: $(BIN_DEBUG) $(RUN_ARGS))
	$(DEBUGGER) --args $(RUN_ARGS) $(BIN_DEBUG)

exec_debugger_gtest: build_debug_gtest
	$(info Executing in Debugger: $(BIN_GTEST) $(TEST_ARGS))
	$(DEBUGGER) --args $(TEST_ARGS) $(BIN_GTEST)

install_include:
ifneq ($(PROJECT_TYPE), binary)
	$(info Copy header files to "$(INSTALL_INC_DIR)".)
	$(ETC_SBIN_DIR)/install_includes.sh $(INC_DIR) $(INSTALL_INC_DIR)
endif

install_debug: build_debug install_include
ifeq ($(PROJECT_TYPE), binary)
	$(info Copy binary "$(BIN_DEBUG)" to "$(INSTALL_BIN_DIR)".)
	$(CMD_CP) $(BIN_DEBUG) $(INSTALL_BIN_DIR)
else ifeq ($(PROJECT_TYPE), libstatic)
	$(info Copy static library "$(BIN_DEBUG)" to "$(INSTALL_LIB_DIR)".)
	$(CMD_CP) $(BIN_DEBUG) $(INSTALL_LIB_DIR)
else ifeq ($(PROJECT_TYPE), libshared)
	$(info Copy shared library "$(BIN_DEBUG)" to "$(INSTALL_LIB_DIR)".)
	$(CMD_CP) $(BIN_DEBUG) $(INSTALL_LIB_DIR)
	$(info Create symlinks to shared library "$(BIN_DEBUG)" in "$(INSTALL_LIB_DIR)".)
	$(CMD_LN) $(INSTALL_LIB_DIR)/$(BIN_DEBUG_FULL_VERSION)  $(INSTALL_LIB_DIR)/$(BIN_DEBUG_MINOR_VERSION)
	$(CMD_LN) $(INSTALL_LIB_DIR)/$(BIN_DEBUG_MINOR_VERSION) $(INSTALL_LIB_DIR)/$(BIN_DEBUG_MAJOR_VERSION)
	$(CMD_LN) $(INSTALL_LIB_DIR)/$(BIN_DEBUG_MAJOR_VERSION) $(INSTALL_LIB_DIR)/$(BIN_DEBUG_NO_VERSION)
endif

install_release: build_release install_include
ifeq ($(PROJECT_TYPE), binary)
	$(info Copy binary "$(BIN_RELEASE)" to "$(INSTALL_BIN_DIR)".)
	$(CMD_CP) $(BIN_RELEASE) $(INSTALL_BIN_DIR)
else ifeq ($(PROJECT_TYPE), libstatic)
	$(info Copy static library "$(BIN_RELEASE)" to "$(INSTALL_LIB_DIR)".)
	$(CMD_CP) $(BIN_RELEASE) $(INSTALL_LIB_DIR)
else ifeq ($(PROJECT_TYPE), libshared)
	$(info Copy shared library "$(BIN_RELEASE)" to "$(INSTALL_LIB_DIR)".)
	$(CMD_CP) $(BIN_RELEASE) $(INSTALL_LIB_DIR)
	$(info Create symlinks to shared library "$(BIN_RELEASE)" in "$(INSTALL_LIB_DIR)".)
	$(CMD_LN) $(INSTALL_LIB_DIR)/$(BIN_RELEASE_FULL_VERSION)  $(INSTALL_LIB_DIR)/$(BIN_RELEASE_MINOR_VERSION)
	$(CMD_LN) $(INSTALL_LIB_DIR)/$(BIN_RELEASE_MINOR_VERSION) $(INSTALL_LIB_DIR)/$(BIN_RELEASE_MAJOR_VERSION)
	$(CMD_LN) $(INSTALL_LIB_DIR)/$(BIN_RELEASE_MAJOR_VERSION) $(INSTALL_LIB_DIR)/$(BIN_RELEASE_NO_VERSION)
endif

uninstall_include:
ifneq ($(PROJECT_TYPE), binary)
	$(info Remove header files from "$(INSTALL_INC_DIR)".)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_INC_DIR)
endif

uninstall_debug: uninstall_include
ifeq ($(PROJECT_TYPE), binary)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_BIN_DIR)/$(BIN_DEBUG_NO_VERSION)
else ifeq ($(PROJECT_TYPE), libstatic)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_DEBUG_NO_VERSION)
else ifeq ($(PROJECT_TYPE), libshared)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_DEBUG_NO_VERSION)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_DEBUG_MAJOR_VERSION)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_DEBUG_MINOR_VERSION)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_DEBUG_FULL_VERSION)
endif

uninstall_release: uninstall_include
ifeq ($(PROJECT_TYPE), binary)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_BIN_DIR)/$(BIN_RELEASE_NO_VERSION)
else ifeq ($(PROJECT_TYPE), libstatic)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_RELEASE_NO_VERSION)
else ifeq ($(PROJECT_TYPE), libshared)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_RELEASE_NO_VERSION)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_RELEASE_MAJOR_VERSION)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_RELEASE_MINOR_VERSION)
	$(ETC_SBIN_DIR)/uninstall_save.sh $(INSTALL_LIB_DIR)/$(BIN_RELEASE_FULL_VERSION)
endif

# Targets for Type: binary
ifeq ($(PROJECT_TYPE), binary)
ifeq ($(PROJECT_CONFIG), debug)
build:      build_debug
clean:      clean_debug
exec:       exec_debug_bin
exec_debug: exec_debugger_debug
install:    install_debug
uninstall:  uninstall_debug
else ifeq ($(PROJECT_CONFIG), release)
build:      build_release
clean:      clean_release
exec:       exec_release_bin
exec_debug: exec_debugger_debug
install:    install_release
uninstall:  uninstall_release
endif
all: clean_debug clean_release build_debug build_release
endif

# Targets for Type: libstatic
ifeq ($(PROJECT_TYPE), libstatic)
ifeq ($(PROJECT_CONFIG), debug)
build:      build_debug
clean:      clean_debug clean_gtest
test:       exec_gtest_bin
test_debug: exec_debugger_gtest
install:    install_debug
uninstall:  uninstall_debug
else ifeq ($(PROJECT_CONFIG), release)
build:      build_release
clean:      clean_release clean_gtest
test:       exec_gtest_bin
test_debug: exec_debugger_gtest
install:    install_release
uninstall:  uninstall_release
endif
all: clean_debug clean_release build_debug build_release
endif

# Targets for Type: libshared
ifeq ($(PROJECT_TYPE), libshared)
ifeq ($(PROJECT_CONFIG), debug)
build:      build_debug
clean:      clean_debug clean_gtest
test:       exec_gtest_bin
test_debug: exec_debugger_gtest
install:    install_debug
uninstall:  uninstall_debug
else ifeq ($(PROJECT_CONFIG), release)
build:      build_release
clean:      clean_release clean_gtest
test:       exec_gtest_bin
test_debug: exec_debugger_gtest
install:    install_release
uninstall:  uninstall_release
endif
all: clean_debug clean_release build_debug build_release
endif

# Common Targets
clean_all: clean_doc
	$(CMD_RM) $(OUT_DIR)/*

doc: clean_doc build_doc
rebuild: clean build

help:
	$(info Availabe make target matrix:)
	$(info +-----------------+-----------+-----------+-----------+--------------------------------------+)
	$(info | Project Type:   | binary    | libstatic | libshared | Description                          |)
	$(info | Project Config: | dbg | rel | dbg | rel | dbg | rel |                                      |)
	$(info +-----------------+-----+-----+-----+-----+-----+-----+--------------------------------------+)
	$(info | build           |  X  |  X  |  X  |  X  |  X  |  X  | Build current config                 |)
	$(info | clean           |  X  |  X  |  X  |  X  |  X  |  X  | Clean build artefacts (current cfg)  |)
	$(info | exec            |  X  |  X  |     |     |     |     | Execute build result                 |)
	$(info | exec_debug      |  X  |  X  |     |     |     |     | Execute build result in debugger     |)
	$(info | test            |     |     |  X  |  X  |  X  |  X  | Build and run unittests              |)
	$(info | test_debug      |     |     |  X  |  X  |  X  |  X  | Build and run unittests in debugger  |)
	$(info | install         |  X  |  X  |  X  |  X  |  X  |  X  | Install build result in host system  |)
	$(info | uninstall       |  X  |  X  |  X  |  X  |  X  |  X  | Remove build result from host system |)
	$(info | doc             |  X  |  X  |  X  |  X  |  X  |  X  | Create documentation in doc          |)
	$(info | rebuild         |  X  |  X  |  X  |  X  |  X  |  X  | Rebuilds current configuration       |)
	$(info | all             |  X  |  X  |  X  |  X  |  X  |  X  | Rebuild debug and release config     |)
	$(info | clean_all       |  X  |  X  |  X  |  X  |  X  |  X  | Clean all build artefacts            |)
	$(info | help            |  X  |  X  |  X  |  X  |  X  |  X  | Prints this help screen              |)
	$(info +-----------------+-----+-----+-----+-----+-----+-----+--------------------------------------+)

