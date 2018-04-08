/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   TestConnection.hpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#ifndef TESTCONNECTION_HPP_201706130910
#define TESTCONNECTION_HPP_201706130910

#include "HostMonitor.hpp"

namespace host_monitor
{

/**
 * @brief Function to test if a given endpoint can be reached.
 * @param[in] endpoint   the endpoint to test.
 * @returns true in case @p endpoint is reachable. false if not.
 */
auto test_connection(Endpoint const& endpoint) -> bool;

} // namespace host_monitor

#endif // TESTCONNECTION_HPP_201706130910
