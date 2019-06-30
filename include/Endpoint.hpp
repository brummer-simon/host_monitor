/**
 * @file      Version.hpp
 * @author    Simon Brummer <simon.brummer@posteo.de>
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef ENDPOINT_HPP_201706130847
#define ENDPOINT_HPP_201706130847

#include <string>
#include <optional>
#include <cstdint>

namespace host_monitor
{
/// @brief Endpoint structure used to hold connection parameters.
class Endpoint
{
public:
    /// @brief Protocol type used to reach an Endpoint.
    enum class Protocol
    {
        ICMPV4 = 0, ///< Use ICMP Packets to reach Endpoint via IPv4.
        ICMPV6,     ///< Use ICMP Packets to reach Endpoint via IPv6.
        TCP,        ///< Use TCP Packets to reach Endpoint.
    };

    /**
     * @brief Build icmpv4 endpoint.
     * @param[in] fqhn   The target that should be monitored. Either FQDN or IPv4-Address.
     * @returns Configured Endpoint.
     */
    static Endpoint make_icmpv4_endpoint(std::string fqhn);

    /**
     * @brief Build icmpv6 endpoint.
     * @param[in] fqhn   The target that should be monitored. Either FQDN or IPv6-Address.
     * @returns Configured Endpoint.
     */
    static Endpoint make_icmpv6_endpoint(std::string fqhn);

    /**
     * @brief Function to generate an TCP Endpoint.
     * @throws std::runtime_error in case @p port is invalid.
     * @param[in] fqhn   The target that should be monitored. Either FQDN or IP-Address.
     * @param[in] port   Port number to connect to.
     * @returns Configured Endpoint.
     */
    static Endpoint make_tcp_endpoint(std::string fqhn, std::string port);

    /**
     * @brief Get endpoints target.
     * @returns string in the form "<fqhn>:<port>".
     */
    std::string get_target() const;

    /**
     * @brief Get endpoints fqhn.
     * @returns fqhn of the endpoint.
     */
    std::string get_fqhn() const;

    /**
     * @brief Get endpoints port.
     * @returns port if endpoint in case it exists.
     */
    std::optional<std::string> get_port() const;

    /**
     * @brief Get Protocol of Endpoint.
     * @returns protocol of endpoint.
     */
    Endpoint::Protocol get_protocol() const;

private:
    Endpoint( Endpoint::Protocol         protocol
            , std::string                fqhn
            , std::optional<std::string> port);

    Endpoint::Protocol         protocol_;
    std::string                fqhn_;
    std::optional<std::string> port_;
};

/**
 * @brief Convert Protocol enum to string representation.
 * @param[in] p   The protocol value that should be converted.
 * @returns @p as string.
 */
std::string protocol_to_string(Endpoint::Protocol p);

/**
 * @brief Convert String to Protocol enum.
 * @param[in] s   the string that should be parsed.
 * @returns @p as protocol. In case the given string can't be converted to a
 *          protocol, the optional is none.
 */
std::optional<Endpoint::Protocol> string_to_protocol(std::string const& s);

} // namespace host_monitor
#endif // ENDPOINT_HPP_201706130847
