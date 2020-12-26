
#ifndef _EDGE_HPP_
#define _EDGE_HPP_

#include <iostream>
#include <string>

#include "element.hpp"

namespace ga {

    class Node;
    class Graph;

    class Edge : public Element {
    public:
        friend std::ostream &operator<<(std::ostream &os, const Edge &e);
        friend std::istream &operator>>(std::istream &is, Edge &e);

        Edge(Graph *g);
        Edge(Graph *g, const std::string &n, Node *n1, Node *n2, int weight, bool bidirection);
        virtual ~Edge();
        Node *src_node() const;
        Node *dst_node() const;

        bool bidirectional() const;
        void bidirectional(bool b);

    private:
        bool m_bidirectional;
        Node *m_src;
        Node *m_dst;
    };
};

#endif
