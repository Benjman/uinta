#ifndef UINTA_ASSERT_H
#define UINTA_ASSERT_H

#ifdef UINTA_DEBUG
#include <cassert>
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

#endif //UINTA_ASSERT_H
