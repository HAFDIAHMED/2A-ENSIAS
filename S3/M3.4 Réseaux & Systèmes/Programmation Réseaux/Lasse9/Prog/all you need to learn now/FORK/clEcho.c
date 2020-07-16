#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main(){
	char* reqt = calloc(1024,sizeof(char));	
	char* reps = calloc(1024,sizeof(char));
    	struct sockaddr_in addserv;
    	addserv.sin_family = AF_INET;
    	inet_aton("127.0.0.1",&(addserv.sin_addr));
     addserv.sin_port = htons(990);
    	memset(&(addserv.sin_zero),'0',8);
	int ds;
	while(1){	
	 	ds = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(connect(ds,(struct sockaddr*) &addserv,sizeof(struct sockaddr))!=-1){
				recv(ds,reps,1024,0);
				printf("%s\n",reps);		
				gets(reqt);
				send(ds,reqt,strlen(reqt),0);
				recv(ds,reps,1024,0);
				printf("%s\n",reps);
		}else printf("didn't connect\n");
		close(ds);
	}
    return 0;
}
