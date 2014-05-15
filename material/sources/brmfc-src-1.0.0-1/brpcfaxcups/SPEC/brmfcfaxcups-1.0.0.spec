Summary: Brother MFC/FAX fax share function driver
Name:    brmfcfaxcups
Version: 1.0.0
Release: 1
URL: www.brother.com
License: Copyright Brother Industries,Ltd -2005
Group: Utilities/System 
Provides: Fax share driver
Requires: ghostscript,cups

BuildRoot: %{_tmppath}/%{name}-root
source: %{name}-%{version}.tar.gz


%description
Brother MFC/FAX fax share function driver



%prep
#mkdir -p $RPM_BUILD_ROOT
%setup -q


%build

%install

mkdir -p $RPM_BUILD_ROOT/usr/local/Brother/fax
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/lib/cups/filter
mkdir -p $RPM_BUILD_ROOT/usr/lib64/cups/filter
mkdir -p $RPM_BUILD_ROOT/usr/share/cups/model
mkdir -p $RPM_BUILD_ROOT/usr/local/Brother/fax
mkdir -p $RPM_BUILD_ROOT/usr/local/Brother/fax
mkdir -p $RPM_BUILD_ROOT/usr/bin




install -m 777 brfaxfilter $RPM_BUILD_ROOT/usr/lib/cups/filter
install -m 777 brfax_cups.ppd $RPM_BUILD_ROOT/usr/share/cups/model
install -m 777 brmfcfax.jar $RPM_BUILD_ROOT/usr/local/Brother/fax
install -m 777 brpcfax $RPM_BUILD_ROOT/usr/bin

%post
lpadmin -p BRFAX -E -v usb:/dev/usb/lp0 -m brfax_cups.ppd
if [ -e /usr/lib64/cups/filter ];then
  cp /usr/lib/cups/filter/brfaxfilter /usr/lib64/cups/filter
fi
if [ -e /etc/init.d/cups ]; then
     /etc/init.d/cups restart
elif [ -e /etc/init.d/cupsys ]; then
     /etc/init.d/cupsys restart
fi


%preun
lpadmin -x BRFAX
if [ -e /usr/lib64/cups/filter/brfaxfilter ];then
     rm -f /usr/lib64/cups/filter/brfaxfilter
fi


%postun
if [ -e /etc/init.d/cups ]; then
     /etc/init.d/cups restart
elif [ -e /etc/init.d/cupsys ]; then
     /etc/init.d/cupsys restart
fi
%clean
rm -rf $RPM_BUILD_ROOT


%files
/usr/lib/cups/filter/brfaxfilter
/usr/share/cups/model/brfax_cups.ppd
/usr/local/Brother/fax/brmfcfax.jar
/usr/local/Brother/fax
/usr/local/Brother
/usr/bin//brpcfax
