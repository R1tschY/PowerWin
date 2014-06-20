#pragma once

#include <string>
#include <map>
#include <boost/noncopyable.hpp>

class Plugin : boost::noncopyable {
public:
  typedef std::map<std::wstring, std::wstring> Options;

  Plugin(const wchar_t* name);
  virtual ~Plugin();

  void activate();
  void deactivate();

  bool isActive() const { return active_; }
  const wchar_t* getName() const { return name_; }

  void setOptions(Options&& options) { options_ = std::move(options); }
  bool getBooleanOption(const std::wstring& key, bool default_);

protected:
  virtual void onActivate(const Options& options) = 0;
  virtual void onDeactivate() = 0;

private:
  bool active_;
  Options options_;
  const wchar_t* name_;
};

