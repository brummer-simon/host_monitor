/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   HostMonitorImpl.cpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#include <iostream>
#include <algorithm>
#include "include/HostMonitorImpl.hpp"
#include "include/TestConnection.hpp"

namespace host_monitor {

HostMonitorImpl::HostMonitorImpl(Endpoint                    endpoint,
                                 std::string const&          alias,
                                 std::chrono::seconds const& interval)
    : endpoint_(endpoint)
    , alias_(alias)
    , interval_(interval)
    , thread_()
    , condVar_()
    , shutdown_(false)
    , avail_(false)
{
    // Test given protocol on construction.
    if (endpoint_.protocol != Protocol::ICMP)
    {
        throw std::runtime_error("HostMonitor: Supplied unknown protocol");
    }

    this->thread_ = std::thread(&HostMonitorImpl::monitorTarget, this);
}

HostMonitorImpl::~HostMonitorImpl()
{
    this->shutdown_ = true;
    this->condVar_.notify_one();
    this->thread_.join();
}

auto HostMonitorImpl::addObserver(ObserverPtr& observer) -> void
{
    std::unique_lock<std::mutex> l(this->observersLock_);
    this->observers_.push_back(observer);
}

auto HostMonitorImpl::delObserver(ObserverPtr& observer) -> void
{
    std::unique_lock<std::mutex> l(this->observersLock_);
    auto pos = std::remove(this->observers_.begin(), this->observers_.end(), observer);
    this->observers_.erase(pos, this->observers_.end());
}

auto HostMonitorImpl::avail() const -> bool
{
    return this->avail_;
}

auto HostMonitorImpl::getEndpoint() const -> Endpoint
{
    return this->endpoint_;
}

auto HostMonitorImpl::getAlias() const -> std::string
{
    return this->alias_;
}

auto HostMonitorImpl::getInterval() const -> std::chrono::seconds
{
    return this->interval_;
}

auto HostMonitorImpl::monitorTarget() -> void
{
    while (!this->shutdown_)
    {
        // Perform connection test
        bool oldAvail = this->avail_;
        bool newAvail = testConnection(this->endpoint_);

        // Update State and inform observers
        if (oldAvail != newAvail)
        {
            this->avail_ = newAvail;

            // Update Observers on state change
            std::unique_lock<std::mutex> l(this->observersLock_);
            for (auto it = this->observers_.begin(); it != this->observers_.end(); ++it)
            {
                (*it)->update(this->avail_);
            }
        }

        // Sleep until duration expired or a shutdown is initiated
        if (!this->shutdown_)
        {
            std::mutex                   m;
            std::unique_lock<std::mutex> l(m);
            this->condVar_.wait_for(l, this->interval_);
        }
    }
}

} // namespace host_monitor
