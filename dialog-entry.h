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

#include "gconf-wrapper.h"

class SettingsDialogEntry {
public:
	virtual ~SettingsDialogEntry() {}

protected:
	SettingsDialogEntry(GConfOption& option, const std::string& name);

  const std::string& name() const { return myName; }

  virtual void updateValue() = 0;

protected:
	GConfOption& myOption;
	const std::string myName;
};

class SettingsDialogStringEntry : public SettingsDialogEntry {
public:
  SettingsDialogStringEntry(GConfStringOption& option, const std::string& name): SettingsDialogEntry(option, name) {}

  void updateValue();
};

class SettingsDialogBooleanEntry : public SettingsDialogEntry {
public:
  SettingsDialogBooleanEntry(GConfBooleanOption& option, const std::string& name): SettingsDialogEntry(option, name) {}

  void updateValue();
};

class SettingsDialogIntegerEntry : public SettingsDialogEntry {
public:
  SettingsDialogIntegerEntry(GConfIntegerOption& option, const std::string& name): SettingsDialogEntry(option, name) {}

  void updateValue();
};

#endif

// vim:ts=2:sw=2:et
