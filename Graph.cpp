#include "Graph.h"
#include <iostream>
#include <string>

using namespace std;

Json::Value root;
Json::Reader reader;

vector<Character> characters;
vector<Node> nodes;
vector<Route> routes;

stack<Node> visited_nodes;
stack<Node> stack_nodes;

queue<Node> queue_nodes;

map<long int, bool> marked;

size_t write_call_back(void *contents, size_t size, size_t nmemb, void *userp) {

    flush(cout);
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Graph::Graph(){
}

void Graph::execute(){
    bool is_exit = false;

    connect();
    read_data();
    init_nodes();
    init_characters();
    do {
        cout << "Character selection. 'x' to exit" << endl;
        Node start =  get_start_point(is_exit);
        cout << "Output points: ";
        for (unsigned int i = 0; i < start.get_connected_nodes().size(); i++)
            cout << start.get_connected_nodes()[i] << " ";
        cout << endl;

        Node end = get_end_point(is_exit);
        string option = get_strategy(is_exit);
        this->get_optimized_path(option, is_exit, start, end);

    } while (!is_exit);

    this->clear_data();
}

void Graph::connect(){

    cout << "Connecting to host! Please wait!..." << endl;
        CURL *curl;
        CURLcode res;
        string buffer;

        curl = curl_easy_init();

        if (curl){
            curl_easy_setopt(curl, CURLOPT_URL, "http://www.pathofexile.com/passive-skill-tree");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_call_back);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK){
                cerr<< "performance failed"<< curl_easy_strerror(res) << endl;
            }
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            write_data(buffer);
        }

}

void Graph::init_characters(){
    Json::Value array2 = root["root"];
    for(unsigned int index = 0; index < array2.size() - 1; index++) {
        for (unsigned int i = 0; i < nodes.size(); i++){
            if (array2["out"][index].asInt() == nodes[i].get_skill_id()){
                Character temp_char = Character(nodes[i].get_skill_name(), nodes[i].get_skill_id(), nodes[i].get_connected_nodes());
                characters.push_back(temp_char);
            }
        }
    }
}

void Graph::init_nodes(){
    vector<string> temp_skills;
    vector<long int> temp_nodes;

    const Json::Value array = root["nodes"];

    for(unsigned int index = 0; index < array.size(); index++) {

        Json::Value skill_id = array[index]["id"];
        Json::Value skill_name = array[index]["dn"];
        Json::Value skill_effect = array[index]["sd"];
        Json::Value connected_nodes = array[index]["out"];

        for(unsigned int i = 0; i < connected_nodes.size(); i++) {
            temp_nodes.push_back(connected_nodes[i].asInt());
         }

        for(unsigned int i = 0; i < skill_effect.size(); i++) {
            temp_skills.push_back(skill_effect[i].asString());
        }

        Node temp_node = Node(skill_id.asInt(), skill_name.asString(), 0, 0, 0, temp_skills, temp_nodes);
        nodes.push_back(temp_node);

        temp_skills.clear();
        temp_nodes.clear();
    }

    edit_nodes();
    mark_nodes();
    write_vector_nodes(nodes, "nodes.txt");

}

void Graph::mark_nodes(){
    for (unsigned int i = 0; i < nodes.size(); i++)
        marked.insert(std::pair<long int, bool>(nodes[i].get_skill_id(), false));
}

