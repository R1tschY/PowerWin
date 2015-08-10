/*
 * resources.h
 *
 *  Created on: 09.08.2015
 *      Author: Richard
 */

#ifndef LIBS_WINDOWS_BASE_RESOURCES_H_
#define LIBS_WINDOWS_BASE_RESOURCES_H_

#include <string>
#include <windows/extra/dll.h>
#include <windows/core/memory.h>
#include <windows.h>

namespace Windows {

class Resources
{
public:
  static std::wstring getString(unsigned id) { return getString(Module::getInstance(), id); }
  static std::wstring getString(HINSTANCE hinstance, unsigned id);

  static HICON getIcon(int id) { return getIcon(Module::getInstance(), id); }
  static HICON getIcon(HINSTANCE hinstance, int id);

  static HICON getIcon(int id, unsigned size) { return getIcon(Module::getInstance(), id, size); }
  static HICON getIcon(HINSTANCE hinstance, int id, unsigned size);

  static HICON getIconMetric(int id) { return getIconMetric(Module::getInstance(), id); }
  static HICON getIconMetric(HINSTANCE hinstance, int id);
};

} /* namespace Windows */

#endif /* LIBS_WINDOWS_BASE_RESOURCES_H_ */
