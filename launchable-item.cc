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

#include "launchable-item.h"
#include "utils.h"

bool LaunchableItem::activate(osso_context_t *context) {
  bool result = false;

  if (getService().empty() || !(result = osso_rpc_run_with_defaults(context, getService().c_str(), "top_application", NULL, DBUS_TYPE_INVALID) == OSSO_OK)) {
    runApplication(getExec());
    return true;
  } else {
    return result;
  }
}

// vim:ts=2:sw=2:et
