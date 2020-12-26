

#include "graph.hpp"
#include "errors.hpp"
#include <iomanip>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <limits>
#include <stdlib.h>

namespace ga {

// static 
bool Graph::c_pretty_print_flag = false;


// static 
bool Graph::pretty_print_flag() {
    return Graph::c_pretty_print_flag;
}
// static 
void Graph::pretty_print_flag(bool f) {
    c_pretty_print_flag = f;
}

// friend 
std::ostream &operator<<(std::ostream &os, const Graph &g) {
    if (Graph::pretty_print_flag()) {
        os << "{\n\t \"name\": \"" << g.name() << "\",\n\t \"nodes\": \n\t [ ";
    } else {
        os << "{ \"name\": \"" << g.name() << "\", \"nodes\": [ ";
    }
    {
        std::vector<Node*> all_nodes = g.all_nodes();
        // os << "node count: " << g.size_nodes() << " size all_nodes: " << all_nodes.size() << std::endl;
        std::vector<Node*>::iterator it = all_nodes.begin();
        if (it != all_nodes.end()) {
            if (*it) {
                if (Graph::pretty_print_flag()) {
                    os << "\n\t\t " << **it;
                } else {
                    os << **it;
                }
            } else {
                if (Graph::pretty_print_flag()) {
                    os << "\n\t\t null";
                } else {
                    os << "null";
                }
            }
            ++it;
        }
        while (it != all_nodes.end()) {
            if (*it) { 
                if (Graph::pretty_print_flag()) {
                    os << ",\n\t\t " << **it;
                } else {
                    os << ", " << **it;
                }
            } else {
                os << ", null";
            }
            ++it;
        }
    }
    if (Graph::pretty_print_flag()) {
        os << "\n\t ],\n\t \"edges\":\n\t [ ";    
    } else {
        os << " ], \"edges\": [ ";
    }
    {
        std::vector<Edge*> all_edges = g.all_edges();
        std::vector<Edge*>::iterator it = all_edges.begin();
        if (it != all_edges.end()) {
            if (*it) {
                if (Graph::pretty_print_flag()) {
                    os << "\n\t\t " << **it;
                } else {
                    os << **it;
                }
            } else {
                if (Graph::pretty_print_flag()) {
                    os << "\n\t\t null";
                } else {
                    os << "null";
                }
            }
            ++it;
        }
        while (it != all_edges.end()) {
            if (*it) {
                if (Graph::pretty_print_flag()) {
                    os << ",\n\t\t " << **it;
                } else {
                    os << ", " << **it;
                }
            } else {
                if (Graph::pretty_print_flag()) {
                    os << ",\n\t\t null";
                } else {
                    os << ", null";
                }
            }
            ++it;
        }
    }
    if (Graph::pretty_print_flag()) {
        os << "\n\t ]\n}\n";
    } else {
        os << " ] }";
    }
    return os;
}

// friend 
std::ostream &to_graphviz(  std::ostream &os, const Graph &g, const std::unordered_set<std::string> &print_labels, 
                            bool use_color, bool use_style, bool landscape) {
    os << "digraph G {" << std::endl;    
    if (landscape) {
        os << "orientation= \"landscape\";" << std::endl;
    }

    for (auto & node : g.all_nodes()) {
        os << node->name() << ';' << std::endl;
    }
    for (auto & edge : g.all_edges()) {
        os << edge->src_node()->name() << " -> " << edge->dst_node()->name();
        os << " [weight=" << edge->weight();

        if (use_color && edge->has_label("color")) {
            os << ", color=" << std::quoted(edge->label("color"));
        }
        if (use_style && edge->has_label("style")) {
            os << ", style=" << std::quoted(edge->label("style"));
        }

        std::stringstream ss;
        bool first_round = true;
        for (const auto &it : print_labels) {
            if (first_round) {
                first_round = false;
            } else {
                if (edge->has_label(it) || it == "weight" || it == "name")    
                    ss << ',';
            }
            if (edge->has_label(it)) {
                ss << it[0] << '=' << edge->label(it);
            } else if (it == "weight") {
                ss << "w=" << edge->weight();
            } else if (it == "name") {
                ss << "n=" << edge->name();
            }
        }
        os << ", label=" << std::quoted(ss.str());
        os << "]" << ';' << std::endl;
    }
    os << "}" << std::endl;
    return os;
}

std::vector<std::string> colors = { "black", "red", "green", "blue", "orange", "yellow", "purple" };
std::vector<std::string> styles = { "", "dotted", "solid", "dashed", "bold", "tapered" };

Graph *random_graph(size_t num_nodes, size_t num_edges, bool connected, bool use_colors, bool use_style) {
    Graph *g = new Graph;
    for (size_t i =0; i < num_nodes; i++) {
        Node *n = new Node(g, "n" + std::to_string(i), rand() % 100);
        g->add_node(n);
    }

    for (size_t j=0; j < num_edges; j++) {
        std::string name = "e" + std::to_string(j);
        std::string sn1 = "n" + std::to_string(rand() % num_nodes);
        std::string sn2 = "n" + std::to_string(rand() % num_nodes);
        bool bidirection = ((rand() % 2) == 0);
        int w = rand() % 100;        
        Edge *e = new Edge(g, name, g->find_node(sn1), g->find_node(sn2), w, bidirection);
        if (use_colors) {
            std::string color_value = colors[rand() % colors.size()];
            e->label("color", color_value);
        }
        if (use_style) {
            std::string style_value = styles[rand() % styles.size()];
            e->label("style", style_value);
        }
        
        g->add_edge(e);
    }
    return g;
}

std::list<Node*> shortest_path(Node *src, Node *dst, bool markup_edges) {
    auto [dist, pred] = all_shortest_paths(src, dst);
    // std::pair<std::unordered_map<Node *, int>, std::unordered_map<Node *, Node *>> paths = all_shortest_paths(src, dst);
    std::list<Node *> rslt;
    if (dist[dst] == std::numeric_limits<int>::max()) {
        return rslt;
    }
    for (Node *n = dst; n != src; n = pred[n]) {
        // std::cout << "n: " << n->name() << " pred[n] " << pred[n]->name() << std::endl;
        rslt.push_front(n);
    }
    rslt.push_front(src);
    if (markup_edges) {
        Graph *g = src->graph();
        bool first_round = true;
        Node *n1 = nullptr;
        for (Node *n2 : rslt) {
            if (first_round) {
                first_round = false;
            } else {
                Edge * e = g->find_shortest_edge(n1, n2);
                e->label("path", std::to_string(dist[n2]));
                e->label("color", "red");
                e->label("style", "bold");
            }
            n1 = n2;
        }
    }
    return rslt;
}

// friend 
std::pair<std::unordered_map<Node *, int>, std::unordered_map<Node *, Node *>>
all_shortest_paths(Node *src, Node *dst) {
    if (src->graph() != dst->graph()) {
        throw graph_exception("shortest path: src and dst are not in the same graph");
    }

    Graph *g = src->graph();
    std::vector<Node*> all_nodes = g->all_nodes();
    std::unordered_map<Node *, int> dist;
    std::unordered_map<Node *, Node *> pred; 
    for (auto n : all_nodes) {
        dist[n] = std::numeric_limits<int>::max();
        pred[n] = nullptr;
    }
    dist[src] = 0;

    // min priority queue
    std::priority_queue<std::pair<int, Node *>, std::vector<std::pair<int, Node *>>, std::greater<std::pair<int, Node *>>> queue;

    for (auto n : all_nodes) {
        queue.push(std::make_pair(dist[n], n));
    }
    while (!queue.empty()) {
        Node *n = queue.top().second;
        queue.pop();
        std::vector<Edge*> edges = n->edges();
        for (Edge *e : edges) {
            Node *u = e->dst_node();
            int alt = (dist[n] == std::numeric_limits<int>::max()) ? dist[n] : dist[n] + e->weight();
            if (alt < dist[u]) {
                // std::cout << "push: " << u->name() << ": " << dist[u] << " alt: " << alt << "n: " << n->name() << std::endl;
                dist[u] = alt;
                pred[u] = n;
                queue.push(std::make_pair(dist[u], u));
            }
        }
    }
    return std::make_pair(dist, pred);
}

// friend 
std::ostream &operator<<(std::ostream &os, const std::vector<Edge *> edge_list) {
    os << "{ ";
    bool first_round = true;
    for (auto &it : edge_list) {
        if (first_round) { 
            first_round = false;
        } else {
            os << ", ";
        }
        os << std::quoted(it->name());
    }
    os << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::list<Node *> edge_list) {
    os << "{ ";
    bool first_round = true;
    for (auto &it : edge_list) {
        if (first_round) { 
            first_round = false;
        } else {
            os << ", ";
        }
        os << std::quoted(it->name());
    }
    os << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Node *> node_list) {
    os << "{ ";
    bool first_round = true;
    for (auto &it : node_list) {
        if (first_round) { 
            first_round = false;
        } else {
            os << ", ";
        }
        os << std::quoted(it->name());
    }
    os << " }";
    return os;
}


/**
 * 
 * {
 *  "name": "potatoe",
 *  "nodes": [ ],
 *  "edges": [ ]
 * }
 * 
 */
// friend 
std::istream &operator>>(std::istream &is, Graph &g) {
    char c;
    std::string token;

    is >> std::skipws >> c;
    if (c != '{') {
        throw graph_exception("Malformed json: missing '{'");
    }

    bool more_graph_tokens = true;
    is >> std::quoted(token);
    if (token == "}") more_graph_tokens = false;
    while (more_graph_tokens)
    {
        is >> c;
        if (c != ':') {
            throw graph_exception("Malformed json 1: expected ':' " + token + " " + c);
        }   
        if (token == "name") {
            is >> std::skipws >> std::quoted(token);
            g.m_name = token;        
        } else if (token == "nodes") {
            is >> std::skipws >> c;
            if (c != '[') {
                throw graph_exception("Malformed json 2: missing '['");
            }
            bool more_nodes = true;
            while (more_nodes) {
                Node *node = new Node(&g);
                is >> *node;
                g.add_node(node);
                is >> std::skipws >> c;
                if ( c == ']' ) {
                    more_nodes = false;
                } else if (c != ',') {
                    throw graph_exception("Malformed json 3: expected ',' or ']'");
                }
            }
        } else if (token == "edges") {
            is >> std::skipws >> c;
            if (c != '[') {
                throw graph_exception("Malformed json 4: missing '['");
            }

            bool more_edges = true;
            while (more_edges) {
                Edge *edge = new Edge(&g);
                is >> *edge;
                g.add_edge(edge);
                is >> std::skipws >> c;
                if ( c == ']' ) {
                    more_edges = false;
                } else if (c != ',') {
                    throw graph_exception("Malformed json 5: expected ',' or ']'");
                }
            }
        }
        is >> c;
        if (c == '}') {
            more_graph_tokens = false;
        } else if (c != ',') {
            throw graph_exception("Malformed json 6: expected ',' or ']'");
        } else {
            is >> std::quoted(token);
        }
    }
    return is;
}


Graph::Graph(const std::string &name ) 
    : m_name(name)
{ }

// virtual
Graph::~Graph()
{
    clear();
}

void Graph::clear() {
    for (auto &e : m_edges) {
        if (e.second) {
            delete e.second;
        }
    }
    m_edges.clear();

    for (auto &n : m_nodes) {
        if (n.second) {
            delete n.second;
        }
    }
    m_nodes.clear();
}

bool Graph::add_node(Node *n) {
    if (m_nodes.find(n->name()) != m_nodes.end()) {
        return false;
    }
    m_nodes[n->name()] = n;
    return true;
}

bool Graph::remove_node(Node *n) {
    if (m_nodes.find(n->name()) == m_nodes.end()) {
        return false;
    }
    m_nodes.erase(n->name());
    return true;
}

size_t Graph::size_nodes() const {
    return m_nodes.size();
}


bool Graph::add_edge(Edge *e) {
    if (m_edges.find(e->name()) != m_edges.end()) {
        return false;
    }
    m_edges[e->name()] = e;
    return true;
}

bool Graph::remove_edge(Edge *e) {
    if (m_edges.find(e->name()) == m_edges.end()) {
        return false;
    }
    m_nodes.erase(e->name());
    return true;
}

std::string Graph::name() const {
    return m_name;
}

std::vector<Node*> Graph::all_nodes() const {
    std::vector<Node*> rslt(m_nodes.size());
    int i = 0;
    for (const auto& [ignored, value] : m_nodes) {
        rslt[i] = value;
        i++;
    }
    return rslt;
}

Node *Graph::find_node(const std::string &name) const {
    auto it = m_nodes.find(name);
    if (it != m_nodes.end()) {
        return (*it).second;
    } else {
        return nullptr;
    }
}

std::vector<Edge*> Graph::all_edges() const {
    std::vector<Edge*> rslt(m_edges.size());
    int i = 0;
    for (const auto& [ignored, value] : m_edges) {
        rslt[i] = value;
        i++;
    }
    return rslt;
}

std::vector<Edge*> Graph::all_src_edges(const Node *src) const {
    std::vector<Edge*> rslt;
    for (const auto& [ignored, value] : m_edges) {
        if (value->src_node() == src)
            rslt.push_back(value);
    }
    return rslt;
}

Edge *Graph::find_edge(const std::string &name) const {
    if (m_edges.find(name) != m_edges.end()) {
        return m_edges.at(name);
    } else {
        return nullptr;
    }
}

std::vector<Edge *> Graph::find_edges(Node *n1, Node *n2) const {
    std::vector<Edge *> rslt;
    for (auto &[ignored, value] : m_edges) {
        if (value->src_node() == n1 && value->dst_node() == n2) {
            rslt.push_back(value);
        }
    }
    return rslt;
}

Edge *Graph::find_shortest_edge(Node *n1, Node *n2) const {
    Edge *rslt = nullptr;
    for (auto &[ignored, value] : m_edges) {
        if (value->src_node() == n1 && value->dst_node() == n2) {
            if (rslt == nullptr) {
                rslt = value;
            } else if (value->weight() < rslt->weight()) {
                rslt = value;
            }
        }
    }
    return rslt;
}

size_t Graph::size_edges() const {
    return m_edges.size();
}

}; // end namespace ga