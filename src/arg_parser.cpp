

#include "arg_parser.hpp"

#include <iomanip>


namespace arg {

argname_type_t argname_type_of(const std::string &n) {
    if (n.length() < 1) {
        return argname_type_empty;
    }
    if (n[0] != '-') {
        return argname_type_free;
    }
    if (n.length() < 2) {
        return argname_type_single_dash;
    }
    if (n[1] != '-') {
        return argname_type_short;
    }
    if (n.length() < 3) {
        return argname_type_doubledash;
    }
    return argname_type_long;
}

arg_descr::arg_descr(char short_name, 
                    std::string long_name, 
                    bool has_value, 
                    std::string value,
                    std::string help) 
    : m_short_name(short_name), m_long_name(long_name), m_has_value(has_value), m_value(value), m_help(help)                    
{}

// virtual
arg_descr::~arg_descr()
{}

bool arg_descr::has_name(const std::string &n) const {
    // if it is empty or does not start with a dash return false
    if (n.length() < 1 || n[0] != '-') {
        return false;
    }
    // if it is one dash only check if shortname is empty
    if (n.length() < 2) {
        return (m_short_name == ' ');
    }
    // if 2nd char is not a dash it must be a shortname
    if (n[1] != '-') {
        return n[1] == m_short_name;
    }
    // if shorter than 3 chars it must be '--' which is not a legal name
    if (n.length() < 3) {
        return false;
    }
    // it started with '--' so it must be a long name
    std::string long_name_only = n.substr(2);
    return (long_name_only == m_long_name);
}

int arg_descr::update_from_args(int argc, char **argv, int argi) {
    if (argi >= argc) {
        throw MissingArgumentException("Running out of arguments");
    }

    std::string arg = argv[argi];

    argi++;    
    
    if (!has_name(arg)) {
        throw UnkownArgumentException(arg);
    }
    m_found_in_args = true;
    argname_type_t arg_type = argname_type_of(arg);
    switch(arg_type) {
    case argname_type_short: 
        if (m_has_value) {
            if (arg.length() > 2) {
                m_value = arg.substr(2);
            } else {
                if (argi >= argc) {
                    throw MissingArgumentException("Need an argument for param: " + arg);
                }
                m_value = argv[argi];
                argi++;
            }
        }
        break;
    case argname_type_long: {
        if (m_has_value) {
            if (argi >= argc) {
                throw MissingArgumentException("Need an argument for param: " + arg);
            }
            m_value = argv[argi];
            argi++;
        }
    }
    break;
    case argname_type_empty: break;
    case argname_type_single_dash: break;
    case argname_type_doubledash: break;
    case argname_type_free: break;
    }
    return argi;
}

std::ostream &operator<<(std::ostream &os, const arg_descr &a) {
    os << "{ " << std::endl; 
    os << '\t' << std::quoted("short_name") << ": " << std::quoted(""+a.m_short_name)) << ", " << std::endl;
    os << '\t' << std::quoted("long_name")  << ": " << std::quoted(a.m_long_name)  << ", " << std::endl;
    os << '\t' << std::quoted("has_value")  << ": " << std::quoted(""+a.m_has_value)  << ", " << std::endl;
    os << '\t' << std::quoted("found_in_args") << ": " << std::quoted(""+a.m_found_in_args) << ", " << std::endl;
    os << '\t' << std::quoted("value") << ": " << std::quoted(a.m_value) << ", " << std::endl;
    os << '\t' << std::quoted("help") << ": " << std::quoted(a.m_help) << std::endl;
    os << " }" << std::endl;
    return os;
}

arg_descr_set::arg_descr_set(std::initializer_list<arg_descr> descriptors, const char *help) 
    : m_help(help)
{
    for (const arg_descr &ad : descriptors) {
        arg_descr *p_ad = new arg_descr(ad);
        m_all_descriptors.push_back(p_ad);
        m_long_name_map[p_ad->m_long_name] = p_ad;
        m_short_name_map[p_ad->m_short_name] = p_ad;
    }
    // m_help = help;
}

// virtual 
arg_descr_set::~arg_descr_set() {
    m_long_name_map.clear();
    m_short_name_map.clear();
    for (arg_descr *arg_descr : m_all_descriptors) {
        if (arg_descr) {
            delete arg_descr;
        }
    }
}

bool arg_descr_set::contains(const std::string &n) const {
    argname_type_t t = argname_type_of(n);

    if (t == argname_type_short) {
        return (m_short_name_map.find(n[1]) != m_short_name_map.end());
    } else if (t == argname_type_long) {
        std::string long_name_only = n.substr(2);
        return (m_long_name_map.find(long_name_only) != m_long_name_map.end());;    
    }
    return false;
}

arg_descr *arg_descr_set::at(const std::string &descr_name) {
    if (!contains(descr_name)) {
        throw UnkownArgumentException(descr_name);
    }
    argname_type_t tn = argname_type_of(descr_name);
    if (tn == argname_type_short) {
        return m_short_name_map[descr_name[1]];
    } else if (tn == argname_type_long) {
        return m_long_name_map[descr_name.substr(2)];
    }
    throw UnkownArgumentException("not an argument: \"" + descr_name + "\"");
}

//void arg_descr_set::insert(const arg_descr &descr) {
//    // TODO
//}

int arg_descr_set::update(int argc, char **argv, int argi) {
    if (argi >= argc) {
        return argi;
    }
    if (contains(argv[argi])) {
        arg_descr *arg_descr = at(argv[argi]);
        return arg_descr->update_from_args(argc, argv, argi);
    } else {
        throw UnkownArgumentException(argv[argi]);
    }
    return argi+1;
}

void collect_cmd_line_args(int argc, char **argv, arg_descr_set &param_vals) {
    std::list<std::string> free_args;
    std::unordered_map<std::string, std::string> cmd_line_dict;
    int i = 1;
    while (i <argc) {
        if (argv[i]) {
            std::string arg =  argv[i];
            argname_type_t t = argname_type_of(arg);
            switch (t) {
                case argname_type_short:
                case argname_type_long:
                    std::cout << "update " << arg << std::endl;
                    i = param_vals.update(argc, argv, i); break;
                case argname_type_free:
                    std::cout << "free " << arg << std::endl;
                    free_args.push_back(arg); i++; break;
                default:
                    i++; break;
            }
        }
    }
}

} // end namespace.
