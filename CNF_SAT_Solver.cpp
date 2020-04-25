#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <list>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"


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

/*class Graph
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
}*/

/*std::list<int> Graph::shortestPath(int src, int dst) {
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
}*/

bool minVertexCover(int size, int k, std::vector<int> vertices) {

     std::vector<int> vCover;
     bool res;
     vCover.clear();
 
    //for(int k = 1; k <= size; k++){

    	Minisat::Solver result;
    	std::vector <std::vector<Minisat::Lit>> nkMatrix(size);
		Minisat::vec<Minisat::Lit> lits;
		Minisat::vec<Minisat::Lit> literals;
	
		for (int n = 0; n < size; n++) {
        	for (int j = 0; j < k; j++) {
            	Minisat::Lit literal = Minisat::mkLit(result.newVar());
            	nkMatrix[n].push_back(literal);
        	}
    	}

    //clause 1
    for(int i = 0; i < k; i++){
        for(int j = 0; j < size; j++){
            lits.push(nkMatrix[j][i]);
        }
        result.addClause(lits);
        lits.clear();

    }

    //clause 2
    for (int n = 0; n < size; n++){
        for(int i = 0; i < k; i++){
			for(int j = i + 1; j < k; j++){
            result.addClause(~nkMatrix[n][i],~nkMatrix[n][j]);
        	}
        }
    }

    //clause 3
    for (int i = 0; i < k; i++){
        for(int n = 0; n < size; n++){
                for(int j = n + 1; j < size; j++){
            		result.addClause(~nkMatrix[n][i],~nkMatrix[j][i]);
        	}
        }
    }

    //clause 4
    for(unsigned int i = 0; i < vertices.size(); i+=2){
        for(int j = 0; j < k; j++){
            literals.push(nkMatrix[vertices[i]][j]);
            literals.push(nkMatrix[vertices[i+1]][j]);
        }
        result.addClause(literals);
        literals.clear();
    }

    res = result.solve();

    if(res){
        for(int i = 0; i < size; i++){
            for(int j = 0; j < k; j++){
                if (Minisat::toInt(result.modelValue(nkMatrix[i][j])) == 0){
                	vCover.push_back(i);
                }
            }
        }
		std::sort(vCover.begin(), vCover.end());
    	for (unsigned int i = 0; i < vCover.size(); i++){
        	std::cout << vCover[i] << " ";
    	}
    	std::cout << std::endl;
    	return res;
    }
    //}
	return res;
    
}

int main() {
    //Graph g(0);

    while (!std::cin.eof()) {
        std::string input;
        std::vector<std::string> input_stream;
        std::vector<unsigned int> edgesList;
		std::vector<int> intVertices;
        int vSize;
        int flag;
		bool sol;

        std::getline(std::cin, input);
        input_stream = split(input, ' ');
        //std::cout << "My Input: " << input << std::endl;
        if (input_stream.size() == 2) {
            if (input_stream[0] == "V") {
                //if ((std::stoi(input_stream[1])) > 0) {
                    vSize = std::stoi(input_stream[1]);
                    //g = Graph(vSize);
//                    edgesList.clear();
                //} else {
                //    std::cout << "Error: V should be positive" << std::endl;
                //}
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
                            intVertices.push_back(std::stoi(temp_edge[i])); 							
                            intVertices.push_back(std::stoi(temp_edge[i+1]));
//                            std::cout << intVertices[i] << " " << intVertices[i+1] << std::endl;
                        }
                        else
                            continue;
//                        std::cout << edgesList[i];
                    }
				if(intVertices.size() > 0){
					for(int k = 1; k <= vSize; k++){
						sol = minVertexCover(vSize, k, intVertices);
						if(sol) break;
						else continue;
					}
				} else if(intVertices.size() == 0) {
					std::cout << std::endl;
				}

                } else {
                    std::cout << "Error: Length of edge is greater than a vertex";
                }
//                    std::cout << temp_edge[i] << std::endl;
            } else {
                std::cout << "Error: Invalid input" << std::endl;
            }
        } /*else if (input_stream.size() == 3 && input_stream[0] == "s") {
                vertexCover(input_stream);
//            std::cout << "Shortest Path";
        }*/ /*else {
            std::cout << "Error: Invalid input" << std::endl;
        }*/
    }

    return 0;
}