void Graph::edit_nodes(){
    bool checked;

    for (unsigned int i = 0; i < nodes.size(); i++){

        for (unsigned int j = 0; j < nodes[i].get_connected_nodes().size(); j++){

            Node node_to_edit = get_node_by_id(nodes[i].get_connected_nodes()[j]);
            checked = false;

            for (unsigned int k = 0; k < node_to_edit.get_connected_nodes().size(); k++){

                if (nodes[i].get_skill_id() == node_to_edit.get_connected_nodes()[k]){
                    checked = true;
                    break;
                }

            }

            if (checked == false){

                for (unsigned int z = 0; z < nodes.size(); z++){

                    if (nodes[z].get_skill_id() == node_to_edit.get_skill_id()){
                        nodes[z].add_connected_node(nodes[i].get_skill_id());
                        break;
                    }

                }

            }

        }

        for (unsigned int j = 0; j < nodes[i].get_skill_effect().size(); j++){

            unsigned int found_str = nodes[i].get_skill_effect()[j].find("to Strength");
            unsigned int found_dex = nodes[i].get_skill_effect()[j].find("to Dexterity");
            unsigned int found_intel = nodes[i].get_skill_effect()[j].find("to Intelligence");

            if (found_str < nodes[i].get_skill_effect()[j].length()) {
                nodes[i].set_str(nodes[i].get_str() + boost::lexical_cast<int>(nodes[i].get_skill_effect()[j].substr(1, 2)));
            }

            if (found_dex < nodes[i].get_skill_effect()[j].length()) {
                nodes[i].set_dex(nodes[i].get_dex() + boost::lexical_cast<int>(nodes[i].get_skill_effect()[j].substr(1, 2)));
            }

            if (found_intel < nodes[i].get_skill_effect()[j].length()) {
                nodes[i].set_intel(nodes[i].get_intel() + boost::lexical_cast<int>(nodes[i].get_skill_effect()[j].substr(1, 2)));
            }

        }

    }

}

void Graph::read_data(){

    ifstream file("data.json");
    bool parsedSuccess = reader.parse(file,
                                   root,
                                   false);

    if(not parsedSuccess) {

        cout << "Failed to parse JSON" << endl
             << reader.getFormatedErrorMessages()
             << endl;
        exit(1);
    }

}

string Graph::get_strategy(bool &is_exit){
    string in_string;
    bool checked = false;
    do {
        cout << "1. Shortest Path (type 'sp')\n";
        cout << "2. Optimize Strength (type 'os')\n";
        cout << "3. Optimize Dexterity (type 'od')\n";
        cout << "4. Optimize Intelligence (type 'oi')\n";
        cout << "Strategy?> ";
        cin >> in_string;
        this->check_exit(in_string, is_exit);
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        if (in_string != "sp" && in_string != "os" && in_string != "od" && in_string != "oi"){
            cout << "Invalid Option\n";
        } else {
            checked = true;
        }

    } while (!checked)
        ;

    return in_string;
}

Node Graph::get_node_by_id(long int _id){
    vector<string> temp_skills;
    vector<long int> temp_nodes;

    Node temp = Node(0, "", 0, 0, 0, temp_skills, temp_nodes);
    for (unsigned int i = 0; i < nodes.size(); i++){
        if (_id == nodes[i].get_skill_id()){
            return nodes[i];
        }
    }
    return temp;
}

Node Graph::get_start_point(bool &is_exit){
    Node temp;
    std::string start = "";
    bool checked = false;

    for (unsigned int i = 0; i < characters.size(); i++){
        cout << "[" << characters[i].get_char_name() << "] ";
    }

    do {
        cout << "\nCharacter? > ";
        std::cin >> start;
        this->check_exit(start, is_exit);
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::transform(start.begin(), start.end(), start.begin(), ::toupper);

        for (unsigned int i = 0; i < characters.size(); i++){
            if (start == characters[i].get_char_name()){
                checked = true;
                return get_node_by_id(characters[i].get_char_id());
            }
        }
        cout << "Invalid Character Name";

    } while(checked == false);

    return temp;
}

Node Graph::get_end_point(bool &is_exit){
    long int temp_id;
    string temp_str;
    bool checked = false;

    do {

        cout << "End Node ID?> ";
        cin >> temp_str;
        this->check_exit(temp_str, is_exit);
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        if (!is_number(temp_str)){
            checked = false;
            cout << "Invalid ID!\n";
        } else {
            temp_id = boost::lexical_cast<int>(temp_str);
            Node end = get_node_by_id(temp_id);
            if (end.get_skill_id() == 0) {
                checked = false;
                cout << "Invalid Node\n";
            } else {
                checked = true;
            }

        }

    } while (checked == false);

    return get_node_by_id(temp_id);
}

