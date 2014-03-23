Brother legal
=============

Please document here Brother™ legal stuff.

Brother™ distributes files under 4 licenses :

* [English_gpl](abstract/English_gpl.txt) ([original](http://www.brother.com/agreement/English_gpl/agree.html)) aka [GPLv2](http://www.gnu.org/licenses/gpl-2.0.html);
* [English_lpr](abstract/English_lpr.txt) ([original](http://www.brother.com/agreement/English_lpr/agree.html));
* [English_sane](abstract/English_sane.txt) ([Original](http://www.brother.com/agreement/English_sane/agree.html));
* [English_source](abstract/English_source.txt) ([Original](http://www.brother.com/agreement/English_source/agree.html)).

Binary printer drivers are covered by only two licenses. On the [Linux printer driver download page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html), LPR drivers are distributed under “English_lpr” license, and cupswrapper and ppd drivers are distributed under _GPLv2_ License. We can find GPLv2 sources for binaries on the [source download page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html).

Unfortunately, it does not seems to be any public source for printers LPR binary drivers distributed under the “English_lpr” license. But this license says:

> Brother grants User a non-exclusive license: to reproduce and/or distribute (via Internet or in any other manner) the Software. Further, Brother grants User a non-exclusive license to modify, alter, translate or otherwise prepare derivative works of the Software and to reproduce and distribute (via Internet or in any other manner) such modification, alteration, translation or other derivative works for any purpose.

So,

* Debian can legaly repackage and redistribute any printer driver binary covered by the “English_lpr” license.
* Debian can repackage and redistribute any printer driver binary covered byt the “GPLv2” license. If source code is available on the source download page, Debian can recompile it.

Note: The “[English_source](http://www.brother.com/agreement/English_source/agree.html)” license seem more conflictual, but no one printer binary driver is covered by this license.

Note: The [abstract](abstract/) directory contain plain-text copy of licenses used by Brother™, and links when original on-line HTML licenses files on officiel Brother™ website.
