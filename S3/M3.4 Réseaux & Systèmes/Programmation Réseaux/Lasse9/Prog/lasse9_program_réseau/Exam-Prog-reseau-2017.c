/*
Q1) Echo: 9   IMAP:   DNS: 56   SNMP: 
Q2) Le fichier /etc/hosts peut être utilisé pour une résolution de nom local
Q3) /etc/protocols définit pour chaque porotcole un nombre correspondant. C’est le numéro utilisé dans l’entête IP.
Q4) On s'est connecté à un serveur web qui contient des ressources HTML. cette connexion à été faite avec succés.
Q5) - le port donnée est déjà affecté
    - 
Q6) Non, car le bind se fait du coté serveur, il sert à associer à un descripteur de socket à une socket local (Sauf si ce client est en même temps client/serveur)
Q7) 	 _______________________________
	|	Application		|
	|___________________Sockets_____|
	|	TCP/UDP			|
	|_______________________________|
	|	IP			|
	|_______________________________|
	|	Acces réseau		|
	|_______________________________|
Q8) le champs sin_zero sert à rendre la longueur de la structure sockaddr_in 16 octect pour que toutes les structure aie la meme taille 
Q9) La fonction select permet de surveiller au même temps plusieurs sockets et d’être averti de chaque changement réalisé.sur plusieurs descripteurs. Ce qui plus optimal au niveau des ressourcesque l'utilisation de fork
Q10) Non, car telnet sert a vérifier le protocoles de mode connecté (c-a-d TCP), alors que le protocoles de UDP sont en mode non connecté


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char** argv){
	
	int dss,dsc;

	struct sockaddr_in aSrv,aClt;

	socklen_t lgaClt=sizeof(aClt);

	int lstSocket[20];

	char * msg=calloc(1024,sizeof(char));
	
	//adresse du serveur
	aSrv.sin_family=AF_INET;
	aSrv.sin_port=htons(9990);
	inet_aton("127.0.0.1",&(aSrv.sin_addr));
	memset(&(aSrv.sin_zero),'0',8);
	
	//créer socket serveur
	dss=socket(PF_INET,SOCK_STREAM,6);
	
	//fonction bind
	bind(dss,(struct sockaddr*)&aSrv,sizeof(struct sockaddr));
	
	//fonction listen
	listen(dss,20);
	
	while(1){
		dsc = accept(sd,(struct sockaddr *)&ad_cli , &taille);

		if(fork() == 0){
			memset(msg,'\0',1024);
			recv(lstSocket[i],msg,strlen(msg),0);
			
			int dsmachine;
			struct sockaddr_in adrMachine;
			adrMachine.sin_family=AF_INET;
			adrMachine.sin_port=htons(9990);
			inet_aton(msg,&(adrMachine.sin_addr));
			memset(&(adrMachine.sin_zero), '0',8);
				    
			dsmachine=socket (AF_INET, SOCK_STREAM, 6);
			int v=connect(dsmachine,(struct sockaddr *) & adrSrv, sizeof(struct sockaddr));
			if(v<0)
				send(dsc,"Inaccessible",1024,0);
			else
				send(dsc,"Accessible",1024,0);
		}
	}
	return 0;
}
