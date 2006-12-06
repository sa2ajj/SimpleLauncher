// This file is a part of Simple Launcher
//
// Copyright (C) 2006, Mikhail Sobolev
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

#include <gtk/gtkscrolledwindow.h>
#include <gtk/gtkcellrendererpixbuf.h>
#include <gtk/gtkcellrenderertext.h>
#include <gtk/gtkcellrenderertoggle.h>

#include "sla-list.h"

SLAList::SLAList(int icon_size): myWidget(0), myStore(0), myView(0) {
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  myStore = gtk_list_store_new(3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_BOOLEAN);
  myView = GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(myStore)));

  gtk_tree_view_set_headers_visible(myView, FALSE);

  renderer = gtk_cell_renderer_pixbuf_new();
  g_object_set(renderer, "yalign", 0.0, 0);
  gtk_cell_renderer_set_fixed_size(renderer, icon_size+4, -1);
  column = GTK_TREE_VIEW_COLUMN(gtk_tree_view_column_new_with_attributes("", renderer, "pixbuf", 0, 0));

  gtk_tree_view_insert_column(myView, column, -1);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "yalign", 0.0, 0);
  column = GTK_TREE_VIEW_COLUMN(gtk_tree_view_column_new_with_attributes("", renderer, "text", 1, 0));
  gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand(column, TRUE);

  gtk_tree_view_insert_column(myView, column, -1);

  renderer = gtk_cell_renderer_toggle_new();
  column = GTK_TREE_VIEW_COLUMN(gtk_tree_view_column_new_with_attributes("", renderer, "active", 2, 0));

  gtk_tree_view_insert_column(myView, column, -1);

  myWidget = gtk_scrolled_window_new(0, 0);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(myWidget), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(myWidget), GTK_WIDGET(myView));
}

void SLAList::addItem(GdkPixbuf *pixbuf, const char *name, bool active) {
  GtkTreeIter iter;
  
  gtk_list_store_append(myStore, &iter);

  gtk_list_store_set(myStore, &iter, 0, pixbuf, 1, name, 2, active, -1);
}

// vim:ts=2:sw=2:et
