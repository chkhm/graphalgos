
#ifndef _NODE_HPP_
#define _NODE_HPP_

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

#include "element.hpp"

namespace ga {

    class Edge;
    class Graph;

    class Node : public Element {
    public:
        friend std::ostream &operator<<(std::ostream& out, const Node &n);
        friend std::istream &operator>>(std::istream& in, Node &n);

        Node(Graph *g, const std::string &name ="", int weight = 0);
        virtual ~Node();
        std::vector<Node*> neighbors();
        std::vector<Edge*> edges();
        void add_edge(Edge *e);
        void add_edges(const std::vector<Edge*> &e);
        void clear_edges();
        void remove_edge(Edge *e);
        void remove_edge(const std::vector<Edge *> &e);
    };

}; // end namespace ga

#endif