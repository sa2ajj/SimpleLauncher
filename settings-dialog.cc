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

#include <gtk/gtkvbox.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtksizegroup.h>
#include <gtk/gtknotebook.h>
#include <gtk/gtklabel.h>

#include "settings-dialog.h"

// FIXME: UGLY!!!!

inline void addPage(GtkNotebook *notebook, const std::string& name, GtkWidget *widget) {
  GtkWidget *label = gtk_label_new(name.c_str());

  gtk_notebook_append_page(notebook, widget, label);
}

inline GtkWidget *packItTogether(GtkSizeGroup *group, const std::string& name, GtkWidget *content) {
  GtkWidget *box = gtk_hbox_new(false, 0);
  GtkWidget *label = gtk_label_new(name.c_str());

  gtk_size_group_add_widget(group, label);
  gtk_box_pack_start(GTK_BOX(box), label, true, true, 0);
  gtk_box_pack_start(GTK_BOX(box), content, true, true, 0);

  return box;
}

inline GtkWidget *createUIPage() {
  GtkSizeGroup *group = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
  GtkWidget *vbox = gtk_vbox_new(true, 0);

  // packItTogether(group, "Button Size:", <small/big>);
  // packItTogether(group, "Transparent background:", [ ]);
  // packItTogether(group, "Show Infobanner:", [ ]);

  g_object_unref(G_OBJECT(group));

  return vbox;
}

SettingsDialog::SettingsDialog(GtkWindow *parent, int size, LauncherItems& items) : myList(size, items) {
  myDialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Launcher Settings", parent, (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), "OK", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL));

  GtkNotebook *notebook = GTK_NOTEBOOK(gtk_notebook_new());

  gtk_container_add(GTK_CONTAINER(myDialog->vbox), GTK_WIDGET(notebook));

  // addPage(notebook, "UI", createUIPage());
  addPage(notebook, "Items", myList.getWidget());

  gtk_widget_set_size_request(GTK_WIDGET(myDialog), 540, 257);
}

SettingsDialog::~SettingsDialog() {
  gtk_widget_destroy(GTK_WIDGET(myDialog));
}

gint SettingsDialog::run() {
  gtk_widget_show_all(GTK_WIDGET(myDialog));

  return gtk_dialog_run(myDialog);
}

// vim:ts=2:sw=2:et
