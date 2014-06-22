#ifndef MACROS_H
#define MACROS_H

// A macro to disallow the copy constructor and operator= functions
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName & operator=(const TypeName&) = delete; \
  TypeName(const TypeName&) = delete

#endif // MACROS_H
