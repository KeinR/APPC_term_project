#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <sstream>
#include <string>

class serror: public std::stringstream {
    bool severe;
    void doRaise();
public:
    serror(bool severe = false);
    ~serror();
    static void raise(const std::string &message, bool severe);
};

#endif
