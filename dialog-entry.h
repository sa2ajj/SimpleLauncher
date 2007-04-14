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

#ifndef _DIALOG_ENTRY_H_
#define _DIALOG_ENTRY_H_

#include <gtk/gtkwidget.h>

#include <hildon-widgets/hildon-number-editor.h>

#include "gconf-wrapper.h"

class SettingsDialogEntry {
public:
	virtual ~SettingsDialogEntry() {}

  const std::string& name() const { return myName; }
  virtual GtkWidget *getWidget() const = 0;

  virtual void updateValue() = 0;

protected:
	SettingsDialogEntry(GConfOption& option, const std::string& name): myOption(option), myName(name) {}

protected:
	GConfOption& myOption;
	const std::string myName;
};

class SettingsDialogStringEntry : public SettingsDialogEntry {
public:
  SettingsDialogStringEntry(GConfStringOption& option, const std::string& name): SettingsDialogEntry(option, name) {}

  void updateValue();
  GtkWidget *getWidget() const;
};

class SettingsDialogBooleanEntry : public SettingsDialogEntry {
public:
  SettingsDialogBooleanEntry(GConfBooleanOption& option, const std::string& name);

  void updateValue();
  GtkWidget *getWidget() const { return myWidget; }

private:
  GtkWidget *myWidget;
};

class SettingsDialogIntegerEntry : public SettingsDialogEntry {
public:
  SettingsDialogIntegerEntry(GConfIntegerOption& option, const std::string& name, int minValue, int maxValue);

  void updateValue();
  GtkWidget *getWidget() const { return GTK_WIDGET(mySpinBox); }

private:
  HildonNumberEditor *mySpinBox;
};

#endif

// vim:ts=2:sw=2:et
