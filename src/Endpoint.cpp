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


Endpoint::Endpoint( Endpoint::Protocol         protocol
                  , std::string                fqhn
                  , std::optional<std::string> port)
    : protocol_(protocol)
    , fqhn_(std::move(fqhn))
    , port_(std::move(port))
{
}

auto Endpoint::make_icmp_endpoint(std::string fqhn) -> Endpoint
{
    return Endpoint(Protocol::ICMP, std::move(fqhn), std::nullopt);
}

auto Endpoint::make_tcp_endpoint(std::string fqhn, std::string port) -> Endpoint
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

auto Endpoint::get_target() const -> std::string
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
