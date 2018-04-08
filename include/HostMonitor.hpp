/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   HostMonitor.hpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#ifndef HOSTMONITOR_HPP_201706130847
#define HOSTMONITOR_HPP_201706130847

#include <vector>
#include <optional>
#include <string>
#include <chrono>
#include <memory>
#include <cstdint>

#include "Endpoint.hpp"
#include "HostMonitorObserver.hpp"

namespace host_monitor
{

/**
 * @brief Checks if a specified Endpoint is reachable.
 *        The checks are performed on a regular basis.
 */
class HostMonitor
{
public:

    /**
     * @brief Constructor.
     * @param[in] endpoint   The target that should be monitored. @See Endpoint.
     * @param[in] metadata   Free field to use how ever you want.
     * @param[in] interval   The duration between performed connection tests.
     */
    HostMonitor( Endpoint                    endpoint
               , std::vector<uint8_t> const& metadata
               , std::chrono::seconds const& interval);

    ~HostMonitor();

    /**
     * @brief Add an Observer to the monitor.
     * @note @p observer must derive from the Interface 'HostMonitorObserver'.
     *       The update-method is called on each registered observer in case of a state change.
     * @param[in] observer   The observer that should be added.
     */
    void add_observer(std::shared_ptr<HostMonitorObserver> observer);

    /**
     * @brief Remove an Observer from the monitor.
     * @param[in] observer   The observer that should be removed.
     */
    void del_observer(std::shared_ptr<HostMonitorObserver> observer);

    /**
     * @brief Check if target was reachable after the last connection check.
     * @returns true if target was reachable
     *          false if target was not reachable
     */
    auto is_available() const -> bool;

    /**
     * @brief Get monitored endpoint.
     * @returns Copy of the monitored endpoint.
     */
    auto get_endpoint() const -> Endpoint;

    /**
     * @brief Get metadata associated with monitor.
     * @returns Copy of the associated metadata.
     */
    auto get_metadata() const -> std::vector<std::uint8_t>;

    /**
     * @brief Get test interval of the monitor.
     * @returns Copy of the test interval.
     */
    auto get_interval() const -> std::chrono::seconds;

    /* Disable copying and moving */
    HostMonitor(HostMonitor const& other) = delete;
    HostMonitor(HostMonitor&& other) = delete;
    HostMonitor& operator = (HostMonitor const& other) = delete;
    HostMonitor&& operator = (HostMonitor&& other) = delete;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace host_monitor

#endif // HOSTMONITOR_HPP_201706130847
