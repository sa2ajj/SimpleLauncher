// This file is a part of Simple Launcher
//
// Copyright (C) 2006, 2007, Mikhail Sobolev
//
// Simple Launcher is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <string>

#include <libintl.h>

#include <glib/gmem.h>
#include <glib/gkeyfile.h>

#include "launcher-item.h"

GtkIconTheme *LauncherItem::ourTheme = NULL;

static const char *DESKTOP_ENTRY_GROUP = "Desktop Entry",
                  *DESKTOP_ENTRY_TYPE_FIELD = "Type",
                  *DESKTOP_ENTRY_ICON_FIELD = "Icon",
                  *DESKTOP_ENTRY_NAME_FIELD = "Name",
                  *DESKTOP_ENTRY_COMMENT_FIELD = "Comment",
                  *DESKTOP_ENTRY_SERVICE_FIELD = "X-Osso-Service",
                  *DESKTOP_ENTRY_TEXT_DOMAIN = "X-Text-Domain";

static const char *DEFAULT_TEXT_DOMAIN = "maemo-af-desktop";

class GKeyFileWrapper {
public:
  GKeyFileWrapper() {
    myKeyFile = g_key_file_new();
  }

 ~GKeyFileWrapper() {
    if (myKeyFile != NULL) {
      g_key_file_free(myKeyFile);
    }
  }

  bool load(const std::string& name) {
    GError *error = NULL;
    bool result = g_key_file_load_from_file(myKeyFile, name.c_str(), G_KEY_FILE_NONE, &error);

    if (error != NULL) {
      g_error_free(error);
    }

    return result;
  }

  std::string getString(const gchar *group, const gchar *itemName) {
    gchar *tempo = g_key_file_get_string(myKeyFile, group, itemName, NULL);
    std::string result;

    if (tempo != NULL) {
      result.assign(tempo);

      g_free(tempo);
    }

    return result;
  }

  std::string getLocaleString(const gchar *group, const gchar *itemName) {
    gchar *tempo = g_key_file_get_locale_string(myKeyFile, group, itemName, NULL, NULL);
    std::string result;

    if (tempo != NULL) {
      result.assign(tempo);

      g_free(tempo);
    }

    return result;
  }

private:
  GKeyFile *myKeyFile;
};

LauncherItem::LauncherItem(): myEnabled(false) {
}

LauncherItem::~LauncherItem() {
}

std::string LauncherItem::translateString(const std::string& what) const {
  if (what.empty()) {
    return what;
  } else {
    return dgettext(myTextDomain.empty() ? DEFAULT_TEXT_DOMAIN : myTextDomain.c_str(), what.c_str());
  }
}

bool LauncherItem::load(const std::string& filename) {
  GKeyFileWrapper key_file;

  for (;;) {
    myFileName = filename;

    if (!key_file.load(filename)) {
      break;
    }

    if (key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_TYPE_FIELD) != "Application") {
      break;
    }

    myName = key_file.getLocaleString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_NAME_FIELD);
    myComment = key_file.getLocaleString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_COMMENT_FIELD);
    myIcon = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_ICON_FIELD);
    myService = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_SERVICE_FIELD);
    myTextDomain = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_TEXT_DOMAIN);

    break;
  }

  return (myEnabled = checkSanity());
}

GdkPixbuf *LauncherItem::getIcon(int icon_size) const {
  GdkPixbuf *pixbuf = NULL;

  if (!myIcon.empty()) {
    if (ourTheme == NULL) {
      ourTheme = gtk_icon_theme_get_default();
    }

    GError *error = NULL;

    pixbuf = gtk_icon_theme_load_icon(ourTheme, myIcon.c_str(), icon_size, GTK_ICON_LOOKUP_NO_SVG, &error);

    if (error != NULL) {
      g_error_free(error);
      error = NULL;
    }
  }

  return pixbuf;
}

// vim:ts=2:sw=2:et
