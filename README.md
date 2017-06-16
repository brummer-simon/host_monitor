# host_monitor
Small C++ library to check periodically if a host is reachable over the network.

## Info
- Supported network protocols: ICMP and TCP. UDP is not supported (please write if you have an Idea how to support UDP)
- Depdencies: 'ping' and 'nc'. Linux: install 'nc', Windows: use cygwin.
