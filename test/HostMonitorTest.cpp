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
#include <gtest/gtest.h>
#include "HostMonitor.hpp"

using host_monitor::Endpoint;
using host_monitor::HostMonitor;

TEST(HostMonitorTest, ICMPv4ToGoogle)
{
    // Create Monitor.
    auto ep = Endpoint::make_icmpv4_endpoint("8.8.8.8");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_TRUE(mon.is_available());
}

TEST(HostMonitorTest, ICMPv6ToGoogle)
{
    // Create Monitor.
    auto ep = Endpoint::make_icmpv6_endpoint("2001:4860:4860::8888");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_TRUE(mon.is_available());
}

TEST(HostMonitorTest, TCPToGoogle)
{
    // Create Monitor.
    auto ep = Endpoint::make_tcp_endpoint("www.google.de", "80");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_TRUE(mon.is_available());
}

TEST(HostMonitorTest, ICMPv4ToInvalid)
{
    // Create Monitor.
    auto ep = Endpoint::make_icmpv4_endpoint("asdkhads.local");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_FALSE(mon.is_available());
}

TEST(HostMonitorTest, ICMPv6ToInvalid)
{
    // Create Monitor.
    auto ep = Endpoint::make_icmpv4_endpoint("asdkhads.local");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_FALSE(mon.is_available());
}

TEST(HostMonitorTest, TCPToInvalid)
{
    // Create Monitor.
    auto ep = Endpoint::make_tcp_endpoint("asdkhads.local", "80");
    auto mon = HostMonitor(ep, std::chrono::seconds(1));

    // Wait for target to respond
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_FALSE(mon.is_available());
}
