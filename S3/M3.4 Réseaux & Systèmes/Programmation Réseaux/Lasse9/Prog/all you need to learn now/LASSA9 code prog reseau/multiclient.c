#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
int main(){

	int dss, dsc;
	struct sockaddr_in aServ,aClt;
	char* req= calloc (2014,sizeof(char));
	time_t laDate;
	struct tm laDateFmt;
	
	
////////// config adr serv
	aServ.sin_family=AF_INET;
	aServ.sin_port=htons(3000);
	memset(&aServ.sin_zero,'0',8);
	inet_aton("127.0.0.1",&(aServ.sin_addr));


	socklen_t lgAdrClt=sizeof(struct sockaddr);
//////// create socket serv for request

	if((dss=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1){
		fprintf(stderr,"erreur\n");
		exit(EXIT_FAILURE);
	}
/////// bind socket with adrServ
	if(bind(dss,(struct sockaddr *)&aServ,sizeof(struct sockaddr))==-1){
		fprintf(stderr,"erreur de bind\n");
		exit(EXIT_FAILURE);
	}

/////// maintenent on fait un listen sur le port 3000 pour des nouvelles tenatives de connexion , 10 vaut dire nombre maximal de connexion en attente 
	
	if(listen(dss,3)==-1){
		printf("erreur de listen\n");
		return EXIT_FAILURE;
	}
////////// traitement de request de clients
	while(1){
		dsc= accept(dss,(struct sockaddr *)&aClt,&lgAdrClt);
		printf("nouveau client connecte\n");
		
		///partie serveur : processus pere
		if(fork()!=0) close(dsc);//nouv instruct
		
		//// partie client : processus fils
		else{
		
		    close(dss); /// nouv instruct

		    ////////////// traitement 
		    while(1){
			time(&laDate);
			laDateFmt = *localtime(&laDate);
			memset(req,'\0',2014);
		    	if(recv(dsc,req,1024,0)>0){
				printf("\n-%s-\n",req);
				if(strcmp(req,"HEURE\r\n")==0){
					printf("\n-%s-\n",req);
					sprintf(req,"%dh:%d\n",laDateFmt.tm_hour,laDateFmt.tm_min);
					send(dsc,req,strlen(req),0);
				}
			
				if(strcmp(req,"DATE\r\n")==0){
					sprintf(req,"%d:%d:%d\n",laDateFmt.tm_mday,(laDateFmt.tm_mon+1),(laDateFmt.tm_year+1900));
					send(dsc,req,strlen(req),0);			
				}
				if(strcmp(req,"HEUREDATE\r\n")==0){
					sprintf(req,"%d:%d:%d  %dh:%d\n",laDateFmt.tm_mday,(laDateFmt.tm_mon+1),(laDateFmt.tm_year+1900),laDateFmt.tm_hour,laDateFmt.tm_min);
					send(dsc,req,strlen(req),0);
				}
				if(strcmp(req,"EXIT\r\n")==0){
					printf("\n-%s-\n",req);
					printf("\n-%s-\n",req);
		    			
					close(dsc); /// nouv instruct
				}

			}
		    }

		}
		
			
	}

		close(dss);

	return 0;
}
