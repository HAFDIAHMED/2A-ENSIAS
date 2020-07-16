#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(){

////////// descripteur de serveur et client
	int dss, dsc;

//////////// @serveur et @ client
	struct sockaddr_in aServ,aClt;

/////////// store request clients
	char* req= calloc (2014,sizeof(char));
	
////////// config @server 
	aServ.sin_family=AF_INET;
	aServ.sin_port=htons(3000);
	memset(&aServ.sin_zero,'0',8);
	inet_aton("127.0.0.1",&(aServ.sin_addr));

/////////// recuperation de la taille de l' adresse client
	socklen_t lgAdrClt=sizeof(struct sockaddr);


//////// create socket serv for request
	if((dss=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		fprintf(stderr,"erreur\n");
		exit(EXIT_FAILURE);
	}

/////// bind socket to  @Serv
	if(bind(dss,(struct sockaddr *)&aServ,sizeof(struct sockaddr))==-1){
		fprintf(stderr,"erreur de bind\n");
		exit(EXIT_FAILURE);
	}

/////// maintenent on fait un listen sur le port 3000 pour des nouvelles tenatives de connexion , 10 vaut dire nombre maximal de connexion en attente 
	
	if(listen(dss,10)==-1){
		printf("erreur de listen\n");
		return EXIT_FAILURE;
	}

////////// traitement de request de clients
	while(1){
	
		dsc= accept(dss,(struct sockaddr *)&aClt,&lgAdrClt);

		/////////  traitement
		memset(req,'\0',2014);
		if(recv(dsc,req,1024,0)>0)
			send(dsc,req,strlen(req),0);
		//close(dsc);
		
	}

		close(dss);

	return 0;
}
