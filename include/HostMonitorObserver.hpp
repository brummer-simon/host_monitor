/**
 * @file      HostMonitorObserver.hpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
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
    /// @brief Contains all information of the registered monitor
    struct Data
    {
        Endpoint const&             endpoint;  ///< Endpoint of the Host monitor this Observer is registered on.
        std::chrono::seconds const& interval;  ///< Test interval of the Host monitor this Observer is registered on.
        bool const                  available; ///< Availability of the monitored endpoint.
    };

    virtual ~HostMonitorObserver() = default;

    /**
     * @brief Update method, called then the connection to a monitored target
     *        is either lost or re-established.
     * @note The implementation of state_change is called
     *       from the host monitors context. Synchronization might be needed.
     * @param[in] data   Data structure holding all Data associated with the state_change.
     */
    virtual void state_change(Data const& data) = 0;
};

} // namespace host_monitor
#endif // HOSTMONITOROBSERVER_HPP_201706130847
