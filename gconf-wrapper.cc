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

#include "gconf-wrapper.h"

GConfClient *GConfItem::ourClient = NULL;

GConfItem::GConfItem() {
  allocateClient();
}

void GConfItem::allocateClient() {
  if (ourClient == NULL) {
    ourClient = gconf_client_get_default();
  }
}

GConfKey::GConfKey(const std::string& path): myKeyPath(path) {
}

std::string GConfKey::merge(const std::string& path) const {
  std::string result = myKeyPath;

  if (path.empty()) {
    // this is actually a bad situation, what to do??
  } else if (path[0] == '/') {
    result.append(path);
  } else {
    result.append("/");
    result.append(path);
  }

  return result;
}

void GConfOption::setGConfValue(const GConfValue *value) {
  GError *error = NULL;

  gconf_client_set(ourClient, myPath.c_str(), value, &error);

  if (error != NULL) {
    g_error_free(error);
  }
}

GConfValue *GConfOption::getGConfValue() const {
  GConfValue *result = NULL;
  GError *error = NULL;

  result = gconf_client_get_without_default(ourClient, myPath.c_str(), &error);

  if (error != NULL) {
    g_error_free(error);

    if (result != NULL) {
      gconf_value_free(result);
      result = NULL;
    }
  }

  if (result != NULL) {
    if (result->type != kind()) {
      gconf_value_free(result);

      result = 0;
    }
  }

  return result;
}

void GConfOption::unsetGConfValue() {
  GError *error = NULL;

  // TODO: should I be picky about errors?
  gconf_client_unset(ourClient, myPath.c_str(), &error);

  if (error != NULL) {
    g_error_free(error);
  }
}

GConfStringOption::GConfStringOption(const GConfKey& key, const std::string& name, const std::string& defaultValue):
  GConfOption(GCONF_VALUE_STRING, key, name),
  myDefaultValue(defaultValue) {
}

const std::string& GConfStringOption::value() const {
  if (!myIsSynchronized) {
    GConfValue *value = getGConfValue();

    if (value == NULL) {
      myValue = myDefaultValue;
    } else {
      myValue = gconf_value_get_string(value);

      gconf_value_free(value);
    }

    myIsSynchronized = true;
  }

  return myValue;
}

const std::string& GConfStringOption::setValue(const std::string& newValue) {
  if (!myIsSynchronized || (myValue != newValue)) {
    myValue = newValue;

    if (myValue == myDefaultValue) {
      unsetGConfValue();
    } else {
      GConfValue *value = gconf_value_new(GCONF_VALUE_STRING);

      gconf_value_set_string(value, myValue.c_str());

      setGConfValue(value);

      gconf_value_free(value);
    }

    myIsSynchronized = true;
  }

  return myValue;
}

GConfBooleanOption::GConfBooleanOption(const GConfKey& key, const std::string& name, bool defaultValue):
  GConfOption(GCONF_VALUE_BOOL, key, name),
  myDefaultValue(defaultValue) {
}

bool GConfBooleanOption::value() const {
  if (!myIsSynchronized) {
    GConfValue *value = getGConfValue();

    if (value == NULL) {
      myValue = myDefaultValue;
    } else {
      myValue = gconf_value_get_bool(value);

      gconf_value_free(value);
    }

    myIsSynchronized = true;
  }

  return myValue;
}

bool GConfBooleanOption::setValue(bool newValue) {
  if (!myIsSynchronized || (myValue != newValue)) {
    myValue = newValue;

    if (myValue == myDefaultValue) {
      unsetGConfValue();
    } else {
      GConfValue *value = gconf_value_new(GCONF_VALUE_BOOL);

      gconf_value_set_bool(value, myValue);

      setGConfValue(value);

      gconf_value_free(value);
    }

    myIsSynchronized = true;
  }

  return myValue;
}

GConfIntegerOption::GConfIntegerOption(const GConfKey& key, const std::string& name, int defaultValue):
  GConfOption(GCONF_VALUE_INT, key, name),
  myDefaultValue(defaultValue) {
}

int GConfIntegerOption::value() const {
  if (!myIsSynchronized) {
    GConfValue *value = getGConfValue();

    if (value == NULL) {
      myValue = myDefaultValue;
    } else {
      myValue = gconf_value_get_int(value);

      gconf_value_free(value);
    }

    myIsSynchronized = true;
  }

  return myValue;
}

int GConfIntegerOption::setValue(int newValue) {
  if (!myIsSynchronized || (myValue != newValue)) {
    myValue = newValue;

    if (myValue == myDefaultValue) {
      unsetGConfValue();
    } else {
      GConfValue *value = gconf_value_new(GCONF_VALUE_INT);

      gconf_value_set_int(value, myValue);

      setGConfValue(value);

      gconf_value_free(value);
    }

    myIsSynchronized = true;
  }

  return myValue;
}

// vim:ts=2:sw=2:et
