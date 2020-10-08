#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifdef NDEBUG
#   define ASSERT(x) ((void)0)
#   define ASSERT_GL(x) ((void)0)
#else
#   include <cassert>
#   include <iostream>
#   define ASSERT(x) assert(x)
#   define ASSERT_GL(msg) {if(db::checkGLError()){std::cerr<<(msg)<<'\n';ASSERT(false);}}
#endif

namespace db {
    bool checkGLError();
    const char *getGLErrorStr(int error);
}

#endif
