GTKCFLAGS := $(shell pkg-config gtk+-2.0 --cflags)
GTKLIBS := $(shell pkg-config gtk+-2.0 --libs)

DBUSCFLAGS := $(shell pkg-config dbus-1 --cflags)
DBUSLIBS := $(shell pkg-config dbus-1 --libs)

CXXFLAGS=-Wall -MMD $(GTKCFLAGS) $(DBUSCFLAGS)
#LDFLAGS = -module -avoid-version
LDFLAGS = -shared
LIBS = -lstdc++

TARGET=simple-launcher.so

all: $(TARGET)

$(TARGET): simple-launcher.o launcher-item.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

test: test.o launcher-item.o
	g++ -o $@ $^ $(GTKLIBS) $(DBUSLIBS)  $(LIBS) -losso

clean:
	rm -f *.d *.o $(TARGET) test

install: $(TARGET)
	install -d $(DESTDIR)/usr/share/applications/hildon-home
	install -m 0644 simple-launcher.desktop $(DESTDIR)/usr/share/applications/hildon-home
	install -d $(DESTDIR)/usr/lib/hildon-home
	install -m 0644 $(TARGET) $(DESTDIR)/usr/lib/hildon-home

-include *.d
