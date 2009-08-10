WXCONFIG = wx-config
CPP = `$(WXCONFIG) --cxx`
CXXFLAGS= `$(WXCONFIG) --cxxflags` -c -Os
LDFLAGS = `$(WXCONFIG) --libs`

SOURCES= src/DivFix++App.cpp src/DivFix++.cpp src/DivFix++Core.cpp src/DivFix++Gui.cpp
OBJECTS=$(SOURCES:.cpp=.o)
MOBJECTS=$(LANGUAGES:.po=.mo)
LANGUAGEDIRS=cs_CZ fa de es  fr  hu  it  ja  ko  ru  tr
LANGUAGES=$(wildcard locale/*/DivFix++.po)
EXECUTABLE=DivFix++

all: $(SOURCES) $(EXECUTABLE) langs

$(EXECUTABLE): $(OBJECTS)
	$(CPP) $(OBJECTS) $(LDFLAGS) -o $@

langs: $(MOBJECTS)

%.mo : %.po
	msgfmt $< -o $@

.cpp.o:
	$(CPP) $(CXXFLAGS) $< -o $@
install:
	mkdir -p $(DESTDIR)/usr/bin
	mkdir -p $(DESTDIR)/usr/share/pixmaps
	mkdir -p $(DESTDIR)/usr/share/applications
	cp DivFix++ $(DESTDIR)/usr/bin/DivFix++
	cp resources/DivFix++.png $(DESTDIR)/usr/share/pixmaps/DivFix++.png
	cp resources/DivFix++.desktop $(DESTDIR)/usr/share/applications/DivFix++.desktop
	@for i in $(LANGUAGEDIRS); do \
	   echo "mkdir -p $(DESTDIR)/usr/share/locale/$$i/LC_MESSAGES"; \
	   mkdir -p $(DESTDIR)/usr/share/locale/$$i/LC_MESSAGES; done;
	@for i in $(LANGUAGEDIRS); do \
	   echo "cp locale/$$i/DivFix++.mo $(DESTDIR)/usr/share/locale/$$i/LC_MESSAGES/DivFix++.mo"; \
	   cp locale/$$i/DivFix++.mo $(DESTDIR)/usr/share/locale/$$i/LC_MESSAGES/DivFix++.mo; done

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
	\t<string>DivFix++ v0.33</string>\n\
\
	\t<key>CFBundleIconFile</key>\n\
	\t<string>DivFix++.icns</string>\n\
\
  	\t<key>CFBundleIdentifier</key>\n\
 	\t<string>net.sourceforge.divfixpp</string>\n\
\
  	\t<key>CFBundleShortVersionString</key>\n\
 	\t<string>v0.33</string>\n\
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

uninstall:
	rm $(DESTDIR)/usr/bin/DivFix++
	rm $(DESTDIR)/usr/share/pixmaps/DivFix++.png
	rm $(DESTDIR)/usr/share/applications/DivFix++.desktop
	rm $(DESTDIR)/usr/share/locale/*/LC_MESSAGES/DivFix++.mo
test:
	cat $(LANGUAGEDIRS)

clean:
	rm -f src/*.o
	rm -f DivFix++
	rm -f locale/*/DivFix++.mo
	rm -rf DivFix++.app
