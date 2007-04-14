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

enum {
  SLA_STORE_COLUMN_ICON = 0,
  SLA_STORE_COLUMN_INDEX,
  SLA_STORE_COLUMN_LAST
};

static GtkWidget *gtk_button_new_stock_image_only(const gchar *stock_id) {
  GtkWidget *button = gtk_button_new();
  GtkWidget *image = gtk_image_new_from_stock(stock_id, GTK_ICON_SIZE_BUTTON);

  gtk_container_add(GTK_CONTAINER(button), image);

  return button;
}

SettingsPageWithItems::SettingsPageWithItems(int icon_size, LauncherItems& items): myWidget(NULL), myStore(NULL), myView(NULL), mySelection(NULL), myLastSelection(NULL), myItems(items) {
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  myStore = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_INT);

  myView = GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(myStore)));
  gtk_tree_view_set_headers_visible(myView, FALSE);
  g_object_set(myView, "allow-checkbox-mode", FALSE, NULL);

  mySelection = gtk_tree_view_get_selection(myView);
  gtk_tree_selection_set_mode(mySelection, GTK_SELECTION_SINGLE);

  g_signal_connect(G_OBJECT(mySelection), "changed", G_CALLBACK(_selectionChanged), this);

  renderer = gtk_cell_renderer_pixbuf_new();
  g_object_set(renderer, "yalign", 0.0, NULL);
  gtk_cell_renderer_set_fixed_size(renderer, icon_size+4, -1);
  column = GTK_TREE_VIEW_COLUMN(gtk_tree_view_column_new_with_attributes("", renderer, "pixbuf", 0, NULL));

  gtk_tree_view_insert_column(myView, column, -1);

  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "yalign", 0.0, NULL);
  column = gtk_tree_view_column_new();
  gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand(column, TRUE);
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(column, renderer, _renderText, this, NULL);

  gtk_tree_view_insert_column(myView, column, -1);

  renderer = gtk_cell_renderer_toggle_new();
  g_object_set(renderer, "activatable", TRUE, NULL);
  g_signal_connect(renderer, "toggled", G_CALLBACK(_toggleBool), this);
  column = gtk_tree_view_column_new();
  gtk_tree_view_column_pack_start(column, renderer, TRUE);
  gtk_tree_view_column_set_cell_data_func(column, renderer, _renderBool, this, NULL);

  gtk_tree_view_insert_column(myView, column, -1);

  GtkWidget *swindow = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(swindow), GTK_WIDGET(myView));

  GtkWidget *move_up = gtk_button_new_stock_image_only(GTK_STOCK_GO_UP),
            *move_down = gtk_button_new_stock_image_only(GTK_STOCK_GO_DOWN);
  GtkTable *table = GTK_TABLE(gtk_table_new(4, 1, FALSE));
  gtk_table_attach(table, move_up, 0, 1, 1, 2, (GtkAttachOptions)(GTK_EXPAND|GTK_FILL), GTK_EXPAND, 0, 0);
  gtk_table_attach(table, move_down, 0, 1, 2, 3, (GtkAttachOptions)(GTK_EXPAND|GTK_FILL), GTK_EXPAND, 0, 0);

  g_signal_connect(move_up, "clicked", G_CALLBACK(_moveUp), this);
  g_signal_connect(move_down, "clicked", G_CALLBACK(_moveDown), this);

  myWidget = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(myWidget), swindow, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(myWidget), GTK_WIDGET(table), FALSE, FALSE, 0);

  gtk_widget_show_all(myWidget);

  for (LauncherItems::Names::const_iterator it = myItems.myNames.begin(); it != myItems.myNames.end(); ++it) {
    LauncherItem *item = myItems.myItems[*it];
    GtkTreeIter iter;

    gtk_list_store_append(myStore, &iter);
    {
      GdkPixbuf *icon = item->getIcon(icon_size);
      gtk_list_store_set(myStore, &iter, SLA_STORE_COLUMN_ICON, icon, SLA_STORE_COLUMN_INDEX, it-myItems.myNames.begin(), -1);
      g_object_unref(G_OBJECT(icon));
    }
  }
}

SettingsPageWithItems::~SettingsPageWithItems() {
  // FIXME: do something! :)
}

void SettingsPageWithItems::_selectionChanged(GtkTreeSelection *selection, void *self) {
  ((SettingsPageWithItems *)self)->selectionChanged(selection);
}

void SettingsPageWithItems::selectionChanged(GtkTreeSelection *) {
  if (myLastSelection != NULL) {
    kickIt(myLastSelection);
    gtk_tree_iter_free(myLastSelection);
    myLastSelection = NULL;
  }

  GtkTreeIter iter;
  GtkTreeModel *dummy;

  if (gtk_tree_selection_get_selected(mySelection, &dummy, &iter)) {
    kickIt(&iter);
    myLastSelection = gtk_tree_iter_copy(&iter);
  }
}

