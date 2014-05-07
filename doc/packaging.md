Prerequist
===========

We are using brother-driver as example for package name.

Software
* dh-make

Goals
=====

* Choose the best folder tree
* Choose between hand building or auto building packaging
* Choose how to version packaging
* Have a great package under the Debian rules

Structure proposale
===================

```sh
/brother-driver-1.0
--/DEBIAN/
----control
----copyright
----changelog
----rules
----preinst
----postinst
----prerm
----postrm
--/usr/share/cups/model/*.ppd
```

/DEBIAN/Control
---------------

```sh
Package: brother-driver
Version: 1.0 (to choose)
Section: unknown (to define)
Priority: optional
Architecture: all
Depends: [to define]
Maintainer: Didier 'OdyX' Raboud <odyx@debian.org>, Thomas DEBESSE <dev@illwieckz.net>, Samuel Dorsaz <samuel@micheldorsaz.ch>
Description: Drivers for Brother printers
Homepage: https://github.com/illwieckz/debian_copyist_brother
```

Naming convention
=================

[2.6. Package name and version](https://www.debian.org/doc/manuals/maint-guide/first.en.html#namever)

Sources
=======

[Openclassrooms - Short tutorial](http://fr.openclassrooms.com/informatique/cours/creer-un-paquet-deb)
[Chapter 4. Required files under the debian directory](https://www.debian.org/doc/manuals/maint-guide/dreq.en.html#control)

Documentation to read
---------------------

[Debian New Maintainers' Guide](https://www.debian.org/doc/manuals/maint-guide/)
[Debian New Maintainers' Guide](http://www.cs.rug.nl/~jurjen/DebPackaging/index.html)
