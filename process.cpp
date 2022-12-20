#include <sys/types.h>
#include <stdio.h>
#include <string>
#include <unistd.h>


#define readIn 0
#define writeIn 1

#define bufferSize 256


int main(int argc, char* argv[]) {

	if(argc < 4){
		exit(0);
	}

int numItems = atoi(argv[1]);
int range = atoi(argv[2]);
int seed = atoi(argv[3]);

int buffer[bufferSize];
int fd[2];

int min = 0;
int max = 0;
float avg = 0;
int randNum = 0;

srand(seed);

pid_t pid;

//Create pipe
	if(pipe(fd) == -1){
		fprintf(stderr,"Pipe failed");
		exit(0);
	}

//Fork
	pid = fork();
	if(pid < 0){
		fprintf(stderr,"Fork failed");
		exit(0);
	}	 

	//Parent(writer)
	if(pid > 0){
		int parentId = getpid();

		close(fd[readIn]);
		int ploop;
		for(ploop = 0;ploop < numItems; ploop++){
			randNum = rand() % range + 1;
			buffer[ploop] = randNum;
			write(fd[writeIn],&buffer[ploop],bufferSize);
		
	}

	close(fd[writeIn]);

	printf("Parent ID: %d - Number of items written into the pipes are: %d\n", parentId, ploop);
	wait();	
	}
	
	//Child(reader)
	else if(pid == 0){
		int childId = getpid();
		


		close(fd[writeIn]);
		int cloop;
		for(cloop = 0; cloop < numItems; cloop++){
			read(fd[readIn],&buffer[cloop],bufferSize);
			randNum = buffer[cloop];
		
			max += randNum;
			min = randNum; 
			avg = (randNum/numItems);

		

		}

	
	close(fd[readIn]);
	
	printf("Child ID: %d - Number items recieved: %d, min: %d, max: %d, avg: %f\n",childId, cloop, min, max, avg);		
	
	
	}	
	
return 0;
