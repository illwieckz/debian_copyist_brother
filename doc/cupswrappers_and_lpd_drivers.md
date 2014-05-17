Cupswrappers and LPD drivers
============================

Most of drivers are LPD drivers with filters.

For each of these printers, a script called _cupswrapper_ write down a PPD and a filter to talk to the lpd driver. The cupswrappers, the PPDs and the filters are covered by the GPLv2 licence, but the lpd driver is closed source. Filters are writted with Bourne Shell or C Shell. For more information, see the [Generated PPDs and Filters](generated_ppds_and_filters.md) documentation page.

The LPD binaries have hardcoded paths (like ``/opt/brother`` or ``/usr/local/Brother``), so its impossible to package them following the Debian Guidelines.

