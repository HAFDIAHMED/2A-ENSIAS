#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>


int main(){



//descripteur de socket
	int ds;


/// vars to store request and response
	char* res=(char *)calloc(2,sizeof(char));
	char* req=(char *)calloc(1024,sizeof(char));


/// parametrage des donnees de serveur
	struct sockaddr_in sAddr;
	sAddr.sin_family=AF_INET;
	sAddr.sin_port=htons(3000);
	memset(&sAddr.sin_zero,'0',8);
	inet_aton("127.0.0.1",&(sAddr.sin_addr));
	
///////creation de socket
	if((ds=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		fprintf(stderr,"erreur\n");
		exit(EXIT_FAILURE);
	}

///////////// connexion socket to @serv
	if(connect( ds,(struct sockaddr *)&sAddr,sizeof(struct sockaddr) )!=-1){

		
		/////////////// traitement 
		scanf("%s",req);
		send(ds,req,strlen(req),0);

		while((recv(ds,res,1,0))>0){
			printf("%c",res[0]);
		}
		printf("\n");
		////////////////////////////////////

	}else{
		printf("probleme de connexion");
	}

	return 0;

}

