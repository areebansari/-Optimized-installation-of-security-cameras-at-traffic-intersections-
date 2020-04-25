#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int randomNumber(void){
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    // read a random unsigned int
    unsigned int num = 0;
    unsigned int size = 1; 

    urandom.read((char *) &num, size);

    // close random stream
    urandom.close();

    return num;
}

int randomSignedNumber(void){
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    // read a random unsigned int
    signed int num = 42;
    signed int size = sizeof(num); 

    urandom.read((char *) &num, size);

    // close random stream
    urandom.close();

    return num;
}

string randomString(int length){
    char randomString[] = "abcdefghijklmnopqrstuvwxyz";
    string sName;
    for(int i=0; i<length; ++i){
	sName = randomString[rand() % (sizeof(randomString) - 1)];
    }
    return sName;
}

bool zeroLength(vector<int> xval, vector<int> yval) {	
	for(unsigned int i=0; i<xval.size(); i++) {
		if(xval[i] == xval[i+1] && yval[i] == yval[i+1])
			return false;
		else
			continue;
	}
	return true;
}

/*bool onSegment(vector< vector<int> > x, vector< vector<int> > y){
	for(unsigned int i=0; i<x.size(); i++) {
		for(unsigned int j=0; j<x[i].size(); j++){
			if( max(x[i][j],y[i][j]) < min(x[i][j+2],y[i][j+2]) 
			   || min(x[i][j],y[i][j]) > max(x[i][j+2],y[i][j+2]) 
			   && max(x[i][j+1],y[i][j+1]) < min(x[i][j+3],y[i][j+3]) 				   || min(x[i][j+1],y[i][j+1]) > max(x[i][j+3],y[i][j+3]) )
			{
					return true;			
			} 
			else {
					return false;
			}
		}
	}
}*/

bool onSegment(vector< vector<int> > x, vector< vector<int> > y){
	bool even = false;
	bool odd = false;
	bool e1=false, o1=false;
	for(unsigned int i=0; i<x.size(); i++) {
		for(unsigned int j=0; j<x[i].size(); j++){
			//cout << "X[" << i << "]" << "[" << j << "]" << x[i][j] << endl;
			//cout << "Y[" << i << "]" << "[" << j << "]" << y[i][j] << endl;			
			cout << "i: " << i <<endl;
			if(j%2==0) {
				//cout << "j: " << j << endl;
				e1 = true;
				o1 = false;
			}
			else {
				//cout << "j: " << j << endl;
				o1 = true;
				e1 = false;
			}
			for(unsigned int k=2; k<x[i].size(); k+=2) {
			if (e1) {
				cout << "j: " << j << endl;
				cout << "X[" << i << "]" << "[" << j << "]" 
				<< x[i][j] << endl;
				cout << "Y[" << i << "]" << "[" << j << "]" 
				<< y[i][j] << endl;
				cout << "X[" << i << "]" << "[" << k << "]" 
				<< x[i][k] << endl;
				cout << "Y[" << i << "]" << "[" << k << "]" 
				<< y[i][k] << endl;
				if( x[i][j]==x[i][k] && y[i][j]==y[i][k] ) 
				{
					even = true;			
					cout << "IM eVeN" << endl;
				} 
				else {
					cout << "Else Even" << endl;
					even = false;
					//continue;
				}
			} }
			for(unsigned int k=3; k<x[i].size(); k+=2) {
			if(o1) {
				cout << "j: " << j << endl;
				cout << "X[" << i << "]" << "[" << j << "]" 
				<< x[i][j] << endl;
				cout << "Y[" << i << "]" << "[" << j << "]" 
				<< y[i][j] << endl;
				cout << "X[" << i << "]" << "[" << k << "]" 
				<< x[i][k] << endl;
				cout << "Y[" << i << "]" << "[" << k << "]" 
				<< y[i][k] << endl;
				if( x[i][j]==x[i][k] && y[i][j]==y[i][k] ) {
					odd = true;
					cout << "IM oDd" << endl;
				}
				else {
					cout << "Else Odd" << endl;
					odd = false;
					//continue;
				}
			} }
			if(even && odd) {
				cout << "found the same pattern" << endl;
				return true;
			}
			else {
				cout << "Still Searching" << endl;
				//odd = false;
				//even = false;
				continue;
			}
		}
	}
	return false;
}

