#ifndef MA_EXCEPTIONS_H
#define MA_EXCEPTIONS_H

#include <exception>

namespace monoAtomic {

    class maSampleOutOfRange : public std::exception {
    public:
        char * what () {
            return "Error getting parent File";
        }
    };


    class maOrphanChannel : public std::exception {
    public:
        char * what () {
            return "Error getting parent File";
        }
    };
}

#endif
