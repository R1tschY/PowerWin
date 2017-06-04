#include "autostart.h"

#include <QSettings>
#include <QCoreApplication>

namespace PowerWin {

bool AutoStart::add(const QString& product, const QString& program)
{
  QSettings autorun(
      "HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run",
      QSettings::NativeFormat);

  autorun.setValue(program, product);
  return true;
}

bool AutoStart::add()
{
  return add(
      QCoreApplication::applicationName(),
      QCoreApplication::applicationFilePath());
}

bool AutoStart::remove(const QString& product)
{
  QSettings autorun(
      "HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run",
      QSettings::NativeFormat);

  autorun.remove(product);
  return true;
}

bool AutoStart::remove()
{
  return remove(
      QCoreApplication::applicationName());
}

bool AutoStart::contains(const QString& product, const QString& program)
{
  QSettings autorun(
      "HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run",
      QSettings::NativeFormat);

  return autorun.value(product).toString() == program;
}

bool AutoStart::contains(const QString& product)
{
  QSettings autorun(
      "HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run",
      QSettings::NativeFormat);

  return autorun.contains(product);
}

bool AutoStart::containsProduct()
{
  return contains(QCoreApplication::applicationName());
}

bool AutoStart::containsProgram()
{
  return contains(
      QCoreApplication::applicationName(),
      QCoreApplication::applicationFilePath());
}

} /* namespace PowerWin */
