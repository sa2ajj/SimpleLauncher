GTKCFLAGS := $(shell pkg-config gtk+-2.0 --cflags)
GTKLIBS := $(shell pkg-config gtk+-2.0 --libs)

GCONFCFLAGS := $(shell pkg-config gconf-2.0 --cflags)
GCONFLIBS := $(shell pkg-config gconf-2.0 --libs)

DBUSCFLAGS := $(shell pkg-config dbus-1 --cflags)
DBUSLIBS := $(shell pkg-config dbus-1 --libs)

CXXFLAGS=-Wall -g -MMD $(GTKCFLAGS) $(DBUSCFLAGS) $(GCONFCFLAGS)
#LDFLAGS = -module -avoid-version
LDFLAGS = -shared
LIBS = -lstdc++

TARGET=simple-launcher.so
DESKTOP_FILE=simple-launcher.desktop
BACKUP_CONF=simple-launcher.backup

all: $(TARGET)

$(TARGET): simple-launcher.o launchable-item.o launcher-item.o settings-page-items.o utils.o settings-dialog.o gconf-wrapper.o dialog-entry.o settings-page-entries.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.d *.o $(TARGET)

install: $(TARGET)
	install -d $(DESTDIR)/usr/share/applications/hildon-home
	install -m 0644 $(DESKTOP_FILE) $(DESTDIR)/usr/share/applications/hildon-home
	install -d $(DESTDIR)/usr/lib/hildon-home
	install -m 0644 $(TARGET) $(DESTDIR)/usr/lib/hildon-home
	install -d $(DESTDIR)/etc/osso-backup/applications
	install -m 0644 $(BACKUP_CONF) $(DESTDIR)/etc/osso-backup/applications/simple-launcher.conf

pclean:
	fakeroot debian/rules clean

package:
	dpkg-buildpackage -rfakeroot -uc -b

-include *.d
