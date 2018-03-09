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
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include "HostMonitor.hpp"
#include "TestConnection.hpp"

namespace host_monitor
{

class HostMonitor::Impl
{
public:
    Impl( Endpoint                    endpoint
        , std::vector<uint8_t> const& metadata
        , std::chrono::seconds const& interval);

    ~Impl();

    void add_observer(std::shared_ptr<HostMonitorObserver> observer);

    void del_observer(std::shared_ptr<HostMonitorObserver> observer);

    auto is_available() const -> bool;

    auto get_endpoint() const -> Endpoint;

    auto get_metadata() const -> std::vector<uint8_t>;

    auto get_interval() const -> std::chrono::seconds;

private:
    void monitor_target();

    using ObserverVector = std::vector<std::shared_ptr<HostMonitorObserver>>;

    Endpoint                endpoint_;      // Endpoint: @See Endpoint.
    std::vector<uint8_t>    metadata_;      // Alias for Target
    std::chrono::seconds    interval_;      // Interval between Connection Tests
    std::thread             thread_;        // Thread performing periodic tests
    std::mutex              mtx_;           // Mutex to use with condition variables
    std::condition_variable cv_;            // Thread sleeping condition
    bool                    shutdown_;      // Thread life-time management Flag
    bool                    available_;     // Holds result from last connection test
    ObserverVector          observers_;     // Vector holding registered observers
    std::mutex              observers_mtx_; // Lock for synchronizing access to observers_
};

HostMonitor::Impl::Impl( Endpoint                    endpoint
                       , std::vector<uint8_t> const& metadata
                       , std::chrono::seconds const& interval)
    : endpoint_(endpoint)
    , metadata_(metadata)
    , interval_(interval)
    , thread_()
    , mtx_()
    , cv_()
    , shutdown_(false)
    , available_(false)
    , observers_()
    , observers_mtx_()
{
    // Start Monitor thread in case given endpoints protocol is supported
    thread_ = std::thread(&HostMonitor::Impl::monitor_target, this);
}

HostMonitor::Impl::~Impl()
{
    {
        auto lock = std::unique_lock<std::mutex>(mtx_);
        shutdown_ = true;
        cv_.notify_one();
    }
    thread_.join();
}

void HostMonitor::Impl::add_observer(std::shared_ptr<HostMonitorObserver> observer)
{
    std::lock_guard<std::mutex> lock(observers_mtx_);
    observers_.push_back(observer);
}

void HostMonitor::Impl::del_observer(std::shared_ptr<HostMonitorObserver> observer)
{
    std::lock_guard<std::mutex> lock(observers_mtx_);
    auto pos = std::remove(observers_.begin(), observers_.end(), observer);
    observers_.erase(pos, observers_.end());
}

auto HostMonitor::Impl::is_available() const -> bool
{
    return available_;
}

auto HostMonitor::Impl::get_endpoint() const -> Endpoint
{
    return endpoint_;
}

auto HostMonitor::Impl::get_metadata() const -> std::vector<uint8_t>
{
    return metadata_;
}

auto HostMonitor::Impl::get_interval() const -> std::chrono::seconds
{
    return interval_;
}

void HostMonitor::Impl::monitor_target()
{
    while (shutdown_ == false)
    {
        // Perform connection test
        auto available_n1 = bool(available_);
        auto available_n = bool(test_connection(endpoint_));

        // Update State and inform observers
        if (available_n1 != available_n)
        {
            available_ = available_n;

            // Update Observers on state change
            std::lock_guard<std::mutex> lock(observers_mtx_);
            for (auto obs : observers_)
            {
                obs->state_change(endpoint_, metadata_, interval_, available_);
            }

            // Update old state
            available_n1 = available_n;
        }

        // Sleep until duration expired or a shutdown is initiated
        auto lock = std::unique_lock<std::mutex>(mtx_);
        auto pred = [this] ()
        {
            return shutdown_;
        };
        cv_.wait_for(lock, interval_, pred);
    }
}

// Interface Implementation
HostMonitor::HostMonitor( Endpoint                    endpoint
                        , std::vector<uint8_t> const& metadata
                        , std::chrono::seconds const& interval)
{
    pimpl_ = std::make_unique<Impl>(endpoint, metadata, interval);
}

HostMonitor::~HostMonitor() = default;

void HostMonitor::add_observer(std::shared_ptr<HostMonitorObserver> observer)
{
    pimpl_->add_observer(observer);
}

void HostMonitor::del_observer(std::shared_ptr<HostMonitorObserver> observer)
{
    pimpl_->del_observer(observer);
}

auto HostMonitor::is_available() const -> bool
{
    return pimpl_->is_available();
}

auto HostMonitor::get_endpoint() const -> Endpoint
{
    return pimpl_->get_endpoint();
}

auto HostMonitor::get_metadata() const -> std::vector<uint8_t>
{
    return pimpl_->get_metadata();
}

auto HostMonitor::get_interval() const -> std::chrono::seconds
{
    return pimpl_->get_interval();
}

} // namespace host_monitor
