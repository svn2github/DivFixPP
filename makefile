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
	msgfmt locale/cs_CZ/DivFix++.po -o locale/cs_CZ/DivFix++.mo

.cpp.o:
	$(CPP) $(CXXFLAGS) $< -o $@
install:
	cp DivFix++ /usr/local/bin
	cp locale/tr/DivFix++.mo /usr/local/share/locale/tr/LC_MESSAGES/DivFix++.mo
	cp locale/hu/DivFix++.mo /usr/local/share/locale/hu/LC_MESSAGES/DivFix++.mo
	cp locale/ja/DivFix++.mo /usr/local/share/locale/ja/LC_MESSAGES/DivFix++.mo
	cp locale/cs_CZ/DivFix++.mo /usr/local/share/locale/cs_CZ/LC_MESSAGES/DivFix++.mo

uninstall:
	rm /usr/local/bin/DivFix++
	rm /usr/local/share/locale/tr/LC_MESSAGES/DivFix++.mo
	rm /usr/local/share/locale/hu/LC_MESSAGES/DivFix++.mo  
	rm /usr/local/share/locale/ja/LC_MESSAGES/DivFix++.mo
	rm /usr/local/share/locale/cs_CZ/LC_MESSAGES/DivFix++.mo
clean: 
	rm src/*.o
	rm DivFix++
	rm locale/tr/DivFix++.mo
	rm locale/hu/DivFix++.mo
	rm locale/ja/DivFix++.mo
	rm locale/cs_CZ/DivFix++.mo
