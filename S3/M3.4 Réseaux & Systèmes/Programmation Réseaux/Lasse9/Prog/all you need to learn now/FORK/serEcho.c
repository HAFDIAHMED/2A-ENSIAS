#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
int main(){
	int dss,dsc,lgAdeClt,i;
    	struct sockaddr_in addserv,addClt;
	char* reqt = calloc(1024,sizeof(char));	
	char* reps = calloc(1024,sizeof(char));
    	addserv.sin_family = AF_INET;
     addserv.sin_port = htons(990);
    	inet_aton("127.0.0.1",&(addserv.sin_addr));
    	memset(&(addserv.sin_zero),'0',8);
     dss = socket(AF_INET,SOCK_STREAM,6);
	if(bind(dss,(struct sockaddr*) &addserv,sizeof(struct sockaddr)) != -1){
		if(listen(dss,2) !=-1){
			while(1){
				dsc = accept(dss,(struct sockaddr *) &addClt, (socklen_t *) &lgAdeClt);
				if(fork() ==0){
					strcpy(reps,"Donner votre message :" );
					send(dsc,reps,1024,0);				
					recv(dsc,reqt,1024,0);
					int i =0;
					while(reqt[i] != '\0'){
						reqt[i] = toupper(reqt[i]);
						i++;
					}
					reqt[i] = '\0';
					send(dsc,reqt,1024,0);
					reqt = calloc(1024,sizeof(char));	
					exit(-1);
					close(dsc);
				}
			}
		}else printf("\nCan't listen");
	}else printf("Can't bind\n");
	printf("Closing program ...\n");
    	return 0;
}
