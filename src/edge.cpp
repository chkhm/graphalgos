
#include "errors.hpp"
#include "edge.hpp"
#include "node.hpp"
#include "graph.hpp"
#include <iomanip>
#include <algorithm>
#include <cctype>

namespace ga {

//friend 
std::ostream &operator<<(std::ostream &os, const Edge &e) {
    os << "{ \"name\": " << std::quoted(e.name());
    os << ", \"weight\": " << std::quoted(std::to_string(e.weight()));
    os << ", \"bidirectional\": " << (e.m_bidirectional ? std::quoted("true") : std::quoted("false"));
    os << ", \"n1\": " << std::quoted(e.src_node()->name());
    os << ", \"n2\": " << std::quoted(e.dst_node()->name());
    os << "\"labels\": [ ";
    bool first_round = true;
    for (auto & [label, value] : e.m_labels ) {
        if (first_round) {
            os << "{ " << std::quoted(label) << ": " << std::quoted(value) << " }";
            first_round = false;
        } else {
            os << ", { " << std::quoted(label) << ": " << std::quoted(value) << " }";
        }
    }
    os << " ]";
    os << " }";
    return os;
}
 
//friend 
std::istream &operator>>(std::istream &in, Edge &e) {
    Graph *g = e.graph();
    char c;
    std::string token1, token2;
    in >> std::skipws >> c;
    if (c != '{') {
        throw graph_exception("Malformed json: missing '{'");
    }
    bool more_to_read = true;
    while (more_to_read) {
        in >> std::skipws >> std::quoted(token1) >> std::skipws >> c;
        if (c != ':') {
            throw graph_exception("Malformed json: missing ':'");
        }
        if (token1 == "name") {
            in >> std::skipws >> std::quoted(token2);
            e.m_name = token2;
        } else if (token1 == "weight") {
            in >> std::skipws >> std::quoted(token2);
            e.m_weight = std::stoi(token2);
        } else if (token1 == "bidirectional") {
            in >> std::skipws >> std::quoted(token2);
            std::transform( token2.begin(), token2.end(), token2.begin(),
                            [](unsigned char c){ return std::tolower(c); });
            e.m_bidirectional = (token2 == "true") || (token2 == "t") || token2 == "1";
        } else if (token1 == "n1") {
            in >> std::skipws >> std::quoted(token2);
            Node *n = g->find_node(token2);
            if (n == nullptr) {
                n = new Node(g, token2);
                g->add_node(n);
            } 
            e.m_src = n;
        } else if (token1 == "n2") {
            in >> std::skipws >> std::quoted(token2);
            Node *n = g->find_node(token2);
            if (n == nullptr) {
                n = new Node(g, token2);
                g->add_node(n);
            } 
            e.m_dst = n;
        } else if (token1 == "labels") {
            in >> std::skipws >> c;
            if (c != '[') {
                throw graph_exception("Malformed json: expecting '[' after 'labels' tag");
            }
            in >> std::skipws >> c;
            if (c != '{' && c != ']') {
                throw graph_exception("Malformed json: expecting either '{' or ']' within 'labels' tag.");
            }
            bool more_labels = (c != ']');
            while (more_labels) {
                in >> std::skipws >> std::quoted(token1) >> std::skipws >> c;
                if (c != ':') {
                    throw graph_exception("Malformed json: missing ':' after a tag in labels");
                }
                in >> std::skipws >> std::quoted(token2) >> std::skipws >> c;
                if (c != '}') {
                    throw graph_exception("Malformed json: missing '}' after a completed label");
                }
                e.label(token1, token2);
                in >> std::skipws >> c;
                if (c == ']') {
                    more_labels = false;
                } else if (c == ',') {
                    in >> std::skipws >> c;
                    if (c != '{') {
                        throw graph_exception("Malformed json: expected '{' to start a new label");
                    }
                } else {
                    throw graph_exception("Malformed json: expecting either ',' or ']' within 'labels'.");
                } 
            }
        }
        in >> std::skipws >> c;
        if (c == '}') {
            more_to_read = false;
        } else if (c != ',') {
            throw graph_exception("Malformed json: expecting either ',' or '}'.");
        }
    } // end while (more_to_read)
     return in;
}

Edge::Edge(Graph *g)
    : Element(g)
    {}

Edge::Edge(Graph *g, const std::string &n, Node *src, Node *dst, int weight, bool bidirection)
    : Element(g, n, weight), m_src(src), m_dst(dst), m_bidirectional(bidirection)
    {}

// virtual 
Edge::~Edge()
{ }

Node *Edge::src_node() const {
    return m_src;
}

Node *Edge::dst_node() const {
    return m_dst;
}

bool Edge::bidirectional() const {
    return m_bidirectional;
}

void Edge::bidirectional(bool b) {
    m_bidirectional = b;
}


} // end namespace