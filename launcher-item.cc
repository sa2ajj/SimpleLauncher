#include <string>

#include <glib/gmem.h>
#include <glib/gkeyfile.h>

#include <gtk/gtkicontheme.h>

#include "launcher-item.h"

static const char *DESKTOP_ENTRY_GROUP = "Desktop Entry",
                  *DESKTOP_ENTRY_TYPE_FIELD = "Type",
                  *DESKTOP_ENTRY_ICON_FIELD = "Icon",
                  *DESKTOP_ENTRY_NAME_FIELD = "Name",
                  *DESKTOP_ENTRY_COMMENT_FIELD = "Comment",
                  *DESKTOP_ENTRY_SERVICE_FIELD = "X-Osso-Service";

inline std::string getStringWrapper(GKeyFile *keyFile, const gchar *group, const gchar *itemName) {
  gchar *tempo = g_key_file_get_string(keyFile, group, itemName, 0);
  std::string result;

  if (tempo != 0) {
    result = tempo;

    g_free(tempo);
  }

  return result;
}

inline std::string getLocaleStringWrapper(GKeyFile *keyFile, const gchar *group, const gchar *itemName) {
  gchar *tempo = g_key_file_get_locale_string(keyFile, group, itemName, 0, 0);
  std::string result;

  if (tempo != 0) {
    result = tempo;

    g_free(tempo);
  }

  return result;
}

LauncherItem::LauncherItem() {
}

LauncherItem::~LauncherItem() {
}

bool LauncherItem::load(const std::string& filename) {
  GKeyFile *key_file = 0;
  GError *error = 0;

  for (;;) {
    if ((key_file = g_key_file_new()) == 0) {
      break;
    }

    if (!g_key_file_load_from_file(key_file, filename.c_str(), G_KEY_FILE_NONE, &error)) {
      break;
    }

    if (getStringWrapper(key_file, DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_TYPE_FIELD) != "Application") {
      break;
    }

    myName = getStringWrapper(key_file, DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_NAME_FIELD);
    myComment = getLocaleStringWrapper(key_file, DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_COMMENT_FIELD);
    myIcon = getStringWrapper(key_file, DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_ICON_FIELD);
    myService = getStringWrapper(key_file, DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_SERVICE_FIELD);

    break;
  }

  if (error != 0) {
    g_error_free(error);
    error = 0;
  }

  if (key_file != 0) {
    g_key_file_free(key_file);
  }

  return !(myName.empty() || myIcon.empty() || myService.empty());
}

GdkPixbuf *LauncherItem::getIcon(int icon_size) const {
  GdkPixbuf *pixbuf = 0;

  if (!myIcon.empty()) {
    GtkIconTheme *theme;
    GError *error = 0;

    theme = gtk_icon_theme_get_default();
    pixbuf = gtk_icon_theme_load_icon(theme, myIcon.c_str(), icon_size, GTK_ICON_LOOKUP_NO_SVG, &error);

    if (error != 0) {
      g_error_free(error);
      error = NULL;
    }
  }

  return pixbuf;
}

bool LauncherItem::activate(osso_context_t *context) {
  return osso_application_top(context, myService.c_str(), 0) == OSSO_OK;
}
