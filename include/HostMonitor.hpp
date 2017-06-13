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

#include <string>
#include <chrono>
#include <memory>
#include <cstdint>
#include "HostMonitorObserver.hpp"

namespace host_monitor {

class HostMonitorImpl;

/**
 * @brief Protocols that can be used for host monitoring.
 */
typedef enum
{
    ICMP = 0   // Use ICMP messages (ping) to reach host.
} Protocol;

/**
 * @brief Endpoint structure used to hold connection parameters
 */
struct Endpoint
{
    /**
     * @brief Default constructor. Use generator functions instead.
     */
    Endpoint(Protocol           protocol,
             std::string const& targetAddr,
             std::uint16_t      targetPort);

    Protocol const      protocol;   // Protocol to use for connection tests
    std::string  const  targetAddr; // Target Address: IP or FQDN
    std::uint16_t const targetPort; // Target Port: Portno of the Target.
};

/**
 * @brief Function to generate an ICMP Endpoint.
 * @param[in] target     The target that should be monitored. Either FQDN or IP-Address.
 */
auto makeIcmpEndpoint(std::string const& targetAddr) -> Endpoint;

/**
 * @brief Checks if a specified host is reachable over the specified protocol.
 *        The checks are performed on a regular basis.
 */
class HostMonitor
{
public:

	/**
     * @brief Fully specified constructor.
     * @throws std::runtime_error then @p target is illegal value.
     * @param[in] target     The target that should be monitored. @See Endpoint.
     * @param[in] alias      Human friendly alias for @p target.
     * @param[in] interval   The duration between performed connection tests.
     */
    HostMonitor(Endpoint                    target,
                std::string const&          alias,
                std::chrono::seconds const& interval);

    /**
     * @brief Destructor.
     */
    ~HostMonitor();

    /**
     * @brief Add an Observer to the monitor.
     * @note @p observer must derive from the Interface 'HostMonitorObserver'.
     *       The update-method is called on each registered observer in case of a
     *       state change.
     * @param[in] observer   The observer that should be added.
     */
    auto addObserver(std::shared_ptr<HostMonitorObserver>& observer) -> void;

    /**
     * @brief Remove an Observer from the monitor.
     * @param[in] observer   The observer that should be removed.
     */
    auto delObserver(std::shared_ptr<HostMonitorObserver>& observer) -> void;

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
    HostMonitorImpl *pimpl_; // Pointer to implementation
};

/**
 * @brief Creates shared pointer holding a newly constructed HostMonitor object.
 * @param[in] target     The target that should be monitored. @See Endpoint.
 * @param[in] alias      Human friendly alias for @p target.
 * @param[in] interval   The duration between performed connection tests.
 * @returns   shared pointer holding new HostMonitor.
 */
auto makeHostMonitor(Endpoint                    target,
                     std::string const&          alias,
                     std::chrono::seconds const& interval) -> std::shared_ptr<HostMonitor>;

} // namespace host_monitor

#endif // HOSTMONITOR_HPP_201706130847
