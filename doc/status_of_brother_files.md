Current status of Brother™ files
================================

Brother™ distributes drivers as Red Hat packages (`.rpm`) and Debian packages (`.deb`). Debian packages are converted from Red Hat packages by Brother™ using [`alien`](http://kitenet.net/programs/alien/) tool.

Debian packages seems to contain more meta-information than Red Hat packages (the `DEBIAN/control` file).

File organization is messy, some packages installs in `/opt`, others in `/usr/local`. Those path are hardcoded inside binaries that do not have source.

For some printers, drivers are distributed as only one non-packaged gzipped ppd (`.ppd.gz`).

Oldest drivers date from 2005.

Some binaries have sources under GPLv2 licenses (on [another page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html)) and can be recompiled.
