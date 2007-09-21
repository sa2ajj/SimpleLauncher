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
  ApplicationItem *tempo = new ApplicationItem(itemID);

  return NULL;
}

ApplicationItem::ApplicationItem(const std::string& itemID): BasicItem(factory.factoryName(), itemID) {
}

ApplicationItem::~ApplicationItem() {
}
