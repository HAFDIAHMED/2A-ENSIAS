

// Server side C/C++ program to demonstrate Socket programming
  
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include  <string.h>

  
#define PORT 8080


int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0},reponse[1024] = {0};
    char *hello = "Hello from server";
      
    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0)) 


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 4) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1){

        	    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    int n=fork();
  //  if (n>0)
   // {
         // dans le processus pere on fait rien 
//}
    if (n==0)
    {

                     //c'est où commence l'intéraction avec le client ,mn hna changer
                    /*
                        par exemple : écrire un serveur multi-client dont le client envoie au serveur
                        un nombre et le serveur lui retourne le double de ce nombre
                    */
  
    	          read(  new_socket , buffer, 1024);

		          if(verifier(buffer)==1)
                  {
                    strcpy(reponse,"accessible");
                  }
                  else
                  {
                     strcpy(reponse,"n'est pas accessible");
                  
                  }

                  sprintf(reponse,"le double est %d",2*x);
                  
                  send( new_socket , reponse , strlen(reponse) , 0 );
                

    }
}
    
    return 0;
}

int verifier(char* ip)
{
 int result=0;

struct sockaddr_in addsrv;
addsrv.sin_family = AF_INET;
addsrv.sin_port = htons(80);
inet_aton(ip,&(addsrv.sin_addr));
memset(&(addsrv.sin_zero), '0', 8);
int ds=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
int result=connect(ds,(struct sockaddr *)& addsrv,sizeof(struct sockaddr));
if(result!=-1) resut=1;
return result ;
}