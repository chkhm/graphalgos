
#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_


#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include "edge.hpp"
#include "node.hpp"

namespace ga {

    class Graph {
        public:

        static bool pretty_print_flag();
        static void pretty_print_flag(bool f);

        friend std::ostream &operator<<(std::ostream &os, const Graph &g);
        friend std::istream &operator>>(std::istream &is, Graph &g);
        friend std::ostream &to_graphviz(std::ostream &os, const Graph &G, const std::unordered_set<std::string> &print_labels, 
                                         bool use_color, bool use_style, bool landscape);

        Graph(const std::string &name = "");
        virtual ~Graph();

        void clear();
        bool add_node(Node *n);
        bool remove_node(Node *n);
        bool add_edge(Edge *e);
        bool remove_edge(Edge *e);

        std::vector<Node*> all_nodes() const;
        Node *find_node(const std::string &name) const;
        size_t size_nodes() const;

        std::vector<Edge*> all_edges() const;
        std::vector<Edge*> all_src_edges(const Node *src) const;

        Edge *find_edge(const std::string &name) const;
        std::vector<Edge *> find_edges(Node *n1, Node *n2) const;
        Edge *find_shortest_edge(Node *n1, Node *n2) const;
        size_t size_edges() const;

        std::string name() const;

        private:
        static bool c_pretty_print_flag;
        std::string m_name;
        std::unordered_map<std::string, Node*> m_nodes;
        std::unordered_map<std::string, Edge*> m_edges;
    };

    Graph *random_graph(size_t num_nodes, size_t num_edges, bool connected, bool use_color, bool use_style);
    std::list<Node*> shortest_path(Node *src, Node *dst, bool markup_edges=false);
    std::pair<std::unordered_map<Node *, int>, std::unordered_map<Node *, Node *>>
        all_shortest_paths(Node *src, Node *dst);
    std::ostream &operator<<(std::ostream &os, const std::vector<Edge *> edge_list);
    std::ostream &operator<<(std::ostream &os, const std::vector<Edge *> edge_list);
    std::ostream &operator<<(std::ostream &os, const std::list<Edge *> edge_list);
    std::ostream &operator<<(std::ostream &os, const std::list<Node *> edge_list);
    std::ostream &operator<<(std::ostream &os, const std::vector<Node *> edge_list);
};

#endif