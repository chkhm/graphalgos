
#include <fstream>
#include "graph.hpp"
#include "errors.hpp"
#include "arg_parser.hpp"

arg::arg_descr a = 
{ 
    'h',
    "help",
    false,
    "",
    "prints this help text."
};

arg::arg_descr_set args = 
{
    {
        { 
            'h',
            "help",
            false,
            "",
            "prints this help text."
        },
        { 
            'p',
            "poop",
            true,
            "",
            "prints this poop text."
        }
    },
    "prints this help text."
};


int main(int argc, char **argv) {
    
    //std::cout << args.at("--help")->m_long_name << " " << args.at("--help")->m_short_name << std::endl;
    //std::cout << args.at("--poop")->m_long_name << " " << args.at("-p")->m_short_name << std::endl;
    //std::cout << args.help() << std::endl;

    collect_cmd_line_args(argc, argv, args);
    //std::cout << args.at("-p")->m_value << std::endl;

    std::cout << args << std::endl;
    exit(0);

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
    }
    std::cout << g << std::endl << std::endl;

    std::cout << "Shortest path from 'A' to 'D': " << std::flush;
    std::list<ga::Node*> path = ga::shortest_path(g.find_node("A"), g.find_node("D"), true);
    std::cout << path << std::endl;

    std::fstream f_out("test_graph.dot", std::ios::out);
    std::unordered_set<std::string> print_labels = {"name", "weight", "path" };
    to_graphviz(f_out, g, print_labels, set_use_color, set_use_style, set_landscape) << std::endl;

    return 0;
}