Driver dependencies
===================

As read on the ~~[“Before the installation”](http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/before.html#005)~~ [(web archive)](http://web.archive.org/web/20140319075234/http://welcome.solutions.brother.com/bsc/public_s/id/linux/en/before.html) document, some printer driver require [csh](https://packages.debian.org/wheezy/csh) or [tcsh](https://packages.debian.org/wheezy/csh).

Related products: DCP-110C, DCP-310CN, FAX-1815C, FAX-1820C, FAX-1835C, FAX-1840C, FAX-1920CN, FAX-1940CN, FAX-2440C, MFC-210C, MFC-3220C, MFC-3240C, MFC-3320CN, MFC-3340CN, MFC-3420C, MFC-3820CN, MFC-410CN, MFC-420CN, MFC-5440CN, MFC-5840CN, MFC-620CN

To run i386 binary printer driver under amd64 architecture, Debian multiarch mecanism can be used:

```sh
luser@debian ~ $ sudo dpkg --add-architecture i386
luser@debian ~ $ sudo apt-get update
luser@debian ~ $ sudo apt-get install libc6:i386 libstdc++6:i386
```

See the “Before the installation” page for more requirements.
