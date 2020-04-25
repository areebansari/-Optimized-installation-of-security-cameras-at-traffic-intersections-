// Compile with c++ a2ece650cpp -std=c++11 -o a2ece650
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <list>

std::vector<std::string> split(std::string str, char delim){
    std::string buffer;                 // buffer string
    std::stringstream ss(str);       // passing string into a stream

    std::vector<std::string> out; // vector to store string splits

    while (getline(ss, buffer, delim)) {
        out.push_back(buffer);
    }
    return out;
}

std::string replaceChar(std::string str, const std::string& replace, char ch) {
    size_t i = str.find_first_of(replace);

    while (i != std::string::npos) {
        str[i] = ch;
        i = str.find_first_of(replace, i + 1);
    }

    return str;
}

int validVertice(std::vector<std::string> str, int size) {
    int valid;
    for (unsigned int i=0; i<str.size(); ++i) {
        if((std::stoi(str[i])) >= size)
            return valid = 0;
        else
            continue;
    }
    return valid = 1;
}

class Graph
{
    int V;
public:
    std::list<int> *adj;
    Graph(int V);
    void addEdge(int v, int w);
    std::list<int> shortestPath(int src, int dst);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new std::list<int> [V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

std::list<int> Graph::shortestPath(int src, int dst) {
    std::list<int> result;
    std::list<int> path;
    std::list<int> new_path;
    std::list<int> neighbours;
    bool *visited = new bool[V];
    std::vector<std::list<int> > queue;

    for (int i = 0; i < V; i++)
        visited[i] = false;

//    std::cout << src << " " << dst << std::endl;
//    std::cout << "size adj: " << adj->size() << std::endl;
//    for (auto i=adj->begin(); i!=adj->end(); ++i)
//        std::cout << " " << *i;

    path.push_back(src);

    if (src == dst)
        return result;

    queue.push_back(path);

    int node;
    int curr_neighbour;

    while (!queue.empty())
    {
//        std::cout << "in while queue" << std::endl;
        path = queue.front();
        queue.erase(queue.begin());

        node = path.back();

        if (!visited[node]) {
            neighbours = adj[node];
//            std::cout << "in visited[done]" << std::endl;
            for (auto& neighbour : neighbours) {
//                std::cout << "in for auto neighbour" << std::endl;
                new_path = path;
                curr_neighbour = neighbour;
                new_path.push_back(curr_neighbour);
                queue.push_back(new_path);

                if (curr_neighbour == dst) {
//                    std::cout << "in curr neighbour" << std::endl;
                    return new_path;
                }
            }
            visited[node] = 1;
        }
    }
    return result;
}

void plotGraph(std::vector<std::string> inputs, Graph g) {
    std::list<int> result_path;
    std::list<int>::const_iterator iterator;

    int src;
    int dst;

    src = std::stoi(inputs[1]);
    dst = std::stoi(inputs[2]);
    result_path = g.shortestPath(src, dst);

    std::string res;

    std::vector<int> v{ std::begin(result_path), std::end(result_path) };

    for(unsigned i = 0; i < v.size(); ++i)
    {
        if(i != 0)
            res.append("-");
        res.append(std::to_string(v[i]));
    }


    if (!result_path.empty()) {
        std::cout << res << std::endl;
    }else{
        std::cout << "Error: Either path or vertex does not exist." << std::endl;
    }
}

int main() {
    Graph g(0);

    while (!std::cin.eof()) {
        std::string input;
        std::vector<std::string> input_stream;
        std::vector<unsigned int> edgesList;
        int vSize;
        int flag;

        std::getline(std::cin, input);
        input_stream = split(input, ' ');
        //std::cout << "My Input: " << input << std::endl;
        if (input_stream.size() == 2) {
            if (input_stream[0] == "V") {
                if ((std::stoi(input_stream[1])) > 0) {
                    vSize = std::stoi(input_stream[1]);
                    g = Graph(vSize);
//                    edgesList.clear();
                } else {
                    std::cout << "Error: V should be positive" << std::endl;
                }
            } else if (input_stream[0] == "E") {
                std::string temp;
                temp = replaceChar(input_stream[1], "<", ' ');
                temp = replaceChar(temp, ">", ' ');
                temp = replaceChar(temp, "{", ' ');
                temp = replaceChar(temp, "}", ' ');
                temp.erase(remove_if(temp.begin(), temp.end(), isspace), temp.end());
                std::vector<std::string> temp_edge = split(temp, ',');
                flag = validVertice(temp_edge, vSize);
                if (flag) {
                    for (unsigned int i = 0; i < temp_edge.size(); ++i) {
                        if (i%2==0) {
                            g.addEdge(std::stoi(temp_edge[i]), std::stoi(temp_edge[i + 1]));
//                            std::cout << temp_edge[i] << " " << temp_edge[i+1] << std::endl;
                        }
                        else
                            continue;
//                        std::cout << edgesList[i];
                    }
                } else {
                    std::cout << "Error: Length of edge is greater than a vertex";
                }
//                    std::cout << temp_edge[i] << std::endl;
            } else {
                std::cout << "Error: Invalid input" << std::endl;
            }
        } else if (input_stream.size() == 3 && input_stream[0] == "s") {
                plotGraph(input_stream, g);
//            std::cout << "Shortest Path";
        } else {
            std::cout << "Error: Invalid input" << std::endl;
        }
    }

    return 0;
}

