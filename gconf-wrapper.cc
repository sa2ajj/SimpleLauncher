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

GConfStringValue::GConfStringValue(const GConfKey& key, const std::string& name, const std::string& defaultValue):
  GConfOption(key, name),
  myDefaultValue(defaultValue) {
}

const std::string& GConfStringValue::value() const {
  if (!myIsSynchronized) {
    GConfValue *value = getGConfValue(GCONF_VALUE_STRING);

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

const std::string& GConfStringValue::setValue(const std::string& newValue) {
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

GConfBooleanValue::GConfBooleanValue(const GConfKey& key, const std::string& name, bool defaultValue):
  GConfOption(key, name),
  myDefaultValue(defaultValue) {
}

bool GConfBooleanValue::value() const {
  if (!myIsSynchronized) {
    GConfValue *value = getGConfValue(GCONF_VALUE_BOOL);

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

bool GConfBooleanValue::setValue(bool newValue) {
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

GConfIntegerValue::GConfIntegerValue(const GConfKey& key, const std::string& name, int defaultValue):
  GConfOption(key, name),
  myDefaultValue(defaultValue) {
}

int GConfIntegerValue::value() const {
  if (!myIsSynchronized) {
    GConfValue *value = getGConfValue(GCONF_VALUE_INT);

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

int GConfIntegerValue::setValue(int newValue) {
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
