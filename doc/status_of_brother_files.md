Current status of Brother™ files
================================

Brother™ distributes drivers as Red Hat packages (`.rpm`) and Debian packages (`.deb`).
Debian packages are converted from rpm by Brother™ using [`alien`](http://kitenet.net/programs/alien/) tool.
Debian packages seems to contain more meta-information than Red Hat packages.

File organization is messy, some packages installs in `/opt`, others in `/usr/local`

For some printers, drivers are distributed as only one non-packaged gzipped ppd (`.ppd.gz`).

Oldest drivers date from 2005.

Some binaries have sources under GPLv2 licenses (on another page) and can be recompiled.
