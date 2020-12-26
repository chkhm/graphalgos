
#ifndef _GRAPH_EXCEPTION_H_
#define _GRAPH_EXCEPTION_H_

#include <exception>
#include <string>

namespace ga {

class graph_exception : public std::logic_error {
public:
    graph_exception(std::string why) : std::logic_error(why) {}
};


}

#endif