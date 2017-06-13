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

#ifndef TESTCONNECTION_HPP_HPP_201706130910
#define TESTCONNECTION_HPP_HPP_201706130910

#include <string>
#include "HostMonitor.hpp"

namespace host_monitor {

/**
 * @brief test if a host is reachable over the specified @p protocol.
 * @param[in] endpoint   The target of the connection test attempt.
 * @returns true if @target could be reached via @p protocol. if not: false.
 */
auto testConnection(Endpoint const& endpoint) -> bool;

} // namespace host_monitor

#endif // TESTCONNECTION_HPP_HPP_201706130910
