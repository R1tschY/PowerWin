#ifndef SYSTEMMENU_H
#define SYSTEMMENU_H

#include "../stdafx.h"
#include "../Plugin.h"

class SystemMenuPlugin : public Plugin {
public:
  SystemMenuPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

  enum Menu : unsigned {
    MenuId_AlwaysOnTop
  };
};

#endif // SYSTEMMENU_H
