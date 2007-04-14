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
#include <gtk/gtklabel.h>

#include "settings-page-entries.h"

SettingsPageWithEntries::SettingsPageWithEntries() {
  myBox = gtk_vbox_new(false, 0);
  myGroup = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
}

SettingsPageWithEntries::~SettingsPageWithEntries() {
  if (myGroup != NULL) {
    g_object_unref(G_OBJECT(myGroup));
    myGroup = NULL;
  }
}

void SettingsPageWithEntries::addEntry(SettingsDialogEntry *entry) {
  GtkWidget *box = gtk_hbox_new(false, 0);
  GtkWidget *label = gtk_label_new(entry->name().c_str());

  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);

  gtk_size_group_add_widget(myGroup, label);
  gtk_box_pack_start(GTK_BOX(box), label, false, false, 0);
  gtk_box_pack_start(GTK_BOX(box), entry->getWidget(), false, false, 2);

  gtk_box_pack_start(GTK_BOX(myBox), box, false, false, 0);
}
