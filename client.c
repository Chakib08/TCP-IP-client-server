#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEST_IP "127.0.0.1"
#define DEST_PORT 3333

int main(int argc, char *argv[ ])
{
	char buffer[500]; //Tableau de 500 caracteres ( pour faire un String )
        //sockfd ==> descripteur socket
    
	int sockfd,nb;
        int nbPoints = 0;
        char nbPointsChaine[1000];
        int nbAleatoire[1000];
        char nbAleatoireChaine[1000];
        int i = 0;
        
	/* contiendra l'adresse de destination (celle du serveur) */
	struct sockaddr_in dest_addr; // Les adresses AF_INET utilisent une structure sockaddr_in définie dans <netinet/in.h> :

        /* 
          int socketUtilisé = socket(Famille, type, protocole)
        * famille ==> AF_INET ==> TCP/IP ( 4 octets )
        * type ==> SOCK_STREAM ==> orienté connexion
        * protocole ==> service désiré ==> Tjrs mettre à 0 sur TCP/IP
        */

        sockfd = socket(AF_INET, SOCK_STREAM, 0); //Initialisation de la socket

	if(sockfd == -1) //Cas ou il y a une erreur la fonction socket() retourne -1	
         {
            perror("erreur d'appel de socket() client!");    
            exit(1);
         }
	else		 
            printf("appel de socket() client OK...\n");
	/* octet: ordre machine ou reseau identique */
	dest_addr.sin_family = AF_INET; //Famille de protocole 
	/* short, ordre du reseau */
	dest_addr.sin_port = htons(DEST_PORT); //Numéro de port
	dest_addr.sin_addr.s_addr = inet_addr(DEST_IP); // adresse internet
	/* zero pour le reste de la structure */
	memset(&(dest_addr.sin_zero), 0, 8); //contient uniquement des zéros (étant donné que l'adresse IP et le port occupent 6 octets, les 8 octets restants doivent être à zéro) 

        //connect() pour etablir la connexion avec le serveur (serveur_TCP.c)
	if(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1)
	{	  perror("erreur de connect() client!");    exit(1);	}
	else	  printf("connect() client OK...\n");
	


        /*...code du traitement ...*/
	//Traitement avec read() et  write()

        printf("Veuillez saisir le nombre de points : ");
        scanf("%d", &nbPoints); //Saisi du nombre de points de la part de l'utilisateur 
        printf("le nombre de points entrés est de : %d \n", nbPoints); 
        sprintf(nbPointsChaine,"%d",nbPoints); //Ecrire le nombre de points dans une chaine de caracteres afin de l'envoyer au serveur avec la fonction write()
        write(sockfd,nbPointsChaine,strlen(nbPointsChaine)); //Envoi du nombre de caracteres au serveur 
        printf("*****Affichage des nombre aleatoires générés par le serveur***** \n");

        for(i = 0; i < nbPoints; i++) //boucle equivalent à celle qui se trouve dan sle fichier serveur_TCP.c à fin de recevoir et d'afficher les nombres aléatoires reçus du serveur
           {
               nb = read(sockfd,nbAleatoireChaine,sizeof(nbAleatoireChaine)); //lecture du nombre aléatoire envoyé par le seveur
               nbAleatoireChaine[nb]='\0';
               printf("%s", nbAleatoireChaine); //Affichage du nombre aléatoire
      
           }

     printf("\n");
       


	close(sockfd); //fermer la socket

	return 0;
}
