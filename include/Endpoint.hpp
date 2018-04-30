/**
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file   Endpoint.hpp
 * @author Simon Brummer
 * @date   12.6.2017
 */

#ifndef ENDPOINT_HPP_201706130847
#define ENDPOINT_HPP_201706130847

#include <string>
#include <optional>
#include <cstdint>

namespace host_monitor
{
/**
 * @brief Endpoint structure used to hold connection parameters
 */
class Endpoint
{
public:
    enum class Protocol
    {
        ICMP = 0,
        TCP,
    };

    /**
     * @brief Build icmp endpoint.
     * @param[in] fqhn   The target that should be monitored. Either FQDN or IP-Address.
     * @returns Configured Endpoint
     */
    static auto make_icmp_endpoint(std::string fqhn) -> Endpoint;

    /**
     * @brief Function to generate an TCP Endpoint.
     * @throws std::runtime_error in case @p port is invalid.
     * @param[in] fqhn   The target that should be monitored. Either FQDN or IP-Address.
     * @param[in] port   Port number to connect to.
     * @returns Configured Endpoint
     */
    static auto make_tcp_endpoint( std::string fqhn
                                 , std::string port) -> Endpoint;

    /**
     * @brief Get endpoints target.
     * @returns string in the form "<fqhn>:<port>"
     */
    auto get_target() const -> std::string;

    /**
     * @brief Get endpoints fqhn.
     * @returns fqhn of the endpoint.
     */
    auto get_fqhn() const -> std::string;

    /**
     * @brief Get endpoints port.
     * @returns port if endpoint in case it exists.
     */
    auto get_port() const -> std::optional<std::string>;

    /**
     * @brief Get Protocol of Endpoint
     * @returns protocol of endpoint.
     */
    auto get_protocol() const -> Endpoint::Protocol;

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
auto protocol_to_string(Endpoint::Protocol p) -> std::string;

/**
 * @brief Convert String to Protocol enum.
 * @param[in] s   the string that should be parsed.
 * @returns @p as protocol. In case the given string can't be converted to a
 *          protocol, the optional is none.
 */
auto string_to_protocol(std::string const& s) -> std::optional<Endpoint::Protocol>;

} // namespace host_monitor
#endif // ENDPOINT_HPP_201706130847
