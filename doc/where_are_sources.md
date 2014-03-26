Where are sources?
==================

Cupswrapper and PPD sources
---------------------------

Theres is many places where there is sources:

Of course, there is sources on the [source code page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html), but for some models, [it's written](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html#plain%20text) that some drivers on the [driver page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html) are written in plain text format and are the sources codes.

By example, the cupswrapper for the [DCP-110C](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html#DCP-110C) is a C-Shell script (see [the “driver dependencies” page](driver_dependencies.md)).

Some printers (like the [HL-2460](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html#HL-2460) model) only have one PPD file as driver. That is source code.

So there is three archives formats for sources:

* `.ppd.gz`: gzipped plain text PPD;
* `.deb`: Debian archive, (mainly gzipped) tar archive inside an ar package;
* `.tar.gz`: gzipped tar archive.

So there is two places for printing driver sources:

* [source code page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html);
* [printer driver page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html).

Only cupswrapper and PPD files drivers are distributed as sources. It seems that all of thems are distributed under GPLv2 License.

LPR sources
-----------

There is no source for neither LPR driver.

Scanner sources
---------------

This is not studied now.

Fax-modem sources
-----------------

This is not studied now.