void Graph::get_optimized_path(string option, bool &is_exit, Node _start_node, Node _end_node){

    Route max_route;

    if (option == "sp") {
        cout << "Please wait...\n";
        this->bfs(_start_node, _end_node, queue_nodes, max_route);

    } else if (option == "os"){

        routes.clear();
        vector<Node> _visited_nodes;
        _visited_nodes.push_back(_start_node);
        cout << "Please wait...\n";
        this->dfs(_start_node, _end_node, _visited_nodes, MAX_DEPTH);

        max_route = routes[0];
        for (unsigned int i = 0; i < routes.size(); i++){
            if (max_route.get_total_str() < routes[i].get_total_str())
                max_route = routes[i];
        }

    } else if (option == "od"){

        routes.clear();
        vector<Node> _visited_nodes;
        _visited_nodes.push_back(_start_node);
        cout << "Please wait...\n";
        this->dfs(_start_node, _end_node, _visited_nodes, MAX_DEPTH);

        max_route = routes[0];
        for (unsigned int i = 0; i < routes.size(); i++){
            if (max_route.get_total_dex() < routes[i].get_total_dex())
                max_route = routes[i];
        }

    } else if (option == "oi"){

        routes.clear();
        vector<Node> _visited_nodes;
        _visited_nodes.push_back(_start_node);
        cout << "Please wait...\n";
        this->dfs(_start_node, _end_node, _visited_nodes, MAX_DEPTH);

        max_route = routes[0];
        for (unsigned int i = 0; i < routes.size(); i++){
            if (max_route.get_total_intel() < routes[i].get_total_intel())
                max_route = routes[i];
        }

    }

    this->get_available_points(max_route, is_exit);

}

void Graph::get_available_points(Route route, bool &is_exit){

    long int num_points;
    string temp_str;
    bool checked = false;

    do {

        cout << "Available Points?> ";
        cin >> temp_str;
        this->check_exit(temp_str, is_exit);
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        if (!is_number(temp_str)){
            checked = false;
            cout << "Invalid Number!\n";
        } else {
            num_points = boost::lexical_cast<int>(temp_str);
            checked = true;
        }

    } while (checked == false);

    cout << endl;

    if (route.get_num_steps() <= num_points){
        this->display_route(route, route.get_stack_nodes().size());
        this->display_route_info(route);
    } else if (route.get_num_steps() > num_points) {
        this->display_route(route, num_points);
    }

}

void Graph::display_route_info(Route route){
    int total = 0;
    stack<Node> stack_n = route.get_stack_nodes();
    vector<Node> vector_n;
    vector<string> checked;

    while(!stack_n.empty()){
        vector_n.push_back(stack_n.top());
        stack_n.pop();
    }

    cout << "ROUTE REPORT: " << endl;
    for (unsigned int i = 0; i < vector_n.size(); i++){
        vector<string> skill_effect = vector_n[i].get_skill_effect();

        for (unsigned int j = 0; j < skill_effect.size(); j++){
            total = 0;

            for (unsigned int k = 0; k < vector_n.size(); k++){

                for (unsigned int z = 0; z < vector_n[k].get_skill_effect().size(); z++){

                    if (get_xxx_string(skill_effect[j]) == get_xxx_string(vector_n[k].get_skill_effect()[z])
                            && !is_in_vector_string(checked, get_xxx_string(vector_n[k].get_skill_effect()[z]))){
                        total += get_int_from_string(vector_n[k].get_skill_effect()[z]);
                    }

                }

            }

            checked.push_back(get_xxx_string(skill_effect[j]));

            if (total > 0){
                cout << "+" << total;
                if(skill_effect[j].find("%") <= skill_effect[j].length())
                    cout << "% ";
                else
                    cout << " ";
                cout << split_string(get_xxx_string(skill_effect[j]));
                cout << endl;
            }
        }

    }
    cout << endl;

}

string Graph::split_string(string str){

    string str_to_find = "xxx";
    string final = "";

    if(str.find("%") <= str.length()){
        int pos_start = str.find("%") + 2;
        int pos_end = str.length();
        final = str.substr(pos_start, pos_end - pos_start);

    } else {
        int pos_start = str.find(str_to_find) + 4;
        int pos_end = str.length();
        final = str.substr(pos_start, pos_end - pos_start);
    }

    return final;
}

string Graph::get_xxx_string(string skill){

    string final = "";
    vector<char> vector_char(skill.begin(), skill.end());
    vector<char> num_vector;
    const char * c;
    string num_str;

    for (unsigned int i = 0; i < vector_char.size(); i++){
        if(isdigit(vector_char[i]) || vector_char[i] == '.')
            num_vector.push_back(vector_char[i]);
    }
    num_vector.push_back('\0');
    c = &num_vector[0];
    num_str = string(c);

    final += skill.replace(skill.find(num_str), num_str.length(), "xxx");

    return final;
}

