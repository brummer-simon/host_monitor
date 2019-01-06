/**
 * @file      Endpoint.cpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Endpoint.hpp"

namespace host_monitor
{

// Protocol related implementation
std::string protocol_to_string(Endpoint::Protocol p)
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

std::optional<Endpoint::Protocol> string_to_protocol(std::string const& s)
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


Endpoint::Endpoint( Endpoint::Protocol         protocol
                  , std::string                fqhn
                  , std::optional<std::string> port)
    : protocol_(protocol)
    , fqhn_(std::move(fqhn))
    , port_(std::move(port))
{
}

Endpoint Endpoint::make_icmp_endpoint(std::string fqhn)
{
    return Endpoint(Protocol::ICMP, std::move(fqhn), std::nullopt);
}

Endpoint Endpoint::make_tcp_endpoint(std::string fqhn, std::string port)
{
    // Try to convert the given port number to an int
    auto val = 0;
    try
    {
        val = std::stoi(port);
    }
    catch (...)
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": " + port + " is not a valid number");
    }

    // Check if given Port number is within the port number range
    auto min = std::uint16_t(0x0001);
    auto max = std::uint16_t(0xFFFF);
    if (val < min || max < val)
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": " + port + " is not in [1, 65535]");
    }

    // Create endpoint
    return Endpoint(Endpoint::Protocol::TCP, std::move(fqhn), std::move(port));
}

std::string Endpoint::get_target() const
{
    auto str = std::string();
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

std::string Endpoint::get_fqhn() const
{
    return fqhn_;
}

std::optional<std::string> Endpoint::get_port() const
{
    return port_;
}

Endpoint::Protocol Endpoint::get_protocol() const
{
    return protocol_;
}

} // namespace host_monitor
