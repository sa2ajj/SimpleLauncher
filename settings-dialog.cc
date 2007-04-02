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

#include "settings-dialog.h"

SettingsDialog::SettingsDialog(GtkWindow *parent, int size, LauncherItems& items) : myList(size, items) {
  myDialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Launcher Settings", parent, (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL));

  gtk_container_add(GTK_CONTAINER(myDialog->vbox), myList.getWidget());

  gtk_widget_set_size_request(GTK_WIDGET(myDialog), 540, 257);
}

SettingsDialog::~SettingsDialog() {
  gtk_widget_destroy(GTK_WIDGET(myDialog));
}

gint SettingsDialog::run() {
  return gtk_dialog_run(myDialog);
}
