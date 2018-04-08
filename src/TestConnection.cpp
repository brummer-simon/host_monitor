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

#include "TestConnection.hpp"

namespace host_monitor
{
namespace
{
// network layer connection test is based on ping.
auto test_connection_icmp(std::string const& fqhn) -> bool
{
    // Create command
    std::string ping_cmd;

    ping_cmd += "ping -c 1 ";        // Send single ICMP packet
    ping_cmd += fqhn;                // Specify fqhn
    ping_cmd += " > /dev/null 2>&1"; // Discard output from command

    // Call command
    return (std::system(ping_cmd.c_str()) == 0) ? true : false;
}

// transport layer connection test is based on nc.
auto test_connection_tcp(std::string const& fqhn, std::string const& port) -> bool
{
    // Create command
    std::string nc_cmd;

    nc_cmd += "nc -z "; // Establish connection, close afterwards
    nc_cmd += fqhn;     // Specify address
    nc_cmd += " ";
    nc_cmd += port;     // Specify port

    // Call command
    return (std::system(nc_cmd.c_str()) == 0) ? true : false;
}
} // anon namespace

auto test_connection(Endpoint const& endpoint) -> bool
{
    // Demux by specified protocol
    switch (endpoint.get_protocol())
    {
        case Endpoint::Protocol::ICMP:
            return test_connection_icmp(endpoint.get_fqhn());

        case Endpoint::Protocol::TCP:
            return test_connection_tcp( endpoint.get_fqhn()
                                      , endpoint.get_port().value());

    // NOTE: Add additional protocol support here ....
    }
    return false;
}

} // namespace host_monitor
