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

#ifndef _GCONF_WRAPPER_H_
#define _GCONF_WRAPPER_H_

#include <string>

#include <gconf/gconf-client.h>

class GConfItem {
public:
  virtual ~GConfItem() {}

protected:
  GConfItem();  // I do not want to create instances of this class

  static void allocateClient();

protected:
  static GConfClient *ourClient;
};

class GConfKey : public GConfItem {
public:
  GConfKey(const std::string&);
  GConfKey(const GConfKey&, const std::string&);
 ~GConfKey() { }

  const std::string& path() const { return myKeyPath; }

  static std::string mergePath(const std::string&, const std::string);

private:
  std::string myKeyPath;
};

class GConfOption : public GConfItem {
protected:
  GConfOption(const GConfKey& key, const std::string& path): myIsSynchronized(false), myPath(GConfKey::mergePath(key.path(), path)) { }

  void setGConfValue(const GConfValue *);
  GConfValue *getGConfValue(GConfValueType) const;
  void unsetGConfValue();

protected:
  mutable bool myIsSynchronized;
  std::string myPath;
};

class GConfStringValue : public GConfOption {
public:
  GConfStringValue(const GConfKey&, const std::string&, const std::string& = "");
 ~GConfStringValue();

  const std::string& value() const;
  const std::string& setValue(const std::string& newValue);

private:
  mutable std::string myValue;
  std::string myDefaultValue;
};

class GConfBooleanValue : public GConfOption {
public:
  GConfBooleanValue(const GConfKey&, const std::string&, bool = false);
 ~GConfBooleanValue();

  bool value() const;
  bool setValue(bool newValue);

private:
  mutable bool myValue;
  bool myDefaultValue;
};

class GConfIntegerValue : public GConfOption {
public:
  GConfIntegerValue(const GConfKey&, const std::string&, int = false);
 ~GConfIntegerValue();

  int value() const;
  int setValue(int newValue);

private:
  mutable int myValue;
  int myDefaultValue;
};

#endif

// vim:ts=2:sw=2:et
