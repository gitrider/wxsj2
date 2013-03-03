####################################################################
# This SPEC file can be used to build source & binary RPMs through
# the "rpmbuild -ba thisfile.spec" command.
#
# NOTE: this file *must* be saved using Unix-style newlines (i.e. simple '\n');
#       if you edit this file with a win32 editor which uses the window-style
#       newlines (i.e. '\r\n') you'll get error when using rpmbuild.
#       Tip: use the "dos2unix" file to convert newlines in the right format.

Name:          wxdockit
Version:       2.1.0
Release:       1
Summary:       A docking library for wxWidgets
Provides:      %{name}

Source:        %{name}-%{version}.tar.gz
Prefix:        /usr/local
License:       wxWidgets
Group:         Development/Libraries
URL:           http://wxextended.sourceforge.net
Packager:      Francesco Montorsi (frm@users.sourceforge.net)
BuildRoot:     /var/tmp/%{name}-%{version}-root


%description
wxDockIt provides the necessary functions to support docking windows of different types:
floating dockwindows, embedded resizable panes, movable sliding toolbars.

%prep
%setup -n wxDockit

%build
./configure --prefix=$RPM_BUILD_ROOT/%{prefix}

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{prefix}/include
mkdir -p $RPM_BUILD_ROOT/%{prefix}/include/lib
make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/%{prefix}/include/wx
/%{prefix}/include/wx/generic
/%{prefix}/include/wx/gtk
/%{prefix}/lib/*%{name}*