int Graph::get_int_from_string(string str){
    int num_int = 0;
    vector<char> vector_char(str.begin(), str.end());
    vector<char> num_vector;
    const char * c;
    string num_str;

    for (unsigned int i = 0; i < vector_char.size(); i++){
        if(isdigit(vector_char[i]) || vector_char[i] == '.')
            num_vector.push_back(vector_char[i]);
    }

    num_vector.push_back('\0');
    c = &num_vector[0];
    num_str = string(c);

    try {
    num_int = boost::lexical_cast<int>(num_str);
    } catch (boost::bad_lexical_cast const&){
        cout << str << endl;
    }

    return num_int;
}

void Graph::display_route(Route route, unsigned int max_nodes){
    unsigned int count = 0;
    stack<Node> temp_stack = route.get_stack_nodes();

    if (route.get_stack_nodes().size() <= max_nodes){
        cout << "Steps: " << route.get_num_steps() << endl;
    } else {
        cout << "Steps: " << max_nodes << endl;
    }
    if (max_nodes > 0){

        while (!temp_stack.empty() && count < max_nodes + 1){

            Node temp_node = temp_stack.top();
            cout << temp_node.get_skill_id() << " - " << temp_node.get_skill_name() << endl;

            temp_stack.pop();
            count++;
        }
        cout << endl;

    }

    if (temp_stack.empty()){
        /*
        cout << "Total Strength: " << "+" << route.get_total_str() << endl;
        cout << "Total Dexterity: " << "+" << route.get_total_dex() << endl;
        cout << "Total Intelligence: " << "+" << route.get_total_intel() << endl;
        */
    } else {
        cout << temp_stack.size() << " more points to end node (incl. end node)\n";
    }

}

void Graph::display_queue(queue<Node> route){

    while (!route.empty()){

        Node temp_node = route.front();
        cout << temp_node.get_skill_id() << " - " << temp_node.get_skill_name();

        if (route.size() > 1)
            cout  << endl;
        route.pop();

    }
    cout << endl << endl;
}

void Graph::dfs(Node _start_node, Node _end_node, vector<Node> &_visited_nodes, int max_depth){

    if (max_depth >= 0){

        Node node_s = _visited_nodes.back();
        stack<Node> temp_stack;
        stack<Node> stack_to_push;

        for (unsigned int i = 0; i < node_s.get_connected_nodes().size(); i++){

            Node node = get_node_by_id(node_s.get_connected_nodes()[i]);
            if (is_in_vector_nodes(_visited_nodes, node)){
                continue;
            }

            if (node.get_skill_id() == _end_node.get_skill_id()){
                _visited_nodes.push_back(node);
                unsigned int hops = _visited_nodes.size();

                for (unsigned int j = 0; j < hops; j++){
                    temp_stack.push(_visited_nodes[j]);
                }

                while (!temp_stack.empty()){
                    stack_to_push.push(temp_stack.top());
                    temp_stack.pop();
                }

                int n = (int) _visited_nodes.size() - 1;
                Route test = Route(stack_to_push);

                routes.push_back(test);
                _visited_nodes.erase(_visited_nodes.begin() + n);

                break;
            }
        }

        for (unsigned int i = 0; i < node_s.get_connected_nodes().size(); i++){

            Node node = get_node_by_id(node_s.get_connected_nodes()[i]);
            if (is_in_vector_nodes(_visited_nodes, node) || node.get_skill_id() == _end_node.get_skill_id()){
                continue;
            }
            _visited_nodes.push_back(node);
            dfs(_start_node, _end_node, _visited_nodes, max_depth - 1);
            int n = (int) _visited_nodes.size() - 1;
            _visited_nodes.erase(_visited_nodes.begin() + n);

        }
    }

}


