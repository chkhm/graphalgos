
#include "errors.hpp"
#include "node.hpp"
#include "graph.hpp"
#include <iomanip>


namespace ga {

// friend 
std::ostream &operator<<(std::ostream& out, const Node &n) {
    out << "{ \"name\": " << std::quoted(n.name()) << ", ";
    out << "\"weight\": \"" << n.weight() << "\" "; 
    out << "\"labels\": [ ";
    bool first_round = true;
    for (auto & [label, value] : n.m_labels ) {
        if (first_round) {
            out << "{ " << std::quoted(label) << ": " << std::quoted(value) << " }";
            first_round = false;
        } else {
            out << ", { " << std::quoted(label) << ": " << std::quoted(value) << " }";
        }
    }
    out << " ]";
    out << " }";

    return out;
}

// friend 
std::istream &operator>>(std::istream& in, Node &n) {
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
            n.m_name = token2;
        } else if (token1 == "weight") {
            in >> std::skipws >> std::quoted(token2);
            n.m_weight = std::stoi(token2);
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
                n.label(token1, token2);
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
        } else if ( c != ',') {
            throw graph_exception("Malformed json: expecting either ',' or '}'");
        }
    }
    return in;
}

Node::Node(Graph *g, const std::string &name, int weight) 
    : Element(g, name, weight)
{ }

// virtual 
Node::~Node() 
{ }

//std::vector<Node*> Node::neighbors();

std::vector<Edge*> Node::edges() {
    return m_graph->all_src_edges(this);
}

//void Node::add_edge(edge *e);
//void Node::add_edges(const std::vector<Edge*> &e);
//void Node::clear_edges();
//void Node::remove_edge(Edge *e);
//void Node::remove_edge(const std::vector<Edge *> &e);

}