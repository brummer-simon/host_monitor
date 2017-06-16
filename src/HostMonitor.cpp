/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   HostMonitor.cpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#include <cstdint>
#include "HostMonitor.hpp"
#include "include/HostMonitorImpl.hpp"

namespace host_monitor {

// Protocol related implementation
auto protocolToString(Protocol p) -> std::string
{
    switch(p)
    {
    case Protocol::ICMP:
        return "icmp";

    case Protocol::TCP:
        return "tcp";

    default:
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": given Protocol is found");
    }
}

auto stringToProtocol(std::string s) -> Protocol
{
    if (s == "icmp")
    {
        return Protocol::ICMP;
    }

    if (s == "tcp")
    {
        return Protocol::TCP;
    }
    throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                             ": " + s + " is not in Protocols");
}

// Endpoint related implementation
Endpoint::Endpoint(Protocol           protocol,
                   std::string const& addr,
                   std::string const& port)
    : protocol(protocol)
    , targetAddr(addr)
    , targetPort(port)
{
}

auto Endpoint::getTarget() const -> std::string
{
    std::string str;
    switch(protocol)
    {
    case Protocol::ICMP:
        str = this->getAddr();
        break;

    case Protocol::TCP:
        str = this->getAddr() + ":" + this->getPort();
        break;
    }
    return str;
}

auto Endpoint::getAddr() const -> std::string
{
    return this->targetAddr;
}

auto Endpoint::getPort() const -> std::string
{
    return this->targetPort;
}

auto Endpoint::getProtocol() const -> Protocol
{
    return this->protocol;
}

auto makeIcmpEndpoint(std::string const& targetAddr) -> Endpoint
{
    return Endpoint(Protocol::ICMP, targetAddr, "");
}

auto makeTcpEndpoint(std::string const& targetAddr, std::string const& targetPort) -> Endpoint
{
    // Try to convert the given port number to an int
    int val = 0;
    try {
        val = std::stoi(targetPort);
    }
    catch (...)
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": " + targetPort + " is not a valid number");
    }

    // Check if given Port number is within the port number range
    int min = 0x0001;
    int max = 0xFFFF;
    if (val < min || max < val)
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                 ": " + targetPort + " is not in [1, 65535]");
    }

    // Create endpoint
    return Endpoint(Protocol::TCP, targetAddr, targetPort);
}

// Host Monitor related implementation
HostMonitor::HostMonitor(Endpoint                    target,
                         std::string const&          alias,
                         std::chrono::seconds const& interval)
{
    this->pimpl_ = new HostMonitorImpl(target, alias, interval);
}

HostMonitor::~HostMonitor()
{
    delete this->pimpl_;
}

auto HostMonitor::addObserver(std::shared_ptr<HostMonitorObserver>& observer) -> void
{
    this->pimpl_->addObserver(observer);
}

auto HostMonitor::delObserver(std::shared_ptr<HostMonitorObserver>& observer) -> void
{
    this->pimpl_->delObserver(observer);
}

auto HostMonitor::avail() const -> bool
{
    return this->pimpl_->avail();
}

auto HostMonitor::getEndpoint() const -> Endpoint
{
    return this->pimpl_->getEndpoint();
}

auto HostMonitor::getAlias() const -> std::string
{
    return this->pimpl_->getAlias();
}

auto HostMonitor::getInterval() const -> std::chrono::seconds
{
    return this->pimpl_->getInterval();
}

auto makeHostMonitor(Endpoint                    target,
                     std::string const&          alias,
                     std::chrono::seconds const& interval) -> std::shared_ptr<HostMonitor>
{
    return std::make_shared<HostMonitor>(target, alias, interval);
}

} // namespace host_monitor
