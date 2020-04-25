
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <vector>

using namespace std;

int main (int argc, char **argv) {

	vector<pid_t> kids;

	int rgenA1[2];
	pipe(rgenA1);
	
	int a1_a2[2];
	pipe(a1_a2);

	pid_t child_pid, child_pid_1, child_pid_2, child_pid_3;
	//while(!cin.eof()) {
	child_pid = fork();

        argv[argc+1]=NULL;
	
	if(child_pid == 0) {
		dup2(rgenA1[1], STDOUT_FILENO);
		close(rgenA1[0]);
		close(rgenA1[1]);
        if(execv("./rgen",argv)==-1){perror("Error");}
	}

	kids.push_back(child_pid);

	child_pid_1 = fork();

    if (child_pid_1 == 0) {
        dup2(rgenA1[0], STDIN_FILENO);
        close(rgenA1[0]);
        close(rgenA1[1]);

        dup2(a1_a2[1], STDOUT_FILENO);
        close(a1_a2[0]);
        close(a1_a2[1]);
		
        // python code(a1)
	    char* argv[3];
    	argv[0] = (char*) "/bin/python";
    	argv[1] = (char*) "ece650-a1.py";
    	argv[2] = nullptr;

    	if(execv("/bin/python", argv)==-1){perror("Error");}
	}

	kids.push_back(child_pid_1);

    child_pid_2 = fork();

	if (child_pid_2 == 0) {
        	dup2(a1_a2[0], STDIN_FILENO);
        	close(a1_a2[0]);
        	close(a1_a2[1]);
        
        /*dup2(a1_a2[1], STDOUT_FILENO);
        close(a1_a2[0]);
        close(a1_a2[1]);*/
        
      // c++ code(a2)
		if(execv("./ece650-a2",argv)==-1){perror("Error");}
	}

	kids.push_back(child_pid_2);
    
    //child_pid_3 = fork();

    /*if (child_pid_3 == 0) {
        dup2(a1_a2[0], STDIN_FILENO);
        close(a1_a2[1]);
        close(a1_a2[0]);
	}
   

	kids.push_back(child_pid_3);*/

	///child_pid = 0;
    	
        pid_t child_pid_4 = fork();
        
	if(child_pid_4 == 0){
		//user input
		dup2(a1_a2[1], STDOUT_FILENO);
	    	close(a1_a2[0]);
	    	close(a1_a2[1]);
	
		while (!std::cin.eof()) {
		        string input;
		        getline(std::cin, input);
		        if (input.size() > 0) {
		            std::cout << input << std::endl;
		            cout.flush();
		        }
	       }
	}
	
	kids.push_back(child_pid_4);	
	//}
	for (pid_t k : kids) {
        int status;
	waitpid(k, &status, 0);
        kill (k, SIGTERM);
    }
	
	return 0;
}