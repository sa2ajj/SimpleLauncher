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

#include <gtk/gtknotebook.h>
#include <gtk/gtklabel.h>

#include "settings-dialog.h"
#include "settings-page-entries.h"

#define SL_APPLET_SETTINGS_ICON_SIZE  26

SettingsDialog::SettingsDialog(GtkWindow *parent, LauncherItems& items, GConfBooleanOption& transparent, GConfIntegerOption& icon_size):
  myTransparent(transparent, "Transparent background:"),
  myIconSize(icon_size, "Icon Size:", 26, 64) {
  myDialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Launcher Settings", parent, (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL));

  myNotebook = GTK_NOTEBOOK(gtk_notebook_new());

  gtk_container_add(GTK_CONTAINER(myDialog->vbox), GTK_WIDGET(myNotebook));

  SettingsPageWithEntries *uiPage = new SettingsPageWithEntries();
  uiPage->addEntry(&myTransparent);
  uiPage->addEntry(&myIconSize);

  SettingsPageWithItems *itemsPage = new SettingsPageWithItems(SL_APPLET_SETTINGS_ICON_SIZE, items);

  addPage("UI", uiPage);
  addPage("Items", itemsPage);

  gtk_widget_set_size_request(GTK_WIDGET(myDialog), 540, 324);
}

SettingsDialog::~SettingsDialog() {
  for (std::vector<SettingsPage *>::iterator it = myPages.begin(); it != myPages.end() ; ++it) {
    delete *it;
  }

  myPages.resize(0);

  gtk_widget_destroy(GTK_WIDGET(myDialog));
}

void SettingsDialog::addPage(const std::string& name, SettingsPage *page) {
  myPages.push_back(page);

  GtkWidget *label = gtk_label_new(name.c_str());

  gtk_notebook_append_page(myNotebook, page->getWidget(), label);
}

gint SettingsDialog::run() {
  gtk_widget_show_all(GTK_WIDGET(myDialog));
  gtk_notebook_set_current_page(myNotebook, 0);

  return gtk_dialog_run(myDialog);
}

void SettingsDialog::updateValues() {
  myTransparent.updateValue();
  myIconSize.updateValue();
}

// vim:ts=2:sw=2:et
