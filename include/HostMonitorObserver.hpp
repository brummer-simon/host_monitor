/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   HostMonitorObserver.hpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#ifndef HOSTMONITOROBSERVER_HPP_201706130847
#define HOSTMONITOROBSERVER_HPP_201706130847

#include <vector>
#include <chrono>
#include <cstdint>

#include "Endpoint.hpp"

namespace host_monitor
{

/**
 * @brief Observer interface intended to use with HostMonitor object.
 * @note  Any object that inherits from this interface can be registered
 *        as observer.
 */
class HostMonitorObserver
{
public:
    virtual ~HostMonitorObserver() = default;

    /**
     * @brief Update method, called then the connection to a monitored target
     *        is either lost or re-established.
     * @note The implementation of state_change is called
     *       from the host monitors kontext. Synchronization might be needed.
     * @param[in] endpoint    Reference to the Endpoint, of the
     *                        host monitor, this observer is registered on.
     * @param[in] metadata    Reference to the metadata, of the
     *                        host monitor, this observer is registered on.
     * @param[in] interval    Reference to the test interval, of the
     *                        host monitor, this observer is registered on.
     * @param[in] available   Current state of the monitored endpoint.
     */
    virtual void state_change( Endpoint const&             endpoint
                             , std::vector<uint8_t> const& metadata
                             , std::chrono::seconds const& interval
                             , bool                        available) = 0;
};

} // namespace host_monitor
#endif // HOSTMONITOROBSERVER_HPP_201706130847
