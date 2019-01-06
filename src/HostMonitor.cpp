/**
 * @file      HostMonitor.cpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <cstdint>

#include "HostMonitor.hpp"
#include "TestConnection.hpp"

namespace host_monitor
{

class HostMonitor::Impl
{
public:
    Impl(Endpoint endpoint, std::chrono::seconds interval);

    ~Impl();

    void add_observer(std::shared_ptr<HostMonitorObserver> observer);

    void del_observer(std::shared_ptr<HostMonitorObserver> observer);

    bool is_available() const;

    Endpoint const& get_endpoint() const;

    std::vector<uint8_t> const& get_metadata() const;

    std::chrono::seconds const&get_interval() const;

private:
    void monitor_target();

    using ObserverVector = std::vector<std::shared_ptr<HostMonitorObserver>>;

    Endpoint                endpoint_;      // Endpoint: @See Endpoint.
    std::chrono::seconds    interval_;      // Interval between Connection Tests
    std::thread             thread_;        // Thread performing periodic tests
    std::mutex              mtx_;           // Mutex to use with condition variables
    std::condition_variable cv_;            // Thread sleeping condition
    bool                    shutdown_;      // Thread life-time management Flag
    bool                    available_;     // Holds result from last connection test
    ObserverVector          observers_;     // Vector holding registered observers
    std::mutex              observers_mtx_; // Lock for synchronizing access to observers_
};

HostMonitor::Impl::Impl(Endpoint endpoint, std::chrono::seconds interval)
    : endpoint_(std::move(endpoint))
    , interval_(std::move(interval))
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
    auto lock = std::lock_guard<std::mutex>(observers_mtx_);
    observers_.push_back(observer);
}

void HostMonitor::Impl::del_observer(std::shared_ptr<HostMonitorObserver> observer)
{
    auto lock = std::lock_guard<std::mutex>(observers_mtx_);
    auto pos = std::remove(observers_.begin(), observers_.end(), observer);
    observers_.erase(pos, observers_.end());
}

bool HostMonitor::Impl::is_available() const
{
    return available_;
}

Endpoint const& HostMonitor::Impl::get_endpoint() const
{
    return endpoint_;
}

std::chrono::seconds const& HostMonitor::Impl::get_interval() const
{
    return interval_;
}

void HostMonitor::Impl::monitor_target()
{
    while (shutdown_ == false)
    {
        // Perform connection test
        auto available_n1 = available_;
        auto available_n = test_connection(endpoint_);

        // Update State and inform observers
        if (available_n1 != available_n)
        {
            available_ = available_n;

            // Construct Data Object
            auto const data = HostMonitorObserver::Data{endpoint_, interval_, available_};

            // Update Observers on state change
            auto lock = std::lock_guard<std::mutex>(observers_mtx_);
            for (auto obs : observers_)
            {
                obs->state_change(data);
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
HostMonitor::HostMonitor(Endpoint endpoint, std::chrono::seconds interval)
{
    pimpl_ = std::make_unique<Impl>(std::move(endpoint), std::move(interval));
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

bool HostMonitor::is_available() const
{
    return pimpl_->is_available();
}

Endpoint const& HostMonitor::get_endpoint() const
{
    return pimpl_->get_endpoint();
}

std::chrono::seconds const& HostMonitor::get_interval() const
{
    return pimpl_->get_interval();
}

} // namespace host_monitor
