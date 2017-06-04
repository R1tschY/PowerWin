#ifndef APP_UTILS_AUTOSTART_H_
#define APP_UTILS_AUTOSTART_H_

#include <QString>

namespace PowerWin {

struct AutoStart {
  static bool add(const QString& product, const QString& program);
  static bool add();

  static bool remove(const QString& product);
  static bool remove();

  static bool contains(const QString& program);
  static bool contains(const QString& product, const QString& program);
  static bool containsProduct();
  static bool containsProgram();
  
private:
  Autostart() = delete;
};

} /* namespace PowerWin */

#endif /* APP_UTILS_AUTOSTART_H_ */
