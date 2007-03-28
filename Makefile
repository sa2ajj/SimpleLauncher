GTKCFLAGS := $(shell pkg-config gtk+-2.0 --cflags)
GTKLIBS := $(shell pkg-config gtk+-2.0 --libs)

DBUSCFLAGS := $(shell pkg-config dbus-1 --cflags)
DBUSLIBS := $(shell pkg-config dbus-1 --libs)

CXXFLAGS=-Wall -g -MMD $(GTKCFLAGS) $(DBUSCFLAGS)
#LDFLAGS = -module -avoid-version
LDFLAGS = -shared
LIBS = -lstdc++

TARGET=simple-launcher.so
DESKTOP_FILE=simple-launcher.desktop
BACKUP_CONF=simple-launcher.backup

all: $(TARGET)

tests: test test1

$(TARGET): simple-launcher.o launchable-item.o launcher-item.o sla-list.o utils.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

test: test.o launcher-item.o
	g++ -o $@ $^ $(GTKLIBS) $(DBUSLIBS) $(LIBS) -losso

test1: test1.o sla-list.o launcher-item.o
	g++ -g -o $@ $^ $(GTKLIBS) $(DBUSLIBS) $(LIBS)

clean:
	rm -f *.d *.o $(TARGET) test test1

install: $(TARGET)
	install -d $(DESTDIR)/usr/share/applications/hildon-home
	install -m 0644 $(DESKTOP_FILE) $(DESTDIR)/usr/share/applications/hildon-home
	install -d $(DESTDIR)/usr/lib/hildon-home
	install -m 0644 $(TARGET) $(DESTDIR)/usr/lib/hildon-home
	install -d $(DESTDIR)/etc/osso-backup/applications
	install -m 0644 $(BACKUP_CONF) $(DESTDIR)/etc/osso-backup/applications/simple-launcher.conf

-include *.d
