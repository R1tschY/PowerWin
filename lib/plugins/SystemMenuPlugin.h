#ifndef SYSTEMMENU_H
#define SYSTEMMENU_H

#include "../Plugin.h"

class SystemMenuPlugin : public Plugin {
public:
  SystemMenuPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

  enum Menu : unsigned {
    MenuId_Sep1 = 0xFFFFFF00,
    MenuId_AlwaysOnTop = 0x00270,
    MenuId_Sep2 = 0xFFFFFF10,
  };
};

#endif // SYSTEMMENU_H