int Graph::bfs(Node _start_node, Node _end_node, queue<Node> _queue_nodes, Route &shortest_route){
    int count = 0;
    marked.clear();
    this->mark_nodes();

    Node _curr_node = _end_node;
    map<long int, long int> prev;

    _queue_nodes.push(_start_node);
    marked.at(_start_node.get_skill_id()) = true;

    while (!_queue_nodes.empty()) {
        Node t = _queue_nodes.front();
        _queue_nodes.pop();
        count++;
        if (t.get_skill_id() == _end_node.get_skill_id()){
            stack<Node> temp_route;
            while (prev[_curr_node.get_skill_id()] != _start_node.get_skill_id()){
                temp_route.push(_curr_node);
                _curr_node = get_node_by_id(prev[_curr_node.get_skill_id()]);
            }

            temp_route.push(_curr_node);
            temp_route.push(_start_node);
            shortest_route = Route(temp_route);

            return t.get_skill_id();
        }

        for (unsigned int i = 0; i < t.get_connected_nodes().size(); i++){
            Node u = get_node_by_id(t.get_connected_nodes()[i]);
            if (marked.at(u.get_skill_id()) == false){
                marked.at(u.get_skill_id()) = true;
                prev.insert(std::pair<long int, long int>(u.get_skill_id(), t.get_skill_id()));
                _queue_nodes.push(u);
            }
        }
    }
    return -1;
}

bool Graph::is_in_queue_nodes(queue<Node> _queue_nodes, Node _check_node){
    while (!_queue_nodes.empty()){
        if (_queue_nodes.front().get_skill_id() == _check_node.get_skill_id())
            return true;
        else
            _queue_nodes.pop();
    }
    return false;
}

bool Graph::is_in_stack_nodes(stack<Node> _stack_nodes, Node _check_node){
    while (!_stack_nodes.empty()){
        if (_stack_nodes.top().get_skill_id() == _check_node.get_skill_id())
            return true;
        else
            _stack_nodes.pop();
    }
    return false;
}

bool Graph::is_in_vector_nodes(vector<Node> visited, Node value) {
    for (unsigned int i = 0; i < visited.size(); i++){
        if (value.get_skill_id() == visited[i].get_skill_id())
            return true;
    }
    return false;
}

bool Graph::is_in_vector_string(vector<string> visited, string value) {
    for (unsigned int i = 0; i < visited.size(); i++){
        if (value == visited[i])
            return true;
    }
    return false;
}

void Graph::write_vector_nodes(vector<Node> nodes, const char* filename){

    ofstream outStream;
    outStream.open(filename);

    for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it){
        outStream << "id: " << (*it).get_skill_id() << endl
             << " name: " << (*it).get_skill_name() << endl
             << " strength: " << (*it).get_str() << endl
             << " dexterity: " << (*it).get_dex() << endl
             << " intelligence: " << (*it).get_intel() << endl
             << " effect: [";

        for(unsigned int i = 0; i < (*it).get_skill_effect().size(); i++){
            outStream << (*it).get_skill_effect()[i];
            if (i < (*it).get_skill_effect().size() - 1){
                outStream << ", ";
            }
        }
        outStream << "] " << endl;

        outStream << " nodes: [";

        for (unsigned int i = 0; i < (*it).get_connected_nodes().size(); i++){
            outStream << (*it).get_connected_nodes()[i];
            if (i < (*it).get_connected_nodes().size() - 1){
                outStream << ", ";
            }
        }

        outStream << "] " << endl << endl;
    }
    outStream.close();
}

void Graph::write_data(string in_string){
    string temp_string = "";

    string start = "characterData";
    string end = "[0.1726,0.2429,0.3132,0.3835]}";

    unsigned int pos_start = in_string.find(start) - 1;
    unsigned int pos_end = in_string.find(end) + end.length();

    temp_string = in_string.substr(pos_start, pos_end - pos_start);
    temp_string = "{" + temp_string + ",";
    ofstream outStream;
    outStream.open("data.json");
    outStream << temp_string;
    outStream.close();
}

void Graph::check_exit(string input, bool &is_exit){
    if (input == "x") {
        is_exit = true;
        exit(1);
    }
}

bool Graph::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void Graph::clear_data(){
    root.clear();

    vector<Character>().swap(characters);
    vector<Node>().swap(nodes);

    while (!visited_nodes.empty()){
        visited_nodes.pop();
    }

    while (!stack_nodes.empty()){
        stack_nodes.pop();
    }

    vector<Route>().swap(routes);

    while(!queue_nodes.empty()){
        queue_nodes.pop();
    }
    marked.clear();
}
