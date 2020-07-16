#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<sys/select.h> 
int main(){

	int dss, dsc;
	
	struct sockaddr_in aServ,aClt;
	char* req= calloc (1024,sizeof(char));
	socklen_t lgAdrClt=sizeof(struct sockaddr);

	time_t laDate;
	struct tm laDateFmt;
	
	int dsClt[20];  ///// stocker les ds des client
	int n,cptClt,maxDS,i,j,k;
	
	fd_set readAllfd; ///  stokcer les ds clients et d serveur
	fd_set readfds;  /// m' aide a trouver le desc qui a debloque select();
	
	
	
	
////////// config adr serv
	aServ.sin_family=AF_INET;
	aServ.sin_port=htons(3000);
	memset(&aServ.sin_zero,'0',8);
	inet_aton("127.0.0.1",&(aServ.sin_addr));


	
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
	
	if(listen(dss,10)==-1){
		printf("erreur de listen\n");
		return EXIT_FAILURE;
	}

	cptClt=0; 
	FD_ZERO(&readAllfd);
	FD_SET(dss,&readAllfd);
	maxDS=dss;
////////// traitenet de request de clients
	while(1){
	
		/// mettre a jour readfds  apres chaque iteration
		FD_ZERO(&readfds);
		readfds=readAllfd; 
		
		select(maxDS+1,&readfds,NULL,NULL,NULL);
		
		//// nouvelle connexion
		if(FD_ISSET(dss,&readfds)){
			dsc= accept(dss,(struct sockaddr *)&aClt,&lgAdrClt);
			
			cptClt++;
			FD_SET(dsc,&readAllfd);
			dsClt[cptClt-1]=dsc;
			if(dsc>maxDS) maxDS=dsc;
			printf("nouveau client communicant via DS , %d\n",dsc);

			
		}
		
		else{
		   for(i=0;i<cptClt;i++){
		
			dsc=dsClt[i];
			/// trouver le ddesc client qui a debloque select
			if(FD_ISSET(dsc,&readfds)){
				
				memset(req,'\0',1024);
				n= recv(dsc,req,1024,0);
				
				//// dans ce cas le client s'est deconnecte
				if(n==0){
				
					// supprimer le client de la liste des desc clients 
					FD_CLR(dsc,&readAllfd);
					for(j=i;j<cptClt-1;j++) dsClt[j]=dsClt[j+1];
					cptClt--;
					close(dsc);
					if((dsc==maxDS) && cptClt!=0)
						maxDS =0;
						for(j=1;j<cptClt;j++)
							if(dsClt[j]>maxDS) maxDS=dsClt[j];
					memset(req,'\0',1024);
					printf("le client communicant via DS ,s'est deconnecte %d\n",dsc);
					break;
				}
				//// notre traitement usuel qu' on fait
				else if(n>0){
					time(&laDate);
					laDateFmt = *localtime(&laDate);
					if(strcmp(req,"HEURE\r\n")==0){
						printf("\n-%s-\n",req);
						sprintf(req,"%dh:%d\n",laDateFmt.tm_hour,laDateFmt.tm_min);
						send(dsc,req,strlen(req),0);
					}
			
					else if(strcmp(req,"DATE\r\n")==0){
						sprintf(req,"%d:%d:%d\n",laDateFmt.tm_mday,(laDateFmt.tm_mon+1),(laDateFmt.tm_year+1900));
						send(dsc,req,strlen(req),0);			
					}
					else if(strcmp(req,"HEUREDATE\r\n")==0){
						sprintf(req,"%d:%d:%d  %dh:%d\n",laDateFmt.tm_mday,(laDateFmt.tm_mon+1),(laDateFmt.tm_year+1900),laDateFmt.tm_hour,laDateFmt.tm_min);
						send(dsc,req,strlen(req),0);
					}
					else if(strcmp(req,"EXIT\r\n")==0){
						printf("\n-%s-\n",req);
						printf("\n-%s-\n",req);
		    				close(dsc);
					}
					memset(req,'\0',1024);
					break;
				}
			 }

///////////////////////////////
			}
/////////////////////////////

		   }
		  	
		
		
			
	}

		

	return 0;
}
