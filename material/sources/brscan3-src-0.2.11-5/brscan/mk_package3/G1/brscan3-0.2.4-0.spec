Summary: Brother Scanner Driver
Name: brscan2
Version: 0.2.4
Release: 0
License: Copyright 2003 Brother Industries, Ltd. All Rights Reserved
Packager: root@bilidc8075
Vendor: Brother Industries, Ltd.
BuildRoot: /share/dev/Scan/brscan/mk_package/linux-2.6-intel/buildroot
Group: Applications
%define _topdir /share/dev/Scan/brscan/mk_package/linux-2.6-intel
Provides: brscan
%description
Brother sane backend Driver
%pre
%post
/usr/local/Brother/sane/setupSaneScan2 -i
%preun
/usr/local/Brother/sane/setupSaneScan2 -e
%postun
rmdir --ignore-fail-on-non-empty /usr/local/Brother/sane/GrayCmData/ALL
rmdir --ignore-fail-on-non-empty /usr/local/Brother/sane/GrayCmData/AL
rmdir --ignore-fail-on-non-empty /usr/local/Brother/sane/GrayCmData
rmdir --ignore-fail-on-non-empty /usr/local/Brother/sane
%files
%attr(0000,root,root) "/usr/bin/brsaneconfig2"
%attr(0000,root,root) "/usr/lib/libbrcolm2.so"
%attr(0000,root,root) "/usr/lib/libbrcolm2.so.1"
%attr(0755,root,root) "/usr/lib/libbrcolm2.so.1.0.0"
%attr(0000,root,root) "/usr/lib/libbrscandec2.so"
%attr(0000,root,root) "/usr/lib/libbrscandec2.so.1"
%attr(0755,root,root) "/usr/lib/libbrscandec2.so.1.0.0"
%attr(0000,root,root) "/usr/lib/sane/libsane-brother2.so"
%attr(0000,root,root) "/usr/lib/sane/libsane-brother2.so.1"
%attr(0755,root,root) "/usr/lib/sane/libsane-brother2.so.1.0.7"
%attr(0644,root,root) "/usr/local/Brother/sane/Brsane2.ini"
%attr(0644,root,root) "/usr/local/Brother/sane/GrayCmData/AL/brmsl09f.cm"
%attr(0644,root,root) "/usr/local/Brother/sane/GrayCmData/ALL/brmsl08f.cm"
%attr(0755,root,root) "/usr/local/Brother/sane/brsaneconfig2"
%attr(0666,root,root) "/usr/local/Brother/sane/brsanenetdevice2.cfg"
%attr(0755,root,root) "/usr/local/Brother/sane/setupSaneScan2"
