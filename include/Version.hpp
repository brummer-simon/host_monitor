/**
 * @file      Version.hpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef VERSION_HPP_20181213224112
#define VERSION_HPP_20181213224112

namespace host_monitor
{

/**
 * @brief Libraries Version information.
 */
struct Version
{
    static char const * const major; ///< Major Version number.
    static char const * const minor; ///< Minor Version number.
    static char const * const patch; ///< Patch number.
    static char const * const full;  ///< Full version number.
};

} // namespace host_monitor

#endif // VERSION_HPP_20181213224112
