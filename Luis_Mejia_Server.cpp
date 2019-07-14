//I used this website as reference
//http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

using namespace std;

int H[4][4] = {{0,0,0,0}, {0,1,0,1}, {0,0,1,1}, {0,1,1,0}};
	
// gets 3 values
void WalshEncode(int values[], char * buf, int buf_size) {

	int enc[12] = {0,0,0,0, 0,0,0,0, 0,0,0,0};
	int bitmasks[3] = {4, 2, 1};
	for (int i=0; i<3; i++) {
		// Run for every value
		for (int j=0; j < 3; j++) {
			// Run for ever bit
			for (int k=0; k < 4; k++) {
				// Run for every entry in the key
				enc[j * 4 + k] += ((values[i] & bitmasks[j])?1:-1) * (H[i+1][k]?1:-1);
			}
		}
	}
	snprintf(buf, buf_size, "%d %d %d %d %d %d %d %d %d %d %d %d\n", enc[0], enc[1], enc[2], enc[3], enc[4], enc[5], enc[6], enc[7], enc[8], enc[9], enc[10], enc[11]);
	
}

void WalshKey(int client, char * buf, int buf_size) {
	if (client >= 1 && client <= 3)
	 snprintf(buf, buf_size, "%d %d %d %d ", H[client][0]?1:-1, H[client][1]?1:-1, H[client][2]?1:-1, H[client][3]?1:-1);
}

int main(int argc, char **argv) {
    if (argc < 2)
	 {
    		cerr << "ERROR: no port\n";
        	exit(1);
    }
    
    int portno = atoi(argv[1]);//stores port number
	//int clilen;//stores size of address
	int listenfd, sockfd, n, i;
	socklen_t clilen;
	struct sockaddr_in cli_addr, serv_addr;//contain address of server and client
	int buffer = 256;//server reads characters from the socket into buffer
	int new_value = 256;//new value to return the message of the binary
	int new_array[20];


    sockfd = socket(AF_INET, SOCK_STREAM, 0);//creates new socket
    if (sockfd < 0)
		cerr<< "ERROR opening socket\n";
		
	//bzero((char *) &serv_addr, sizeof(serv_addr));//sets all values in the buffer to 0

    //preparation of the socket address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);//converts port number in host byte to network byte order
    serv_addr.sin_addr.s_addr = INADDR_ANY;//contains IP address

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    	cerr << "ERROR binding.\n";//binds a socket to host address
    	exit(2);
    }

    listen(sockfd, 5);//listen on the socket for connections
    //cout << "Server running...waiting for connections.\n";

	
	//causes the process to block until a client connects to server
	clilen = sizeof(cli_addr);
	int newsockfd[3];
	
	for(i=0; i<3; i++)
	{
		newsockfd[i] = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
	    if (newsockfd[i] < 0) {
	    	cerr<< "ERROR on accept";
	    	return 0;
		}
	        
	}
	
	int vals[3];
	int to[3];
	int from[3];
	for(i=0; i<3; i++)
	{
		char buf[100];
		int total_read=0;
		while (total_read < 2) {
			total_read += read(newsockfd[i], buf + total_read, 2 - total_read);
		}
		to[i] = buf[0] - '0';
		from[to[i]-1] = i;
		vals[i] = buf[1] - '0';
		if (to[i] >=1 && to[i] <=3 && vals[i] >=0 && vals[i] <= 7) {
			cout << "Here is the message from child "<< (i + 1) << ": Value = "<< vals[i] << ", Destination = " << to[i] << endl;
		}
	}
	
	char encMesg[100];
    char key[200];
	WalshEncode(vals, encMesg, 100);
	
	for (int j=0; j < 3; j++) {
		WalshKey(from[j] + 1, key,100);
		strncat(key, encMesg, 200);
  	//	write(newsockfd[j], encMesg, strlen(encMesg));
  		write(newsockfd[j], key, strlen(key));
  		sleep(1);
	}



	for(i=0; i<3; i++)
	{
		close(newsockfd[i]);
	}
 
 
	close(sockfd);


	
    return 0;
}

