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

// Endpoint related implementation
Endpoint::Endpoint(Protocol           protocol,
                   std::string const& addr,
                   std::uint16_t      port)
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
        str = this->getAddr() + ":" + std::to_string(this->getPort());
        break;
    }
    return str;
}

auto Endpoint::getAddr() const -> std::string
{
    return this->targetAddr;
}

auto Endpoint::getPort() const -> std::uint16_t
{
    return this->targetPort;
}

auto Endpoint::getProtocol() const -> Protocol
{
    return this->protocol;
}

auto makeIcmpEndpoint(std::string const& targetAddr) -> Endpoint
{
    return Endpoint(Protocol::ICMP, targetAddr, 0);
}

auto makeTcpEndpoint(std::string const& targetAddr, std::uint16_t targetPort) -> Endpoint
{
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
