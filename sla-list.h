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

#ifndef __SLA_LIST_H__
#define __SLA_LIST_H__

#include <gtk/gtkliststore.h>
#include <gtk/gtktreeview.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkcellrenderertoggle.h>

#include "launchable-item.h"

class SLAList {
public:
  SLAList(int, LaunchableItems&);
 ~SLAList();

  GtkWidget *getWidget() { return myWidget; }

private:
  static void _renderText(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter, gpointer self);
  static void _renderBool(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter, gpointer self);
  static void _moveUp(GtkButton *, void *);
  static void _moveDown(GtkButton *, void *);
  static void _toggleBool(GtkCellRendererToggle *, const gchar *, void *);

  void renderText(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter);
  void renderBool(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter);
  void moveUp(GtkButton *);
  void moveDown(GtkButton *);
  void toggleBool(GtkCellRendererToggle *, const gchar *);

  void swap(GtkTreeIter *a, GtkTreeIter *b);

private:
  GtkWidget *myWidget;
  GtkListStore *myStore;
  GtkTreeView *myView;
  GtkTreeSelection *mySelection;

  LaunchableItems& myItems;
};

#endif

// vim:ts=2:sw=2:et
