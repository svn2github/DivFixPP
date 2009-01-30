WXCONFIG = wx-config
CPP = `$(WXCONFIG) --cxx`
CXXFLAGS= `$(WXCONFIG) --cxxflags` -c -Os
LDFLAGS = `$(WXCONFIG) --libs`

SOURCES= src/DivFix++App.cpp src/DivFix++.cpp src/DivFix++Core.cpp src/DivFix++Gui.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LANGUAGES=  locale/tr/DivFix++.po \
			locale/hu/DivFix++.po\
			locale/ja/DivFix++.po\
			locale/cs_CZ/DivFix++.po\
			locale/ko/DivFix++.po\
			locale/it/DivFix++.po\
			locale/fr/DivFix++.po\
			locale/de/DivFix++.po\
			locale/es/DivFix++.po

MOBJECTS=$(LANGUAGES:.po=.mo)
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
	mkdir -p $(DESTDIR)/usr/share/locale/tr/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/hu/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/ja/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/cs_CZ/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/ko/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/it/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/fr/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/de/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/es/LC_MESSAGES
	cp DivFix++ $(DESTDIR)/usr/bin/DivFix++
	cp resources/DivFix++.png $(DESTDIR)/usr/share/pixmaps/DivFix++.png
	cp resources/DivFix++.desktop $(DESTDIR)/usr/share/applications/DivFix++.desktop
	cp locale/tr/DivFix++.mo $(DESTDIR)/usr/share/locale/tr/LC_MESSAGES/DivFix++.mo
	cp locale/hu/DivFix++.mo $(DESTDIR)/usr/share/locale/hu/LC_MESSAGES/DivFix++.mo
	cp locale/ja/DivFix++.mo $(DESTDIR)/usr/share/locale/ja/LC_MESSAGES/DivFix++.mo
	cp locale/cs_CZ/DivFix++.mo $(DESTDIR)/usr/share/locale/cs_CZ/LC_MESSAGES/DivFix++.mo
	cp locale/ko/DivFix++.mo $(DESTDIR)/usr/share/locale/ko/LC_MESSAGES/DivFix++.mo
	cp locale/it/DivFix++.mo $(DESTDIR)/usr/share/locale/it/LC_MESSAGES/DivFix++.mo
	cp locale/fr/DivFix++.mo $(DESTDIR)/usr/share/locale/fr/LC_MESSAGES/DivFix++.mo
	cp locale/de/DivFix++.mo $(DESTDIR)/usr/share/locale/de/LC_MESSAGES/DivFix++.mo
	cp locale/es/DivFix++.mo $(DESTDIR)/usr/share/locale/es/LC_MESSAGES/DivFix++.mo

uninstall:
	rm $(DESTDIR)/usr/bin/DivFix++
	rm $(DESTDIR)/usr/share/pixmaps/DivFix++.png
	rm $(DESTDIR)/usr/share/applications/DivFix++.desktop
	rm $(DESTDIR)/usr/share/locale/tr/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/hu/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/ja/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/cs_CZ/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/ko/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/it/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/fr/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/de/LC_MESSAGES/DivFix++.mo
	rm $(DESTDIR)/usr/share/locale/es/LC_MESSAGES/DivFix++.mo
clean:
	rm -f src/*.o
	rm -f DivFix++
	rm -f $(MOBJECTS)
