#pragma once

#include <string>
#include <map>
#include <c++/stringview.h>


#include "windows/macros.h"

class Plugin {
  DISALLOW_COPY_AND_ASSIGN(Plugin);

public:
  typedef std::map<std::wstring, std::wstring> Options;

  Plugin(cpp::wstring_view name);
  virtual ~Plugin();

  void activate();
  void deactivate();

  bool isActive() const { return active_; }
  cpp::wstring_view getName() const { return name_; }

  void setOptions(Options options) { options_ = std::move(options); }
  bool getBooleanOption(cpp::wstring_view key, bool default_);

private:
  virtual void onActivate(const Options& options) = 0;
  virtual void onDeactivate() = 0;

private:
  bool active_;
  Options options_;
  const std::wstring name_;
};

