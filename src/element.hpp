


#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

namespace ga {

    class Graph;

    class Element {
    public:
        friend std::ostream &operator<<(std::ostream& out, const Element &e);
        friend std::istream &operator>>(std::istream& in, Element &e);

        Element(Graph *g, const std::string &name ="", int weight = 0);
        virtual ~Element();

        std::string name() const;
        int weight() const;
        void weight(int w);

        Graph *graph() const;
        void graph(Graph *g);

        bool has_label(const std::string &l) const;
        std::string label(const std::string &l) const;
        void label(const std::string &l, const std::string &v);
        std::vector<std::string> all_labels() const;
        std::vector<std::string> all_label_values() const;
        std::vector<std::pair<std::string, std::string>> all_label_value_pairs() const;
        size_t size_labels() const;

    protected:
        Graph *m_graph;
        std::string m_name;
        int m_weight;
        std::unordered_map<std::string, std::string> m_labels;
    };

}; // end namespace ga

#endif