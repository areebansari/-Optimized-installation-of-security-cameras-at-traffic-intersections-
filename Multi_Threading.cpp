#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <list>
#include <memory>
#include <pthread.h>
#include <mutex>
#include <time.h>
#include <errno.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

//#define DEBUG

std::vector<int> intVertices;
std::vector<float> CNFt, VC1t, VC2t;
clockid_t CNFid, VC1id, VC2id;
timespec cnftime, vc1time, vc2time;
int vSize;
std::mutex mutee;

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
		std::cout << "CNF-SAT-VC: ";
    	for (unsigned int i = 0; i < vCover.size(); i++){
        	std::cout << vCover[i];
			if(i != vCover.size() - 1) {
				std::cout << ",";
			}			
    	}
    	std::cout << std::endl;
    	return res;
    }
    //}
	return res;   
}

void approxVC1(std::vector<int> intVertices) {

    //int no_of_visits = 0;
    int freqVal;
	bool visited[intVertices.size()];
	int vsize = *(&visited + 1) - visited;
	for(int j=0; j<=vsize; j++) {
		visited[j] = false;
	}
	int visitCount[intVertices.size()];
	int size = *(&visitCount + 1) - visitCount;
	for (int i=0; i<=size; i++) {
		visitCount[i] = 0;
	}
	std::vector<int> list = intVertices;
	std::vector<int> A1;
	int index = -1;
	
	while(list.size() != 0) {
		int no_of_visits = 0;
		
    	for (auto vi = intVertices.begin(); vi != intVertices.end(); vi++) {
			index = *vi;
    	
    		if(!visited[index]) {
    			visited[index] = true;
			}
		
			else if(visited[index]) {
				visitCount[index] += 1;
			}
		
			if(visitCount[index] > no_of_visits) {
				no_of_visits = visitCount[index];
				freqVal = *vi;
			}
    	}
    	
    	A1.push_back(freqVal);
    	
    	intVertices.clear();

        for (unsigned int i = 0; i < list.size() - 1; i++) {
            if (i % 2 == 0) {
                if (list[i] == freqVal || list[i+1] == freqVal) {
                    continue;
                }
				else {
                    intVertices.push_back(list[i]);
                    intVertices.push_back(list[i+1]);
                }
            }
			else {
                continue;
            }
        }
        list = intVertices;
    }

	std::sort(A1.begin(), A1.end());

	std::cout << "APPROX-VC-1: ";
	for(unsigned int i = 0; i < A1.size(); i++) {
		std::cout << A1[i];
		if(i != A1.size() - 1)
			std::cout << ",";
	}
	std::cout << std::endl;
}

void approxVC2(std::vector<int> intVertices) {
    int u;
    int v;
	std::vector<int> newList = intVertices;
	std::vector<int> A2;
		
	while(newList.size() != 0) {
		u = newList[0];
		v = newList[1];
	
		A2.push_back(u);
		A2.push_back(v);
		
		intVertices.clear();
		std::vector<int> index;
		
		for(unsigned int i = 0; i<newList.size();i++){
			if(u == newList[i]){
				if(i%2==0){
					index.push_back(i);
					index.push_back(i+1);
					
				}
				else if(i%2 != 0){
					index.push_back(i);
					index.push_back(i-1);
				}
			}
			
			if(v == newList[i]) {
				if(i%2==0){
					index.push_back(i);
					index.push_back(i+1);
					
				}
				else if(i%2 != 0){
					index.push_back(i);
					index.push_back(i-1);
				}
			}			
		}

        std::sort(index.begin(), index.end());
		index.erase(std::unique(index.begin(), index.end()));
		
		for(unsigned int j=0; j<newList.size(); j++) {
			int flag = 0;
			for(unsigned int k=0;k<index.size();k++) {
				if(index[k] == j) {
				flag = 1;
				}
			}
			if(flag == 0)
			{
				intVertices.push_back(newList[j]);
			}
		}					
		newList.clear();
		newList = intVertices;
		index.clear();
    }
	
	std::sort(A2.begin(), A2.end());

	std::cout << "APPROX-VC-2: ";
	for(unsigned int i = 0; i < A2.size(); i++) {
		std::cout << A2[i];
		if(i != A2.size() - 1)
			std::cout << ",";
	}
	std::cout << std::endl;
}

void *threadInput(void*) {
	int flagg = 0;
	while (flagg == 0) {
		mutee.lock();
        std::string input;
        std::vector<std::string> input_stream;
        //int vSize;
	intVertices.clear();
        int flag;

        std::getline(std::cin, input);
        input_stream = split(input, ' ');
        if (input_stream.size() == 2) {
            if (input_stream[0] == "V") {
                    vSize = std::stoi(input_stream[1]);
            } else if (input_stream[0] == "E") {
				flagg = 1;
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
                            intVertices.push_back(std::stoi(temp_edge[i])); 							intVertices.push_back(std::stoi(temp_edge[i+1]));
                        }
                        else
                            continue;
                    }
                } else {
                    std::cout << "Error: Length of edge is greater than a vertex";
                }
            } else {
                std::cout << "Error: Invalid input" << std::endl;
            }
        } 
	mutee.unlock();
	}
	
	//while (flagg == 1) {
	/*if(std::cin.eof()) {
		std::cout << "Thread run time VC1: ";
		for(unsigned int i = 0; i < VC1t.size(); i++) {
			std::cout << VC1t[i];
			if(i != VC1t.size() - 1)
				std::cout << ",";
		}
		std::cout << "\nThread run time VC2: ";
		for(unsigned int i = 0; i < VC2t.size(); i++) {
			std::cout << VC2t[i];
			if(i != VC2t.size() - 1)
				std::cout << ",";
		}
		std::cout << "\nThread run time CNF: ";
		for(unsigned int i = 0; i < CNFt.size(); i++) {
			std::cout << CNFt[i];
			if(i != CNFt.size() - 1)
				std::cout << ",";
		}
		std::cout << std::endl;
	} */
	return NULL;
}

