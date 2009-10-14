# Copyright (c) 2006-2009 Erdem U. Altinyurt
# Thanks for oc2pus
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments to spamjunkeater gmail.com

# norootforbuild

%define _prefix	/usr

Name:			DivFix++
Summary:		A program to repair broken AVI file streams by rebuilding index part of file
Version:		0.34
Release:		1
License:		GPL
Group:			Productivity/Multimedia/Video/Editors and Convertors
URL:			http://divfixpp.sourceforge.net/
Source0:		%{name}_v%{version}-src.tar.bz2
BuildRoot:		%{_tmppath}/%{name}-%{version}-build
BuildRequires:	dos2unix
BuildRequires:	gcc-c++
BuildRequires:	pkgconfig
%if %{defined suse_version}
BuildRequires:	update-desktop-files
%endif
BuildRequires:	wxGTK-devel >= 2.8

%description
This program designed to repair broken AVI file streams by
rebuilding index part of file. This is very useful when trying
to preview movies which has no index part, like some files are
currently downloading from ed2k or bittorent networks.

DivFix++ has supports CLI tools, this means you can fix, preview
and delete movies automatically via script (by using argument
parameters...)

DivFix++ program code supports lots of operating system, because
it's writen by cross-platform API, wxWidgets.

%prep
%setup -q -n DivFix++_v%{version}

dos2unix     docs/*
%__chmod 644 docs/*

%build
%if %{defined suse_version} && 0%{?suse_version} < 1030
		%__make %{?jobs:-j%{jobs}} \
			WXCONFIG=wx-config-2.8
%else
	%__make %{?jobs:-j%{jobs}} \
		WXCONFIG=wx-config
%endif

%install
%__install -dm 755 %{buildroot}%{_datadir}/pixmaps
%__install -dm 755 %{buildroot}%{_datadir}/applications
%__install -dm 755 %{buildroot}%{_datadir}/locale
%__rm -f %{buildroot}%{_datadir}/applications/%{name}.desktop
%__install -D -s -m 755 DivFix++ %{buildroot}%{_bindir}/DivFix++
%__install -D -m 644 resources/DivFix++.png %{buildroot}%{_datadir}/pixmaps/DivFix++.png
%__rm -f locale/*/*.po
%__rm -f locale/*.pot
%__cp -rf locale/* %{buildroot}%{_datadir}/locale
%__cat > %{name}.desktop << EOF
[Desktop Entry]
Comment=A program to repair broken AVI file streams by rebuilding index part of file
Name=%{name}
GenericName=
Type=Application
Exec=%{name}
Icon=%{name}.png
Encoding=UTF-8
EOF
%if %{defined suse_version}
%suse_update_desktop_file -i %{name} AudioVideo AudioVideoEditing
%else
%__install -D -m 644 resources/DivFix++.desktop %{buildroot}%{_datadir}/applications/DivFix++.desktop
%endif

%find_lang %{name}

%clean
[ -d "%{buildroot}" -a "%{buildroot}" != "" ] && %__rm -rf "%{buildroot}"

%files -f %{name}.lang
%defattr(-,root,root)
%doc docs/*
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%changelog
* Wed Oct 14 2009 Erdem U. Altinyurt <spamjunkeater@gmail.com> - 0.34-0
- Make MultiOS compatible and update to 0.34
* Thu Aug 20 2009 Erdem U. Altinyurt <spamjunkeater@gmail.com> - 0.33-0
- Make MultiOS compatible and update to 0.33
* Wed Nov 29 2006 Toni Graffy <toni@links2linux.de> - 0.26-0.pm.1
- initial release of rpm