void SettingsPageWithItems::kickIt(GtkTreeIter *iter) {
  GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(myStore), iter);

  if (path != NULL) {
    g_signal_emit_by_name(myStore, "row-changed", path, iter);
    gtk_tree_path_free(path);
  }
}

void SettingsPageWithItems::_renderText(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter, gpointer self) {
  ((SettingsPageWithItems *)self)->renderText(column, cell, model, iter);
}

void SettingsPageWithItems::_renderBool(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter, gpointer self) {
  ((SettingsPageWithItems *)self)->renderBool(column, cell, model, iter);
}

void SettingsPageWithItems::_toggleBool(GtkCellRendererToggle *renderer, const gchar *path, void *self) {
  ((SettingsPageWithItems *)self)->toggleBool(renderer, path);
}

void SettingsPageWithItems::_moveUp(GtkButton *button, void *self) {
  ((SettingsPageWithItems *)self)->moveUp(button);
}

void SettingsPageWithItems::_moveDown(GtkButton *button, void *self) {
  ((SettingsPageWithItems *)self)->moveDown(button);
}

void SettingsPageWithItems::renderText(GtkTreeViewColumn *, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter) {
  int index;

  gtk_tree_model_get(GTK_TREE_MODEL(myStore), iter, SLA_STORE_COLUMN_INDEX, &index, -1);

  LauncherItem *item = myItems[index];

  if (gtk_tree_selection_iter_is_selected(mySelection, iter)) {
    gchar *text = g_markup_printf_escaped("%s\n<small>%s</small>", item->getName().c_str(), item->getComment().c_str());
    g_object_set(cell, "markup", text, NULL);
    g_free(text);
  } else {
    g_object_set(cell, "text", item->getName().c_str(), NULL);
  }
}

void SettingsPageWithItems::renderBool(GtkTreeViewColumn *, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter) {
  int index;

  gtk_tree_model_get(GTK_TREE_MODEL(myStore), iter, SLA_STORE_COLUMN_INDEX, &index, -1);

  g_object_set(cell, "active", myItems[index]->isEnabled(), NULL);
}

void SettingsPageWithItems::toggleBool(GtkCellRendererToggle *renderer, const gchar *spath) {
  GtkTreeIter iter;

  if (gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(myStore), &iter, spath)) {
    int index;

    gtk_tree_model_get(GTK_TREE_MODEL(myStore), &iter, SLA_STORE_COLUMN_INDEX, &index, -1);
    myItems[index]->toggle();
  }
}

void SettingsPageWithItems::moveUp(GtkButton *) {
  GtkTreeModel *dummy;
  GtkTreeIter current;

  if (gtk_tree_selection_get_selected(mySelection, &dummy, &current)) {
    GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(myStore), &current);

    // FIXME: we kinda expexted path to be non-NULL here :)
    if (gtk_tree_path_prev(path)) {
      GtkTreeIter next;

      if (gtk_tree_model_get_iter(GTK_TREE_MODEL(myStore), &next, path)) {
        swap(current, next);
      }
    }

    gtk_tree_path_free(path);
  }
}

void SettingsPageWithItems::moveDown(GtkButton *) {
  GtkTreeModel *dummy;
  GtkTreeIter current;

  if (gtk_tree_selection_get_selected(mySelection, &dummy, &current)) {
    GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(myStore), &current);
    GtkTreeIter next;

    // FIXME: we kinda expexted path to be non-NULL here :)
    gtk_tree_path_next(path);

    if (gtk_tree_model_get_iter(GTK_TREE_MODEL(myStore), &next, path)) {
      swap(current, next);
    }

    gtk_tree_path_free(path);
  }
}

void SettingsPageWithItems::swap(GtkTreeIter& a, GtkTreeIter& b) {
  int i1, i2;

  gtk_tree_model_get(GTK_TREE_MODEL(myStore), &a, SLA_STORE_COLUMN_INDEX, &i1, -1);
  gtk_tree_model_get(GTK_TREE_MODEL(myStore), &b, SLA_STORE_COLUMN_INDEX, &i2, -1);

  myItems.swap(i1, i2);

  gtk_list_store_set(myStore, &a, SLA_STORE_COLUMN_INDEX, i2, -1);
  gtk_list_store_set(myStore, &b, SLA_STORE_COLUMN_INDEX, i1, -1);

  gtk_list_store_swap(myStore, &a, &b);
}

// vim:ts=2:sw=2:et
