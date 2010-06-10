WXCONFIG = wx-config
CPP = `$(WXCONFIG) --cxx`
CXXFLAGS= `$(WXCONFIG) --cxxflags` -02
LDFLAGS = `$(WXCONFIG) --libs`
RC = `$(WXCONFIG) --rescomp`
#RC = x86_64-w64-mingw32-windres --define WX_CPU_AMD64
RCFLAGS = `$(WXCONFIG) --cxxflags`
MSGFMT = msgfmt

SOURCES= src/DivFix++App.cpp\
			src/DivFix++.cpp\
			src/DivFix+2Core.cpp\
			src/DivFix++Gui.cpp
RESOURCES= resources/resource.rc
RESOURCE_OBJ=$(RESOURCES:.rc=.o)
OBJECTS=$(SOURCES:.cpp=.o)
MOBJECTS=$(LANGUAGES:.po=.mo)

LANGUAGEDIRS=cs_CZ fa de es fr hu it ja ko ru tr uk
LANGUAGES=$(wildcard locale/*/DivFix++.po)
EXECUTABLE=DivFix++
EXECUTABLE_WIN=DivFix++.exe

DESTDIR		=
PREFIX		= $(DESTDIR)/usr
BINDIR	    = $(PREFIX)/bin
DATADIR	    = $(PREFIX)/share
LOCALEDIR   = $(DATADIR)/locale

all: $(SOURCES) $(EXECUTABLE) langs

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(OBJECTS) $(LDFLAGS) -o $@

win: $(SOURCES) $(RESOURCES) $(EXECUTABLE_WIN) langs

$(EXECUTABLE_WIN): $(OBJECTS) $(RESOURCE_OBJ)
	$(CPP) $(OBJECTS) $(RESOURCE_OBJ) $(LDFLAGS) -static-libgcc -o $@

langs: $(MOBJECTS)

%.mo : %.po
	$(MSGFMT) $< -o $@

%.o : %.rc
	$(RC) $(RCFLAGS) $(CXXFLAGS)  $< -o $@

.cpp.o:
	$(CPP) $(CXXFLAGS) -c $< -o $@

install:
	install -D -m 755 DivFix++ $(BINDIR)/$(EXECUTABLE)
	install -D -m 644 resources/DivFix++.png $(DATADIR)/pixmaps/DivFix++.png
	install -D -m 644 resources/DivFix++.desktop $(DATADIR)/applications/DivFix++.desktop
	@for i in $(LANGUAGEDIRS); do \
	   echo "install -D -m 644 locale/$$i/DivFix++.mo $(LOCALEDIR)/$$i/LC_MESSAGES/DivFix++.mo"; \
	   install -D -m 644 locale/$$i/DivFix++.mo $(LOCALEDIR)/$$i/LC_MESSAGES/DivFix++.mo; done

uninstall:
	rm $(BINDIR)/$(EXECUTABLE)
	rm $(DATADIR)/pixmaps/DivFix++.png
	rm $(DATADIR)/applications/DivFix++.desktop
	rm $(LOCALEDIR)/*/LC_MESSAGES/DivFix++.mo

test:
	cat $(LANGUAGEDIRS)

clean:
	rm -f src/*.o
	rm -f resources/resource.o
	rm -f locale/*/DivFix++.mo
	rm -f DivFix++
	rm -f DivFix++.exe
	rm -rf DivFix++.app

distclean: clean

mac: all
	mkdir -p DivFix++.app/Contents
	mkdir -p DivFix++.app/Contents/MacOS
	mkdir -p DivFix++.app/Contents/Resources
	mv DivFix++ DivFix++.app/Contents/MacOS/
	cp resources/DivFix++.icns DivFix++.app/Contents/Resources/
	cp docs/* DivFix++.app/Contents/Resources/
	echo "APPLDivF" > DivFix++.app/Contents/PkgInfo
	echo "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n\
<plist version=\"1.0\">\n\
<dict>\n\
 	\t<key>CFBundleDevelopmentRegion</key>\n\
 	\t<string>English</string>\n\
\
  	\t<key>CFBundleExecutable</key>\n\
 	\t<string>DivFix++</string>\n\
\
	\t<key>CFBundleGetInfoString</key>\n\
	\t<string>DivFix++ v0.34</string>\n\
\
	\t<key>CFBundleIconFile</key>\n\
	\t<string>DivFix++.icns</string>\n\
\
  	\t<key>CFBundleIdentifier</key>\n\
 	\t<string>net.sourceforge.divfixpp</string>\n\
\
  	\t<key>CFBundleShortVersionString</key>\n\
 	\t<string>v0.34</string>\n\
\
  	\t<key>CFBundleInfoDictionaryVersion</key>\n\
 	\t<string>6.0</string>\n\
\
  	\t<key>CFBundleName</key>\n\
 	\t<string>DivFix++</string>\n\
\
  	\t<key>CFBundlePackageType</key>\n\
 	\t<string>APPL</string>\n\
\
  	\t<key>CFBundleSignature</key>\n\
 	\t<string>DivF</string>\n\
\
  	\t<key>CFBundleVersion</key>\n\
 	\t<string>1.0.0</string>\n\
\
	\t<key>DRURLs</key>\n\
	\t<string>http://divfixpp.sourceforge.net</string>\n\
\
	\t<key>NSMainNibFile</key>\n\
	\t<string>DivFix++</string>\n\
\
	\t<key>NSPrincipalClass</key>\n\
	\t<string>NSApplication</string>\n\
\
  	\t<key>NSHumanReadableCopyright</key>\n\
 	\t<string> (c) 2006-2009, Erdem U. Altinyurt</string>\n\
\
</dict>\n\
</plist>\n\n" > DivFix++.app/Contents/Info.plist

	@for i in $(LANGUAGEDIRS); do \
		echo "mkdir -p DivFix++.app/Contents/Resources/locale/ru"; \
		mkdir -p DivFix++.app/Contents/Resources/locale/$$i; done
	@for i in $(LANGUAGEDIRS); do \
		echo "cp locale/$$i/DivFix++.mo DivFix++.app/Contents/Resources/locale/$$i/"; \
		cp locale/$$i/DivFix++.mo DivFix++.app/Contents/Resources/locale/$$i/; done
