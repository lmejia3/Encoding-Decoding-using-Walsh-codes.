#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <strings.h>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


using namespace std;

int WalshDecode(int key[], int enc[])
{
	int result =0;
	
	return result;
}


	
int main(int argc, char **argv)
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *hostname;
	int buffer =0;


    if (argc < 3)
	{
     	cerr << "usage hostname port\n";
      exit(0);
 	}
    	portno = atoi(argv[2]);//port number pass by command line
	
	hostname = gethostbyname(argv[1]);//retrieve hostname by command line
	if(hostname== NULL)
	{
		cerr<< "ERROR no such host\n";
	}
	
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)hostname->h_addr,(char*)&serv_addr.sin_addr.s_addr, hostname->h_length);
	serv_addr.sin_port = htons(portno);
	

	
	int i= 0;
	int k=0;
	int data[3];
	int to[3];
	pid_t children[3];
    
	while(k < 3 && (cin >> to[k] >> data[k])) //get file numbers by I/O redirection
	{
		//cout<<to[k]<<":"<<data[k]<<endl;//check if its printing out the correct numbers
		k++;
	}
	if (k!=3) {
		cerr << "Error when reading data!" << endl;;
	}

	for (k=0; k < 3; k++) {
		//id_number[i++] = id;
	
		//data_number[i++] = data;

		cout << "Child "<< (k+1) <<", sending value: "<< data[k] <<" to child process " << to[k] << "\n";
		children[k] = fork();
	    if( children[k] == 0)
		{
			sockfd = socket(AF_INET, SOCK_STREAM, 0);//create socket
			connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			char buf[100];
			buf[0] = (to[k] % 10) + '0';
			buf[1] = (data[k] % 10) + '0';

			write(sockfd, buf, 2);
			int read_len = read(sockfd, buf, 99);
			buf[read_len] = 0;
			int key[4];
			int enc[12];
			if (sscanf(buf, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &key[0], &key[1], &key[2], &key[3],
			    &enc[0], &enc[1], &enc[2], &enc[3], &enc[4], &enc[5], &enc[6], &enc[7], &enc[8], &enc[9], &enc[10], &enc[11]) == 16) {
			    	stringstream tmp_sout;
			    	

			    	int val=0;
			    	for (int i=0; i<3; i++) {
						val <<= 1;
						// repeat the key 3 times
			    		int bit=0;
			    		for (int j=0; j<4; j++) {
			    			bit += enc[4*i + j] * key[j];
						}
						if (bit==4) {
							val |= 1;
						}

					}

					cout<<endl;
					tmp_sout <<"Child "<< k+1<<endl
					<<"Signal: "<< enc[0] << " " << enc[1]  << " " <<  enc[2]  << " " <<  enc[3] << " " <<
					enc[4] << " " <<  enc[5] << " " <<  enc[6] << " " <<  enc[7] << " " <<  enc[8] << " " <<
					enc[9] << " " <<  enc[10] << " " <<  enc[11] <<endl
					<<"Code: "<< key[0] << " " << key[1] << " " << key[2] << " " << key[3] <<endl
					<<"Received value = " << val << endl;
					
					cout << tmp_sout.str();
					cout.flush();
			} else {
				cerr << "Bad encoder response" << endl;
			}

			
			close(sockfd);
			exit(0);


		}

    }
    
    //Gracefully finish the children processes
    for (int i=0; i<3; i++) {
    	waitpid(children[i], NULL, 0);
	}

	


    return 0;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	



