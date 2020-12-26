
#include <gtest/gtest.h>
#include <fstream>
#include "graph.hpp"
#include "errors.hpp"

TEST(GraphTest, SimpleGraphShortestPath) {

    ga::Graph::pretty_print_flag(true);
    bool set_connected = false;
    bool set_use_color = true;
    bool set_use_style = true;
    bool set_show_labels = true;
    bool set_landscape = false;
    
    // ga::Graph *g = ga::random_graph(100, 150, set_connected, set_use_color, set_use_style);

    ga::Graph g;
    std::fstream f("test_graph.json");
    try {
        f >> g;
    } catch (ga::graph_exception e) {
        std::cerr << e.what() << std::endl;
        throw e;
    }
    std::cout << g << std::endl << std::endl;

    std::cout << "Shortest path from 'A' to 'D': " << std::flush;
    std::list<ga::Node*> path = ga::shortest_path(g.find_node("A"), g.find_node("D"), true);
    std::cout << path << std::endl;

    std::fstream f_out("test_graph.dot", std::ios::out);
    std::unordered_set<std::string> print_labels = {"name", "weight", "path" };
    to_graphviz(f_out, g, print_labels, set_use_color, set_use_style, set_landscape) << std::endl;

}