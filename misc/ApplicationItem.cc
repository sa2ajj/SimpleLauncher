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

#include "ApplicationItem.h"

static ApplicationItemFactory factory;

const std::string& ApplicationItemFactory::factoryName() const {
  const std::string& NAME = "ApplicationItem";

  return NAME;
}

BasicItem *ApplicationItemFactory::createItem(const std::string& itemID) const {
  return new ApplicationItem(itemID);
}

ApplicationItem::ApplicationItem(const std::string& itemID): BasicItem(factory.factoryName(), itemID) {
}

ApplicationItem::~ApplicationItem() {
}

bool ApplicationItem::load() {
  GKeyFileWrapper key_file;

  for (;;) {
    if (!key_file.load(myID)) {
      break;
    }

    if (key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_TYPE_FIELD) != "Application") {
      break;
    }

    myName = key_file.getLocaleString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_NAME_FIELD);
    myComment = key_file.getLocaleString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_COMMENT_FIELD);
    myIcon = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_ICON_FIELD);
    myService = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_SERVICE_FIELD);
    myExec = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_EXEC_FIELD);
    myTextDomain = key_file.getString(DESKTOP_ENTRY_GROUP, DESKTOP_ENTRY_TEXT_DOMAIN);

    break;
  }

  return (myEnabled = checkSanity());
}

void ApplicationItem::activate(osso_context_t *context) {
  bool result = false;

  if (myService.empty() || !(result = osso_rpc_run_with_defaults(context, myService.c_str(), "top_application", NULL, DBUS_TYPE_INVALID) == OSSO_OK)) {
    runApplication(getExec());
    return true;
  } else {
    return result;
  }
}
