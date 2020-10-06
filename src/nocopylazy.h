#ifndef NOCOPYLAZY_H_INCLUDED
#define NOCOPYLAZY_H_INCLUDED

// Extended if the reason for no copy or move constructors/operators
// is because it's not worth the time and effort due to them
// not being used, rather than any technical reason.
class nocopylazy {
public:
    nocopylazy();
    virtual ~nocopylazy() = 0;
    nocopylazy(nocopylazy&&) = delete;
    nocopylazy(const nocopylazy&) = delete;
    nocopylazy &operator=(nocopylazy&&) = delete;
    nocopylazy &operator=(const nocopylazy&) = delete;
};

#endif
