#include <iostream>

#include "gconf-wrapper.h"

int main() {
  g_type_init();

  GConfKey sla("/apps/simple-launcher");

  GConfBooleanOption transparency(sla, "transparency", false);

  std::cout << "Value: " << transparency.value() << std::endl;

  return 0;
}
