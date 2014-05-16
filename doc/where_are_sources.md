Where are sources?
==================

Cupswrapper and PPD sources
---------------------------

Theres is many places where we can find sources:

Of course, there is sources on the ~~[source code page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html), but for some models, ~~[it's written](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html#plain%20text)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html#plain%20text) that some drivers on the ~~[driver page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html) are written in plain text format and are the sources codes.

For example, the cupswrapper for the ~~[DCP-110C](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html#DCP-110C)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html#DCP-110C) is a C-Shell script (see [the “driver dependencies” page](driver_dependencies.md)).

Some printers (like the ~~[HL-2460](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html#HL-2460)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html#HL-2460) model) only have one PPD file as driver. That is source code.

So sources are distributed whith three archive formats:

* `.ppd.gz`: gzipped plain text PPD;
* `.deb`: Debian archive, (mainly gzipped) tar archive inside an ar package;
* `.tar.gz`: gzipped tar archive.

So there is two places for printing driver sources:

* ~~[source code page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_src.html);
* ~~[printer driver page](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html)~~ [(web archive)](http://web.archive.org/web/20140319075144/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/download_prn.html).

Only cupswrapper and PPD drivers are distributed as sources. It seems that all of thems are distributed under GPLv2 License.

Many PPD are not distributed directly but embedded in Shell or C-Chell scripts from cupswrapper package, sometimes as heredocs, sometimes as a long list of `echo` commands.

Example:

Abstract from ``cupswrapperDCP110C-1.0.2`` C-Shell script:

```sh
set  ppd_file_name=/usr/share/cups/model/brdcp110c_cups.ppd

touch $ppd_file_name

echo '*PPD-Adobe: "4.3"'                                                                >>$ppd_file_name
echo ''                                                                                 >>$ppd_file_name
echo '*%================================================'                               >>$ppd_file_name
echo '*%	Copyright(C) 2004 Brother Industries, Ltd.'                                    >>$ppd_file_name
echo '*%	"Brother DCP-110C CUPS"'                                                       >>$ppd_file_name
echo '*%================================================ '                              >>$ppd_file_name
echo ''                                                                                 >>$ppd_file_name
echo '*%==== General Information Keywords ========================'                     >>$ppd_file_name
echo '*FormatVersion: "4.3"'                                                            >>$ppd_file_name
echo '*FileVersion: "1.01"'                                                             >>$ppd_file_name
echo '*LanguageVersion: English'                                                        >>$ppd_file_name
echo '*LanguageEncoding: ISOLatin1'                                                     >>$ppd_file_name
echo '*PCFileName: "DCP110C.PPD"'                                                       >>$ppd_file_name
echo '*Manufacturer: "Brother"'                                                         >>$ppd_file_name
echo '*Product: "(DCP-110C)"'                                                           >>$ppd_file_name
echo '*1284DeviceID: "MFG:Brother;MDL:DCP-110C"'                                        >>$ppd_file_name
echo '*cupsVersion: 1.1'                                                                >>$ppd_file_name
echo '*cupsManualCopies: False'                                                         >>$ppd_file_name
echo '*cupsFilter: "application/vnd.cups-postscript 0 brlpdwrapperDCP110C"'             >>$ppd_file_name
echo '*cupsModelNumber: 6'                                                              >>$ppd_file_name
echo '*ModelName: "Brother DCP-110C"'                                                   >>$ppd_file_name
echo '*ShortNickName: "Brother DCP-110C"'                                               >>$ppd_file_name
echo '*NickName: "Brother DCP-110C CUPS v1.1"'                                          >>$ppd_file_name
echo '*PSVersion: "(3010.106) 3"'                                                       >>$ppd_file_name
[…]
```

Abstract from `cupswrapperHL2030-2.0.1` Shell script:

```sh
ppd_file_name=/usr/share/cups/model/HL2030.ppd

cat <<ENDOFPPDFILE >$ppd_file_name
*PPD-Adobe: "4.3"
*%================================================
*%	Copyright(C) 2006 Brother Industries, Ltd.
*%	"Brother HL2030 for CUPS"
*%================================================

*%==== General Information Keywords ========================
*FormatVersion: "4.3"
*FileVersion: "1.00"
*LanguageEncoding: ISOLatin1
*LanguageVersion: English
*Manufacturer: "Brother"
*PCFileName: "HL2030.PPD"
*Product: "(Brother HL2030 series)"
*cupsVersion:   1.1
*cupsManualCopies: True
*cupsModelNumber:       68
*cupsFilter: "application/vnd.cups-postscript 0 brlpdwrapperHL2030"
*PSVersion: "(3010.106) 3"
*ModelName: "HL2030"
*NickName: "Brother HL2030 for CUPS"
*ShortNickName: "Brother HL2030 for CUPS"
[…]
ENDOFPPDFILE
```

Sometime thoses scripts write scripts, For example both `cupswrapperDCP110C-1.0.2` and `cupswrapperHL2030-2.0.1` do that.

LPR sources
-----------

There is no source for neither LPR driver.

Scanner sources
---------------

This is not studied now.

Fax-modem sources
-----------------

This is not studied now.
