CC=g++
CFLAGS=-c -g -Wall `sdl-config --cflags` 
LDFLAGS= -lSGE -lSDL_image -lSDL_gfx -lSDL_ttf -ljpeg -lpng -lz -lm `sdl-config --libs` -lSDL_mixer -lunrar
SOURCES=main.cc label.cc container.cc guielement.cc stackpanel.cc listbox.cc combobox.cc listboxcombo.cc window.cc guistart.cc guiimages.cc guisettings.cc filebrowser.cc fileobject.cc listboxfilebrowser.cc imageloader.cc stuff.cc config.cc imageviewer.cc guibookmarks.cc listboxcombobookmarks.cc guiosdimage.cc labeldesigned.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=engine-pc

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o hello
