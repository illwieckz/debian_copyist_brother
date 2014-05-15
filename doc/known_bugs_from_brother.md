Known bugs from Brother™
========================

Drivers provided by Brother are plenty of bugs.

Useless quotes
--------------

97 cupswrapper have useless quotes like that line in ``cupswrapperdcp373cw``:

```sh
printer_model=""dcp373cw""
```

Echoed code
-----------

Cupswrappers writes ppd or filters with heredoc or echo, sometimes some lines of the cupswrapper (not the generated ppd or filter) are mistakenly echoed:

For example, you can see in ``cupswrapperMFC3820CN-1.0.0`` the ``echo 'rm -f /u…`` code, fortunately, another line was written to reenable this command:

```sh
set  brotherlpdwrapper=/usr/lib/cups/filter/brlpdwrapperMFC3820CN
rm -f  $brotherlpdwrapper
echo 'rm -f /usr/lib/cups/filter/brlpdwrapperMFC3820CN'
echo  '#! /bin/csh'  >>  $brotherlpdwrapper
echo  '#'  >>  $brotherlpdwrapper
```

Some cupswrappers was written in C Shell, another in Bourne Shell, but we can see Bourne Shell cupswrapper were ported from C Shell cupswrappers since this bug remains in some Bourn Shell cupswrappers like the ``cupswrappermfc990cw`` one:

```sh
rm -f  $brotherlpdwrapper
#echo 'rm -f /usr/lib/cups/filter/brlpdwrapper${printer_model}'

cat <<!ENDOFWFILTER! >$brotherlpdwrapper
#! /bin/sh
#
```

124 cupswrappers have this bug.

Useless code
------------

The ``cupswrappermfc5860cn`` has this useless structure:

```sh
printer_model="mfc660cn"
cat <<ENDOFPPDFILE1 >$ppd_file_name
…
FPPDFILE1

if [ $printer_model = "mfc5860cn" ]; then
cat <<ENDOFPPDFILE_2 >>$ppd_file_name
…
ENDOFPPDFILE_2

cat <<ENDOFPPDFILE_END >>$ppd_file_name
…
ENDOFPPDFILE_END
```

30 other cupswrappers do this test with another value of ``printer_model`` variable, so this code is never executed by this 30 other cupswrappers.

Bad shebang
-----------

The ``cupswrappermfc6890cdw`` cupswrapper writted in Bourne Shell has this strange shebang:

```sh
# !/usr/bin
```

A shee-space-bang with a directory as interpreter!

Reminiscence
------------

169 Bourne Shell cupswrappers has bits from previous C Shell like that (cupswrapperMFC9660-1.0.2):

```sh
cat <<!ENDOFWFILTER! >$brotherlpdwrapper
#! /bin/sh
#
# Brother Print filter  >>  $brotherlpdwrapper
```

or that (cupswrapperSetup_mfc9840cdw):

```sh
cat <<!ENDOFWFILTER! >$tmp_filter
#! /bin/sh
#
# Brother Print filter  >>
```

Note the trailing ``>>`` from a previous ``echo`` line, in some case the name of the generated file is printed inside the generated file.

Version discordance
-------------------

For example, the HL2240 printer driver install a file ``brcupsconfig4`` (``/usr/local/Brother/Printer/HL2240/cupswrapper/brcupsconfig4``), but the source package for this driver provide an source for the binary ``brcupsconfig3`` (``brhl2240cups_src-2.0.4-2/brcupsconfig3/brcupsconfig.c``). There is 8 different ``brcupsconfig`` binary files and 4 ``brcupsconfig4`` binary files inside the archives, so is can be more binary than sources.  The various ``brcupsconfig`` files are covered by GPLv2 license, if the source is not available, there may be a GPL violation, we don't know.

Bad archive
-----------

Many source tarballs (``tar.gz`` format) have "lone zero block" problems, perhaps some are broken.

Duplication
-----------

Drivers are mainly duplication of others, duplicating bugs or things that not have meaning. Perhaps 80% of code is redundant.
