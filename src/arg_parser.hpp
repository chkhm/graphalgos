
#ifndef _ARG_PARSER_HPP_
#define _ARG_PARSER_HPP_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>

namespace arg {

class UnkownArgumentException : public std::logic_error {
    public:
    UnkownArgumentException(const std::string &why) : std::logic_error(why) {}
    virtual const char *what() const noexcept { return m_what.c_str(); }

    private:
    std::string m_what;
};

class MissingArgumentException : public std::logic_error {
    public:
    MissingArgumentException(const std::string &why) : std::logic_error(why) {}
    virtual const char *what() const noexcept { return m_what.c_str(); }

    private:
    std::string m_what;
};


enum argtype_t {
    argtype_string,
    argtype_char,
    argtype_int,
    argtype_uint,
    arg_type_float
};

enum argname_type_t {
    argname_type_short,
    argname_type_long,
    argname_type_empty,
    argname_type_single_dash,
    argname_type_doubledash,
    argname_type_free
};

argname_type_t argname_type_of(const std::string &n);

struct arg_descr {
    arg_descr( char short_name, 
                    std::string long_name, 
                    bool has_value,
                    std::string value,
                    std::string help);
    
    arg_descr() = default;
    arg_descr(arg_descr const&) = default;
    
    virtual ~arg_descr();

    bool has_name(const std::string &n) const;
    int update_from_args(int argc, char **argv, int argi);

    char m_short_name = 0;
    std::string m_long_name;
    bool m_has_value = false;
    bool m_found_in_args = false;
    std::string m_value;
    std::string m_help;
};

std::ostream &operator<<(std::ostream &os, const arg_descr &a);

class arg_descr_set {
public:
    // arg_descr_set(const std::list<arg_descr *> &descriptors, const std::string &help);

    arg_descr_set() = default;
    arg_descr_set(arg_descr_set const&) = default;
    arg_descr_set(std::initializer_list<arg_descr> descriptors, const char *help); 
    arg_descr_set(std::initializer_list<arg_descr_set> descriptors); //, const char *help); 

    virtual ~arg_descr_set();

    void insert(const arg_descr &descr);
    int update(int argc, char **argv, int argi);
    bool contains(const std::string &descr_name) const;
    arg_descr *at(const std::string &descr_name);
    std::string help() const { return m_help; };
    void help(const std::string &h) { m_help = h; }

private:
    std::list<arg_descr *> m_all_descriptors;
    std::unordered_map<std::string, arg_descr *> m_long_name_map;
    std::unordered_map<char, arg_descr *> m_short_name_map;
    std::string m_help;
};

std::ostream &operator<<(std::ostream &os, const arg_descr_set &args);


void collect_cmd_line_args(int argc, char **argv, arg_descr_set &param_vals); 

} // end namespace.

#endif // ARG_PARSER