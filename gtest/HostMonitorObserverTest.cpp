/**
 * @file      HostMonitorTest.cpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <thread>
#include <chrono>
#include <memory>
#include <gtest/gtest.h>
#include "HostMonitor.hpp"
#include "HostMonitorObserver.hpp"

using host_monitor::Endpoint;
using host_monitor::HostMonitor;

struct Observer : public host_monitor::HostMonitorObserver
{
    virtual void state_change(Data const& data) override
    {
        available = data.available;
    }

    bool available = false;
};


TEST(HostMonitorObserverTest, ICMPToGoogle)
{
    // Create Monitor.
    auto ep = Endpoint::make_icmp_endpoint("www.google.de");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));
    auto obs = std::make_shared<Observer>();

    mon.add_observer(obs);

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_TRUE(obs->available);
}

TEST(HostMonitorObserverTest, TCPToGoogle)
{
    // Create Monitor.
    auto ep = Endpoint::make_tcp_endpoint("www.google.de", "80");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));
    auto obs = std::make_shared<Observer>();

    mon.add_observer(obs);

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_TRUE(obs->available);
}

TEST(HostMonitorObserverTest, ICMPToInvalid)
{
    // Create Monitor.
    auto ep = Endpoint::make_icmp_endpoint("asdkhads.local");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));
    auto obs = std::make_shared<Observer>();

    mon.add_observer(obs);

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_FALSE(obs->available);
}

TEST(HostMonitorObserverTest, TCPToInvalid)
{
    // Create Monitor.
    auto ep = Endpoint::make_tcp_endpoint("asdkhads.local", "80");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));
    auto obs = std::make_shared<Observer>();

    mon.add_observer(obs);

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_FALSE(obs->available);
}

