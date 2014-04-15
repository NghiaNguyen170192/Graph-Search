
Route Graph::get_max_strength_path(Route shortest_route){

    stack<Node> str_route;
    vector<Node> temp_route;

    stack<Node> test = shortest_route.get_stack_nodes();
    while (!test.empty()){
        temp_route.push_back(test.top());
        test.pop();
    }

    for (unsigned int i = 0; i < temp_route.size(); i++){

        for (unsigned int j = 0; j < temp_route[i].get_connected_nodes().size(); j++){

            // if not the next node
            if (temp_route[i].get_connected_nodes()[j] != temp_route[i + 1].get_skill_id()){

                Node node_to_check = get_node_by_id(temp_route[i].get_connected_nodes()[j]);

                // if str is higher
                if (node_to_check.get_str() > temp_route[i].get_str()){
                    //cout << node_to_check.get_skill_name() << endl;

                    for (unsigned int k = 0; k < node_to_check.get_connected_nodes().size(); k++){
                        // check if node can go to next node
                        if (node_to_check.get_connected_nodes()[k] == temp_route[i + 1].get_skill_id()){

                            if (i != 0) {
                                // check if prev node can go to this node
                                for (unsigned int z = 0; z < temp_route[i - 1].get_connected_nodes().size(); z++){
                                    if (temp_route[i - 1].get_connected_nodes()[z] == node_to_check.get_skill_id())
                                        temp_route.at(i) = node_to_check;
                                }
                            }

                        }
                    }


                }

            }

        }

    }

    for (unsigned int i = 0; i < temp_route.size(); i++){
        str_route.push(temp_route[i]);
    }
    Route route = Route(str_route);
    this->display_route(route);
    return route;
}


int Graph::dfs(Node _start_node, Node _end_node, stack<Node> _visited_nodes){

    marked.clear();
    this->mark_nodes();

    stack<Node> temp_route;
    map<long int, long int> prev;

    for (unsigned int i = 0; i < _start_node.get_connected_nodes().size(); i++){
        _visited_nodes.push(get_node_by_id(_start_node.get_connected_nodes()[i]));
    }

    while (!_visited_nodes.empty()){
        Node u = _visited_nodes.top();
        _visited_nodes.pop();

        for (unsigned int i = 0; i < u.get_connected_nodes().size(); i++){
            Node w = get_node_by_id(u.get_connected_nodes()[i]);

            if (marked.at(w.get_skill_id()) == false){
                marked.at(w.get_skill_id()) = true;
                prev.insert(std::pair<long int, long int>(w.get_skill_id(), u.get_skill_id()));
                _visited_nodes.push(w);

            }

            if (w.get_skill_id() == _end_node.get_skill_id()){
                Node _curr_node = _end_node;
                while (prev[_curr_node.get_skill_id()] != _start_node.get_skill_id()){
                    temp_route.push(_curr_node);
                    _curr_node = get_node_by_id(prev[_curr_node.get_skill_id()]);

                }

                temp_route.push(_curr_node);
                temp_route.push(_start_node);

                Route route = Route(temp_route);

                this->display_route(route);
                routes.push_back(route);
                prev.clear();

            }

        }
    }

    return 0;
}


int Graph::ucs(Node _start_node, Node _end_node, queue<Node> _queue_nodes){

    marked.clear();
    this->mark_nodes();

    Node _curr_node = _end_node;
    stack<Node> temp_route;
    map<long int, long int> prev;
    marked.at(_start_node.get_skill_id()) = true;

    _queue_nodes.push(_start_node);
    Node node;

    while (!_queue_nodes.empty()) {

        node = _queue_nodes.front();
        _queue_nodes.pop();

        if (node.get_skill_id() == _end_node.get_skill_id()){

            while (prev[_curr_node.get_skill_id()] != _start_node.get_skill_id()){
                temp_route.push(_curr_node);
                _curr_node = get_node_by_id(prev[_curr_node.get_skill_id()]);
            }

            temp_route.push(_curr_node);
            temp_route.push(_start_node);

            Route shortest_route = Route(temp_route);

            this->display_route(shortest_route);
            return node.get_skill_id();
        }

        marked.at(node.get_skill_id()) = true;

        for (unsigned int i = 0; i < node.get_connected_nodes().size(); i++){

            Node n = get_node_by_id(node.get_connected_nodes()[i]);
            if (marked.at(n.get_skill_id()) == false){

                if(!is_in_queue_nodes(_queue_nodes, n)){
                    prev.insert(std::pair<long int, long int>(n.get_skill_id(), node.get_skill_id()));
                    _queue_nodes.push(n);

                } else if (n.get_str() > node.get_str()){
                    node = n;
                }
            }
        }
    }

    return -1;
}

//size_t write_callback(char* buf, size_t size, size_t nmemb, void* up) {
//    //callback must have this declaration
//    //buf is a pointer to the data that curl has for us
//    //size*nmemb is the size of the buffer
//    for (unsigned int c = 0; c < size * nmemb; c++)
//    {
//        temp_data.push_back(buf[c]);
//    }
//    return size * nmemb; //tell curl how many bytes we handled
//}

void Graph::connect(){

//    CURL* curl;

//    curl = curl_easy_init();

//    curl_easy_setopt(curl, CURLOPT_URL, "http://www.pathofexile.com/passive-skill-tree");
//    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);

//    curl_easy_perform(curl);
//    write_data(temp_data);


//    curl_easy_cleanup(curl);
//    curl_global_cleanup();

}
