/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Endpoint.cpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#include "Endpoint.hpp"

namespace host_monitor
{

// Protocol related implementation
auto protocol_to_string(Endpoint::Protocol p) -> std::string
{
    switch(p)
    {
        case Endpoint::Protocol::ICMP:
            return std::string("ICMP");

        case Endpoint::Protocol::TCP:
            return std::string("TCP");
    }
    return std::string();
}

auto string_to_protocol(std::string const& s) -> std::optional<Endpoint::Protocol>
{
    if (s == "ICMP")
    {
        return Endpoint::Protocol::ICMP;
    }

    if (s == "TCP")
    {
        return Endpoint::Protocol::TCP;
    }
    return {};
}


Endpoint::Endpoint( Endpoint::Protocol                protocol
                  , std::string const&                fqhn
                  , std::optional<std::string> const& port)
    : protocol_(protocol)
    , fqhn_(fqhn)
    , port_(port)
{
}

auto Endpoint::make_icmp_endpoint(std::string const& fqhn) -> Endpoint
{
    return Endpoint(Protocol::ICMP, fqhn, std::nullopt);
}

auto Endpoint::make_tcp_endpoint(std::string const& fqhn, std::string const& port) -> Endpoint
{
    // Try to convert the given port number to an int
    int val = 0;
    try {
        val = std::stoi(port);
    }
    catch (...)
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": " + port + " is not a valid number");
    }

    // Check if given Port number is within the port number range
    int min = 0x0001;
    int max = 0xFFFF;
    if (val < min || max < val)
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": " + port + " is not in [1, 65535]");
    }

    // Create endpoint
    return Endpoint(Endpoint::Protocol::TCP, fqhn, port);
}

auto Endpoint::get_target() const -> std::string
{
    std::string str;
    switch(protocol_)
    {
        case Endpoint::Protocol::ICMP:
            str = get_fqhn();
            break;

        case Endpoint::Protocol::TCP:
            str = get_fqhn() + ":" + get_port().value();
            break;
    }
    return str;
}

auto Endpoint::get_fqhn() const -> std::string
{
    return fqhn_;
}

auto Endpoint::get_port() const -> std::optional<std::string>
{
    return port_;
}

auto Endpoint::get_protocol() const -> Protocol
{
    return protocol_;
}

} // namespace host_monitor
