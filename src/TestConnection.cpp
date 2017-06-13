/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   TestConnection.cpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#include <cstdlib>
#include "include/TestConnection.hpp"

namespace host_monitor {

namespace
{
    // network layer connection test is based on ping.
    auto testConnectionIcmp(std::string const& target) -> bool
    {
        // Create command
        std::string pingCmd;

        pingCmd += "ping -c 1 ";        // Send single ICMP packet
        pingCmd += target;              // Specify target
        pingCmd += " > /dev/null 2>&1"; // Discard output from command

        // Call command
        return (std::system(pingCmd.c_str()) == 0) ? true : false;
    }
} // anon namespace

auto testConnection(Endpoint const& endpoint) -> bool
{
    // Demux by specified protocol
    switch (endpoint.protocol)
    {
    case Protocol::ICMP:
        return testConnectionIcmp(endpoint.targetAddr);

    // NOTE: Add additional protocol support here ....
    }
    return false;
}

} // namespace host_monitor