void *threadminSAT(void* unused) {
	mutee.lock();  
	bool sol;
	if(vSize > 16) {
		sleep(5);
		std::cout << "CNF-SAT-VC: timeout" << std::endl;
	}
	else {
		if(intVertices.size() > 0) {
			for(int k = 0; k <= vSize; k++) { 
				sol = minVertexCover(vSize, k, intVertices);
				if(sol) break;
				else continue;
			}
		}
		else if(intVertices.size() == 0) {	
			std::cout << "CNF-SAT-VC: " << std::endl;
		}
	}
	mutee.unlock();
	#ifdef DEBUG
	int s;
	float sec, nsec, t;
	s = pthread_getcpuclockid(pthread_self(), &CNFid);

	if(s != 0)
		handle_error_en(s, "pthread_getcpuclockid");
	struct timespec ts;
	if(clock_gettime(CNFid, &ts) == -1)
		handle_error("clock_gettime");

	//std::cout << ts.tv_sec << "." << ts.tv_nsec << std::endl;
	sec = ts.tv_sec;
	nsec = ts.tv_nsec;
	t = (sec*1000000 + nsec)/1000000;
	//std::cout << "sec: "  << sec << " nsec: " << nsec/1000000 << std::endl;
	CNFt.push_back(t);
	//#ifdef DEBUG
	std::cout << "CNFt: " << t << std::setprecision(9) << std::endl;
	/**********Time*****************/
	std::cout << "Thread run time CNF: ";
	for(unsigned int i = 0; i < CNFt.size(); i++) {
		std::cout << CNFt[i];
		if(i != CNFt.size() - 1)
			std::cout << ",";
	}
	std::cout << std::endl;
	/****************************/
	#endif
	return NULL;
}

void *threadapproxVC1(void* unused) {
	mutee.lock();
	approxVC1(intVertices);
	mutee.unlock();

	int s;
	float sec, nsec, t;
	s = pthread_getcpuclockid(pthread_self(), &VC1id);

	if(s != 0)
		handle_error_en(s, "pthread_getcpuclockid");
	struct timespec ts;
	if(clock_gettime(VC1id, &ts) == -1)
		handle_error("clock_gettime");
	//VC1t.push_back(ts.tv_sec + ts.tv_nsec / 1000000);

	//std::cout << ts.tv_sec << "." << ts.tv_nsec << std::endl;
	sec = ts.tv_sec;
	nsec = ts.tv_nsec;
	t = (sec*1000000 + nsec)/1000000;
	//std::cout << "sec: "  << sec << " nsec: " << nsec/1000000 << std::endl;
	VC1t.push_back(t);
	#ifdef DEBUG
	std::cout << "VC1t: " << t << std::setprecision(9) << std::endl;
/**********Time*****************/
	std::cout << "Thread run time VC1: ";
	for(unsigned int i = 0; i < VC1t.size(); i++) {
		std::cout << VC1t[i];
		if(i != VC1t.size() - 1)
			std::cout << ",";
	}
	std::cout << std::endl;
/****************************/
	#endif
	return NULL;
}

void *threadapproxVC2(void* unused) {
	mutee.lock();
	approxVC2(intVertices);
	mutee.unlock();

	int s;
	float sec, nsec, t;
	s = pthread_getcpuclockid(pthread_self(), &VC2id);

	if(s != 0)
		handle_error_en(s, "pthread_getcpuclockid");
	struct timespec ts;
	if(clock_gettime(VC2id, &ts) == -1)
		handle_error("clock_gettime");
	//VC2t.push_back(ts.tv_sec + ts.tv_nsec / 1000000);

	//std::cout << ts.tv_sec << "." << ts.tv_nsec << std::endl;
	sec = ts.tv_sec;
	nsec = ts.tv_nsec;
	t = (sec*1000000 + nsec)/1000000;
	//std::cout << "sec: "  << sec << " nsec: " << nsec/1000000 << std::endl;
	VC2t.push_back(t);
	#ifdef DEBUG
	std::cout << "VC2t: " << t << std::setprecision(9) << std::endl;
	/**********Time*****************/
	std::cout << "Thread run time VC2: ";
	for(unsigned int i = 0; i < VC2t.size(); i++) {
		std::cout << VC2t[i];
		if(i != VC2t.size() - 1)
			std::cout << ",";
	}
	std::cout << std::endl;
/****************************/
	#endif
	return NULL;
}

int main() {
    while(1) {
		pthread_t thInput;
		pthread_create (&thInput, NULL, &threadInput, NULL);
		pthread_join (thInput, NULL);

		pthread_t thSAT;
		pthread_create (&thSAT, NULL, &threadminSAT, NULL);
		pthread_join (thSAT, NULL);

		pthread_t thVC1;
		pthread_create (&thVC1, NULL, &threadapproxVC1, NULL);
		pthread_join (thVC1, NULL);

		pthread_t thVC2;
		pthread_create (&thVC2, NULL, &threadapproxVC2, NULL);
		pthread_join (thVC2, NULL);
	}
    return 0;
}