vector<int> randomCoordinates(int _coords_k, int _lineSeg_k, 
			      vector< vector <int> > x, vector< vector <int> > y) {
	int count = 0;
	int numofTries = 0;
	vector<int> xyValues;	
	int val = 0;
	while(count < (_lineSeg_k + 1) * 2) {
		val = randomSignedNumber()%_coords_k;
		xyValues.push_back(val);
		count++;
	}

	vector<int> xvalue;
	vector<int> yvalue;
	
	while(numofTries < 25) {

		numofTries++;

		for(unsigned int i=0; i<xyValues.size(); i++) {
			if(i%2==0){
			   xvalue.push_back(xyValues[i]);
			} else {
			   yvalue.push_back(xyValues[i]);
			}
		}
		
		if(zeroLength(xvalue,yvalue)){
			return xyValues;
		} else {
			cout << "Error" << endl;
			continue;
		}
	}

	if(numofTries >= 25) {
		cerr << "Error: Failed to generate correct input for 25 simultaneous attempts" << endl;
	}	

	return xyValues;
}

int main(int argc, char **argv) {

    int stCount = 0;
    int streetNum_k = 10; //-s
    int lineSeg_k = 5; //-n
    int waitTime_k = 5; //-l
    int coords_k = 20; //-c
    int randomStreetNum = 2;
    int randomWait = 5;
    int randomLineSeg = 1;
    vector<int> xyCoords;
    vector<int> xVals;
    vector<int> yVals;
    vector < vector <int> > all_x;
    vector < vector <int> > all_y;
    int c;

    while ((c = getopt(argc, argv, "s:n:c:l:")) != -1)
        switch (c) {
            case 's':
                streetNum_k = atoi(optarg);
                break;
            case 'n':
                lineSeg_k = atoi(optarg);
                break;
            case 'c':
                coords_k = atoi(optarg);
                break;
            case 'l':
                waitTime_k = atoi(optarg);
                break;
            default:
                return 0;
        }

    if (optind < argc) {
        cout << "Found positional arguments\n";
        for (int index = optind; index < argc; index++)
            cout << "Non-option argument: " << argv[index] << "\n";
    }

    string streetName;
    string prevStName;
    string xyPattern;
    vector<string> streetNames;

    while(!cin.eof()){
    int random = ceil(randomNumber() * streetNum_k/255);
    if(random >= 2 && random <= streetNum_k)
	    randomStreetNum = random;
    //cout << "Random int: " << random << endl; 
    //cout << "Street Num: " << randomStreetNum << endl;
    
    random = ceil(randomNumber() * waitTime_k/255);
    if (random >=5 && random <= waitTime_k)
    	    randomWait = random;

    //cout << "Random int: " << random << endl; 
    //cout << "Wait time: " << randomWait << endl;

    while(stCount < randomStreetNum) {
	streetName = randomString(6);
	prevStName = streetName;
	if(prevStName == streetName){
		streetName += randomString(3);
	}
	int _randomLineSeg = randomNumber()%lineSeg_k; 
	if(_randomLineSeg >= 1 && _randomLineSeg <= lineSeg_k)
		randomLineSeg = _randomLineSeg;
	xyCoords = randomCoordinates(coords_k, randomLineSeg, all_x, all_y);
	for (unsigned int i=0; i<xyCoords.size(); i++){ 
		if(i%2==0){
		    xVals.push_back(xyCoords[i]);
		} else {
		    yVals.push_back(xyCoords[i]);
		}
	}
	for (unsigned int i=0; i<xVals.size(); i++) {
		xyPattern = xyPattern + "(" + to_string(xVals[i]) + "," 
			    + to_string(yVals[i]) + ")";
	}
        cout << "a " << "\"" << streetName << "\" " << xyPattern << endl;
	streetNames.push_back(streetName);

	/*for(unsigned int i=0; i<xVals.size(); i++){
		cout << "xVals: " << xVals[i] << endl;
		cout << "yVals: " << yVals[i] << endl;
	}*/
	
	all_x.push_back(xVals);
	all_y.push_back(yVals);

	xVals.clear();
	yVals.clear();
	xyCoords.clear();
	xyPattern.clear();

	stCount++;
    }

    cout << "g" << endl;

	/*for(unsigned int i=0; i<all_x.size(); i++) {
		for(unsigned int j=0; j<all_x[i].size(); j++){
			cout << "X[" << i << "]" << "[" << j << "]" << all_x[i][j] << 										endl;
			cout << "Y[" << i << "]" << "[" << j << "]" << all_y[i][j] << 										endl;
		}
	}*/
	//onSegment(all_x, all_y);

    sleep(randomWait);

    for (unsigned int i=0; i<streetNames.size(); ++i) {
    	cout << "r " << "\"" << streetNames[i] << "\"" << endl;
    }
    streetNames.clear();
    all_x.clear();
    all_y.clear();
    stCount = 0;
    }

    return 0;
}