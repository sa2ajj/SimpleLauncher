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

#include <glib/gerror.h>
#include <glib/gutils.h>
#include <glib/gshell.h>
#include <glib/gspawn.h>
#include <glib/gmem.h>

#include "utils.h"

void runApplication(const std::string& whatToRun) {
  if (!whatToRun.empty()) {
    GError *error = NULL;
    std::string::size_type pos;
    std::string app, args;

    if ((pos = whatToRun.find(' ')) != std::string::npos) {
      app = whatToRun.substr(0, pos);
      args = whatToRun.substr(pos);
    } else {
      app = whatToRun;
    }

    {
      gchar *fullPath = g_find_program_in_path(app.c_str());

      app = std::string(fullPath);

      g_free(fullPath);
    }

    if (!args.empty()) {
      app += args;
    }

    if (!app.empty()) {
      gint argc;
      gchar **argv;
      GPid pid;

      if (g_shell_parse_argv (app.c_str(), &argc, &argv, &error)) {
        g_spawn_async(NULL, argv, NULL, (GSpawnFlags)0, NULL, NULL, &pid, &error);
      }

      if (error != NULL) {
        g_error_free(error);
      }
    }
  }
}
