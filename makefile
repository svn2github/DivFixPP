WXCONFIG = wx-config
CPP = `$(WXCONFIG) --cxx`
CXXFLAGS= `$(WXCONFIG) --cxxflags` -c -Os
LDFLAGS = `$(WXCONFIG) --libs`

SOURCES= src/DivFix++App.cpp src/DivFix++.cpp src/DivFix++Core.cpp src/DivFix++Gui.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=DivFix++

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CPP) $(OBJECTS) $(LDFLAGS) -o $@
	msgfmt locale/tr/DivFix++.po -o locale/tr/DivFix++.mo
	msgfmt locale/hu/DivFix++.po -o locale/hu/DivFix++.mo
	msgfmt locale/ja/DivFix++.po -o locale/ja/DivFix++.mo
	msgfmt locale/ko/DivFix++.po -o locale/ko/DivFix++.mo
	msgfmt locale/it/DivFix++.po -o locale/it/DivFix++.mo
	msgfmt locale/cs_CZ/DivFix++.po -o locale/cs_CZ/DivFix++.mo

.cpp.o:
	$(CPP) $(CXXFLAGS) $< -o $@
install:
	mkdir -p $(DESTDIR)/usr/bin
	mkdir -p $(DESTDIR)/usr/share/locale/tr/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/hu/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/ja/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/ko/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/it/LC_MESSAGES
	mkdir -p $(DESTDIR)/usr/share/locale/cs_CZ/LC_MESSAGES
	cp DivFix++ $(DESTDIR)/usr/bin
	cp locale/tr/DivFix++.mo $(DESTDIR)/usr/share/locale/tr/LC_MESSAGES/DivFix++.mo
	cp locale/hu/DivFix++.mo $(DESTDIR)/usr/share/locale/hu/LC_MESSAGES/DivFix++.mo
	cp locale/ja/DivFix++.mo $(DESTDIR)/usr/share/locale/ja/LC_MESSAGES/DivFix++.mo
	cp locale/ko/DivFix++.mo $(DESTDIR)/usr/share/locale/ko/LC_MESSAGES/DivFix++.mo
	cp locale/it/DivFix++.mo $(DESTDIR)/usr/share/locale/it/LC_MESSAGES/DivFix++.mo
	cp locale/cs_CZ/DivFix++.mo $(DESTDIR)/usr/share/locale/cs_CZ/LC_MESSAGES/DivFix++.mo
uninstall:
	rm /usr/local/bin/DivFix++
	rm /usr/local/share/locale/tr/LC_MESSAGES/DivFix++.mo
	rm /usr/local/share/locale/hu/LC_MESSAGES/DivFix++.mo  
	rm /usr/local/share/locale/ja/LC_MESSAGES/DivFix++.mo
	rm /usr/local/share/locale/ko/LC_MESSAGES/DivFix++.mo
	rm /usr/local/share/locale/it/LC_MESSAGES/DivFix++.mo
	rm /usr/local/share/locale/cs_CZ/LC_MESSAGES/DivFix++.mo
clean: 
	rm src/*.o
	rm DivFix++
	rm locale/tr/DivFix++.mo
	rm locale/hu/DivFix++.mo
	rm locale/ja/DivFix++.mo
	rm locale/ko/DivFix++.mo
	rm locale/it/DivFix++.mo
	rm locale/cs_CZ/DivFix++.mo
