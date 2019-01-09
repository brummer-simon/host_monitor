# Project sanity checks
# Author: Simon Brummer <simon.brummer@posteo.de>

# Check given Project Type
ifeq ($(PROJECT_TYPE), binary)
else ifeq ($(PROJECT_TYPE), libstatic)
else ifeq ($(PROJECT_TYPE), libshared)
else ifeq ($(PROJECT_TYPE), headeronly)
else
	INVALID_PROJECT_TYPE := true
endif

# Check given Build Type
ifeq ($(PROJECT_CONFIG), debug)
else ifeq ($(PROJECT_CONFIG), release)
else
	INVALID_PROJECT_BUILD := true
endif

# Add target for sanity checking
run_sanity_checks:
ifdef INVALID_PROJECT_TYPE
	$(error Invalid PROJECT_TYPE. It must be either "binary", "libstatic", "libshared" or "headeronly".)
endif
ifdef INVALID_PROJECT_CONFIG
	$(error Invalid PROJECT_CONFIG. It must be either "debug" or "release".)
endif
