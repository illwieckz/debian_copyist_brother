Summary: Brother MFC9880 CUPS wrapper driver
Name: cupswrapperMFC9880
Version: 1.0.2
Release: 1
URL: www.brother.com
License: Copyright Brother Industries,Ltd -2005
Group: Utilities/System 
Provides: CUPS wrapper driver

Requires: cups,  lpr,     
BuildRoot: %{_tmppath}/%{name}-root
source: %{name}-%{version}.tar.gz


%description
Brother MFC9880 CUPS wrapper driver 


%prep
#mkdir -p $RPM_BUILD_ROOT
%setup -q


%build

%install

mkdir -p $RPM_BUILD_ROOT/usr/local/Brother/cupswrapper


install -m 777 cupswrapperMFC9880-1.0.2 \
$RPM_BUILD_ROOT/usr/local/Brother/cupswrapper
install -m 777 brcupsconfig \
$RPM_BUILD_ROOT/usr/local/Brother/cupswrapper



%post
if [ -e /bin/sh ]; then
sh /usr/local/Brother/cupswrapper/cupswrapperMFC9880-1.0.2 -i
elif [ -e /bin/bash ]; then
bash /usr/local/Brother/cupswrapper/cupswrapperMFC9880-1.0.2 -i
else
echo ''
echo ' ****** ERROR: bash is required. ******'
fi


%preun
if [ -e /bin/sh ]; then
sh /usr/local/Brother/cupswrapper/cupswrapperMFC9880-1.0.2 -e
elif [ -e /bin/bash ]; then
bash /usr/local/Brother/cupswrapper/cupswrapperMFC9880-1.0.2 -e
fi

%postun
rm -f  $RPM_BUILD_ROOT/usr/local/Brother/cupswrapperMFC9880-1.0.2

%clean
rm -rf $RPM_BUILD_ROOT


%files
/usr/local/Brother/cupswrapper/cupswrapperMFC9880-1.0.2
/usr/local/Brother/cupswrapper
/usr/local/Brother
