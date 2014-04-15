#ifndef GRAPH_H
#define	GRAPH_H

#include <cstdlib>
#include <fstream>
#include <map>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <cstdio>
#include <cstring>
#include <deque>
#include <list>
#include <queue>
#include <boost/lexical_cast.hpp>

// This is the JSON header
#include "jsoncpp/json.h"
#include "jsoncpp/value.h"

#include "Character.h"
#include "Node.h"
#include "Route.h"

using namespace std;

class Graph {
    private:
        const static int MAX_DEPTH = 27;
    public:
        Graph();

        void execute();
        void connect();
        void read_data();
        void init_nodes();
        void edit_nodes();
        void mark_nodes();
        void init_characters();

        string get_strategy(bool&);
        Node get_node_by_id(long int);
        Node get_start_point(bool&);
        Node get_end_point(bool&);

        void get_optimized_path(string option, bool&, Node, Node);
        void get_available_points(Route route, bool&);

        int bfs(Node _start_node, Node _end_node, queue<Node> _queue_nodes, Route &shortest_route);
        void dfs(Node _start_node, Node _end_node, vector<Node> &_visited_nodes, int max_depth);

        void write_vector_nodes(vector<Node> nodes, const char* filename);
        void write_data(string);

        void display_route(Route, unsigned int);
        void display_queue(queue<Node> route);
        void display_route_info(Route route);

        int get_int_from_string(string str);
        string get_xxx_string(string str);
        string split_string(string str);
        bool is_in_vector_string(vector<string> visited, string value);
        bool is_in_stack_nodes(stack<Node> _stack_nodes, Node _check_node);
        bool is_in_queue_nodes(queue<Node> _queue_nodes, Node _check_node);
        bool is_in_vector_nodes(vector<Node> visited, Node value);
        void check_exit(string, bool&);
        bool is_number(const std::string& s);
        void clear_data();
};

#endif
