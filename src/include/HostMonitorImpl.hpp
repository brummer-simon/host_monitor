/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   HostMonitorImpl.hpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#ifndef HOSTMONITORIMPL_HPP_201706130900
#define HOSTMONITORIMPL_HPP_201706130900

#include <thread>
#include <condition_variable>
#include <vector>
#include <memory>
#include "HostMonitor.hpp"
#include "HostMonitorObserver.hpp"

namespace host_monitor {

// Internally used Types
typedef std::shared_ptr<HostMonitorObserver> ObserverPtr;
typedef std::vector<ObserverPtr>             ObserverVector;

class HostMonitorImpl
{
public:
    /**
     * @brief Fully specified constructor.
     * @throws std::runtime_error then @p protocol is a illegal value.
     * @param[in] endpoint   The target that should be monitored. @See Endpoint.
     * @param[in] alias      Human friendly alias for @p target.
     * @param[in] interval   The duration between performed connection tests.
     */
    HostMonitorImpl(Endpoint                    endpoint,
                    std::string const&          alias,
                    std::chrono::seconds const& interval);

    /**
     * @brief Destructor.
     */
    ~HostMonitorImpl();

    /**
     * @brief Add an Observer to the monitor.
     * @note @p observer must derive from the Interface 'HostMonitorObserver'.
     *       The update-method is called on each registered observer in case of a
     *       state change.
     * @param[in] observer   The observer that should be added.
     */
    auto addObserver(ObserverPtr& observer) -> void;

    /**
     * @brief Remove an Observer from the monitor.
     * @param[in] observer   The observer that should be removed.
     */
    auto delObserver(ObserverPtr& observer) -> void;

    /**
     * @brief Check if target was reachable after the last connection check.
     * @returns true if target was reachable
     *          false if target was not reachable
     */
    auto avail() const -> bool;

    // Getter
    auto getEndpoint() const -> Endpoint;
    auto getAlias() const -> std::string;
    auto getInterval() const -> std::chrono::seconds;

private:
    Endpoint                endpoint_;      // Endpoint: @See Endpoint.
    std::string             alias_;         // Alias for Target
    std::chrono::seconds    interval_;      // Interval between Connection Tests
    std::thread             thread_;        // Thread performing periodic tests
    std::condition_variable condVar_;       // Thread sleeping condition
    bool                    shutdown_;      // Thread life-time management Flag
    bool                    avail_;         // Holds result from last connection test
    ObserverVector          observers_;     // Vector holding registered observers
    std::mutex              observersLock_; // Lock for synchronizing access to observers_

    // Internal thread method.
    auto monitorTarget() -> void;
};

} // namespace host_monitor

#endif // HOSTMONITORIMPL_HPP_201706130900
