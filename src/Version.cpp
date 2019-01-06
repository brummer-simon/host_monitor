/**
 * @file      Version.cpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Version.hpp"

// Preprocessor magic to convert given input to string.
#define STRINGIFY(x) STR(x)
#define STR(x) #x

namespace host_monitor
{
// Initialize Version numbers with given version from makefile.
char const * const Version::major = STRINGIFY(VERSION_MAJOR);
char const * const Version::minor = STRINGIFY(VERSION_MINOR);
char const * const Version::rev   = STRINGIFY(VERSION_REVISION);
char const * const Version::full  = STRINGIFY(VERSION);
} // namespace_monitor
