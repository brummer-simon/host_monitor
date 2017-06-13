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

/**
 * @brief Observer interface intended to use with HostMonitor object.
 * @note  Any object that inherits from this interface can be registered
 *        as Observer.
 */
class HostMonitorObserver
{
public:
    /**
     * @brief Destructor
     */
    virtual ~HostMonitorObserver()
    {
    }

    /**
     * @brief Update method, called then the connection to a
     *        monitored target is either lost or re-established.
     * @param[in] targetAvail   Current state of the monitored target.
     *                          true if target is reachable, false if target is not reachable.
     */
    virtual auto update(bool targetAvail) -> void = 0;
};

#endif // HOSTMONITOROBSERVER_HPP_201706130847
