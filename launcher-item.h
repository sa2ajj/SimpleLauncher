#ifndef __LAUNCHER_ITEM_H__
#define __LAUNCHER_ITEM_H__

#include <string>

#include <gdk-pixbuf/gdk-pixbuf.h>

class LauncherItem {
public:
  LauncherItem();
 ~LauncherItem();

  bool load(const std::string&);

  GdkPixbuf *getIcon(int icon_size) const;

  const std::string& getName() const { return myName; }
  const std::string& getComment() const { return myComment; }
  const std::string& getService() const { return myService; }
private:
  std::string myName, myComment, myIcon, myService;
};

#endif
