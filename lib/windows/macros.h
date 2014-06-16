#ifndef MACROS_H
#define MACROS_H

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName & operator=(const TypeName&); \
  TypeName(const TypeName&);

#endif // MACROS_H
