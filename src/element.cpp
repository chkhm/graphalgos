
#include "element.hpp"
#include "graph.hpp"

namespace ga {

// friend 
std::ostream &operator<<(std::ostream& out, const Element &e);

// friend 
std::istream &operator>>(std::istream& in, Element &e);


Element::Element(Graph *g, const std::string &name, int weight)
    : m_graph(g), m_name(name), m_weight(weight)
    {}

// virtual 
Element::~Element()
{}

std::string Element::name() const {
    return m_name;
}

int Element::weight() const {
    return m_weight;
}

void Element::weight(int w) {
    m_weight = w;
}

Graph *Element::graph() const {
    return m_graph;
}

void Element::graph(Graph *g) {
    m_graph = g;
}

bool Element::has_label(const std::string &l) const {
    return (m_labels.find(l) != m_labels.end());
}

std::string Element::label(const std::string &l) const {
    return m_labels.at(l);
}

void Element::label(const std::string &l, const std::string &v) {
    m_labels[l] = v;
}

std::vector<std::string> Element::all_labels() const {
    std::vector<std::string> rslt(m_labels.size());
    size_t i = 0;
    for (auto &[label, ignored] : m_labels) {
        rslt[i] = label;
        i++;
    }
    return rslt;
}

std::vector<std::string> Element::all_label_values() const {
    std::vector<std::string> rslt(m_labels.size());
    size_t i = 0;
    for (auto &[ignored, value] : m_labels) {
        rslt[i] = value;
        i++;
    }
    return rslt;
}

std::vector<std::pair<std::string, std::string>> Element::all_label_value_pairs() const {
    std::vector<std::pair<std::string, std::string>> rslt(m_labels.size());
    size_t i = 0;
    for (auto &p : m_labels) {
        rslt[i] = p;
        i++;
    }
    return rslt;
}

size_t Element::size_labels() const {
    return m_labels.size();
}

} // end namespace ga
