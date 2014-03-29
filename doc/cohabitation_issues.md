Cohabitation issues
===================

On the [“Before the installation” page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/before.html#evlenv), we can read:

> Connecting more than one machine with the same model number is not supported.

This is not supported, but is this functionnal neverthless?

There is 391 printer models supported on the [Printer binary driver download page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html), is various drivers of different models can be installed simultaneously?

For information, for both printers, scanners and pcfaxes product, there is 765 Debian-compatible drivers package (699 `.deb`, 66 lonely `.ppd`).

Cohabitation
------------

* Can we install one driver next to the other drivers without risk?

Some drivers can’t be installed alongside others. For example, the `hl4150cdnlpr-1.1.1-5.i386.deb` driver run a script at `postinst` time named `/usr/local/Brother/Printer/hl4150cdn/inf/setupPrintcapij` that writes the printer name `hl4150cdn` into a file named `/etc/printcap`. If another driver do the same, that means that we can’t use two different printer models with this kind of driver.

Also this driver assume that the Brother printer is known as `/dev/usb/lp0`. If there is more than one USB printer plugged on the same computer and the Brother one is not the first USB printer device, the driver will not work.

Some other driver use named path, for example `cupswrapperMFC7820N-2.0.1` assume that the USB device is `usb://Brother/MFC-7820N` using `usb:/dev/usb/lp0` only as a fallbacck.

Mutualisation
-------------

* Is some driver package share same binary (like `rawtobr3` or `braddprinter`) with different (newer) and probably incompatible versions?
