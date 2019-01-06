/**
 * @file      TestConnection.hpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
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
bool test_connection(Endpoint const& endpoint);

} // namespace host_monitor

#endif // TESTCONNECTION_HPP_201706130910
