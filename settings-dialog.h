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

#ifndef __SETTINGS_DIALOG_H_
#define __SETTINGS_DIALOG_H_

#include <vector>

#include <gtk/gtkdialog.h>

#include "gconf-wrapper.h"
#include "launcher-item.h"
#include "dialog-entry.h"
#include "settings-page.h"

class SettingsDialog {
public:
  SettingsDialog(GtkWindow *parent, LauncherItems& items, GConfBooleanOption& transparent, GConfIntegerOption& icon_size);
 ~SettingsDialog();

  gint run();

  void updateValues();

  void addPage(const std::string&, SettingsPage *);

private:
  std::vector<SettingsPage *> myPages;

  GtkDialog *myDialog;
  GtkNotebook *myNotebook;

  SettingsDialogBooleanEntry myTransparent;
  SettingsDialogIconSizeEntry myIconSize;
};

#endif

// vim:ts=2:sw=2:et
