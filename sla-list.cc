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
#include <gtk/gtktable.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtktreeselection.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtkstock.h>

#include "sla-list.h"

SLAList::SLAList(int icon_size): myWidget(0), myStore(0), myView(0), mySelection(0) {
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  myStore = gtk_list_store_new(4, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_STRING);

  myView = GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(myStore)));
  gtk_tree_view_set_headers_visible(myView, FALSE);

  mySelection = gtk_tree_view_get_selection(myView);
  gtk_tree_selection_set_mode(mySelection, GTK_SELECTION_SINGLE);

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
  g_object_set(renderer, "activatable", TRUE, 0);
  g_signal_connect(renderer, "toggled", G_CALLBACK(_toggleBool), this);
  column = GTK_TREE_VIEW_COLUMN(gtk_tree_view_column_new_with_attributes("", renderer, "active", 2, 0));

  gtk_tree_view_insert_column(myView, column, -1);

  GtkWidget *swindow = gtk_scrolled_window_new(0, 0);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(swindow), GTK_WIDGET(myView));

  gtk_widget_set_size_request(swindow, 500, 300);

  GtkWidget *move_up = gtk_button_new_from_stock(GTK_STOCK_GO_UP),
            *move_down = gtk_button_new_from_stock(GTK_STOCK_GO_DOWN);
  GtkTable *table = GTK_TABLE(gtk_table_new(4, 1, FALSE));
  gtk_table_attach(table, move_up, 0, 1, 1, 2, (GtkAttachOptions)(GTK_EXPAND|GTK_FILL), GTK_EXPAND, 0, 0);
  gtk_table_attach(table, move_down, 0, 1, 2, 3, (GtkAttachOptions)(GTK_EXPAND|GTK_FILL), GTK_EXPAND, 0, 0);

  g_signal_connect(move_up, "clicked", G_CALLBACK(_moveUp), this);
  g_signal_connect(move_down, "clicked", G_CALLBACK(_moveDown), this);

  myWidget = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(myWidget), swindow, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(myWidget), GTK_WIDGET(table), FALSE, FALSE, 0);

  gtk_widget_show_all(myWidget);
}

SLAList::~SLAList() {
  // FIXME: do something! :)
}

void SLAList::collectItems(std::vector<std::pair<std::string, bool> >& result) {
  GtkTreeIter iter;

  if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(myStore), &iter)) {
    char *name;
    gboolean active;

    gtk_tree_model_get(GTK_TREE_MODEL(myStore), &iter, 3, &name, 2, &active, -1);

    result.push_back(std::pair<std::string, bool>(name, active));

    while (gtk_tree_model_iter_next(GTK_TREE_MODEL(myStore), &iter)) {
      gtk_tree_model_get(GTK_TREE_MODEL(myStore), &iter, 3, &name, 2, &active, -1);

      result.push_back(std::pair<std::string, bool>(name, active));
    }
  }
}

void SLAList::addItem(const char *filename, GdkPixbuf *pixbuf, const char *name, bool active) {
  GtkTreeIter iter;
  
  gtk_list_store_append(myStore, &iter);
  gtk_list_store_set(myStore, &iter, 0, pixbuf, 1, name, 2, active, 3, filename, -1);
}

void SLAList::_toggleBool(GtkCellRendererToggle *renderer, const gchar *path, void *self) {
  ((SLAList *)self)->toggleBool(renderer, path);
}

void SLAList::_moveUp(GtkButton *button, void *self) {
  ((SLAList *)self)->moveUp(button);
}

void SLAList::_moveDown(GtkButton *button, void *self) {
  ((SLAList *)self)->moveDown(button);
}

void SLAList::toggleBool(GtkCellRendererToggle *renderer, const gchar *spath) {
  GtkTreePath *path = gtk_tree_path_new_from_string(spath);

  if (path != 0) {
    GtkTreeIter iter;

    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(myStore), &iter, path)) {
      gboolean value;

      gtk_tree_model_get(GTK_TREE_MODEL(myStore), &iter, 2, &value, -1);
      gtk_list_store_set(myStore, &iter, 2, !value, -1);
    }
  }
}

void SLAList::moveUp(GtkButton *) {
  GtkTreeModel *dummy;
  GtkTreeIter current;

  if (gtk_tree_selection_get_selected(mySelection, &dummy, &current)) {
    GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(myStore), &current);

    // FIXME: we kinda expexted path to be non-NULL here :)
    if (gtk_tree_path_prev(path)) {
      GtkTreeIter next;

      if (gtk_tree_model_get_iter(GTK_TREE_MODEL(myStore), &next, path)) {
        gtk_list_store_swap(myStore, &current, &next);
      }
    }

    gtk_tree_path_free(path);
  }
}

void SLAList::moveDown(GtkButton *) {
  GtkTreeModel *dummy;
  GtkTreeIter current;

  if (gtk_tree_selection_get_selected(mySelection, &dummy, &current)) {
    GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(myStore), &current);
    GtkTreeIter next;

    // FIXME: we kinda expexted path to be non-NULL here :)
    gtk_tree_path_next(path);

    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(myStore), &next, path)) {
      gtk_list_store_swap(myStore, &current, &next);
    }

    gtk_tree_path_free(path);
  }
}

// vim:ts=2:sw=2:et
