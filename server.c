#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h> //Ajout de cette bibliotheque pour permettre la génération des nombres aléatoires


    
    //nombreAleatoire = (rand() % (MAX - MIN + 1)) + MIN;

/* le port que le serveur ecoute */
#define MYPORT 3333
/* le nombre de connections que la file d'attente peut contenir */
#define BACKLOG 10
int main()
{
   srand(time(NULL)); // Initialiser le génerateur de nombres aléatoires pour la fonction rand()
 const int MAX = 1000, MIN = 0; // plage didiée à notre génerateur de nombres aléatoires fixé avec deux constantes (plage entre 0 et 1000)
 char nbPointsChaine[500];
 int nbPoints;
 int nbAleatoire[1000];
  int i = 0;
char nbAleatoireChaine[1000];
    
	/* ecoute sur sockfd, connection entrante sur new_fd */
        //sockfd socket de serveur_TCP qu'on va décrire
        //new_fd socket qu'on a créer precedemment dans un autre fichier.c en l'occurence (client.c) et qui va etre connecté à sockfd en acceptant l'appel avec accept()
	int sockfd, new_fd;
	/* my_addr : adresse de la machine qui execute ce programme */
	struct sockaddr_in my_addr;
	/* their_addr : adresse de la machine qui se connecte */
	struct sockaddr_in their_addr;
	int sin_size, nb;
	char buffer[500];
        
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)	{  perror("erreur d'ouverture de socket() !");	  exit(1);}
	else	  printf("ouverture socket() OK...\n");

	/* octet dans l'ordre de la machine (idem reseau pour un seul octet!) */
	my_addr.sin_family = AF_INET;
	/* Port: short, dans l'ordre du reseau */
	my_addr.sin_port = htons(MYPORT);
	/* remplissage automatique de l'adresse de la machine locale */
	my_addr.sin_addr.s_addr = INADDR_ANY;
	/* zero pour le reste de la structure */
	memset(&(my_addr.sin_zero), 0, 8);

    //bind() liaison du socket à un point de communication defini par un port et une adresse 
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)	{
		  perror("erreur dans l'appel de bind() !");	  exit(1);	}
	else	  printf("appel de bind() OK...\n");
   //listen() mettre le socket en attente de connnexion
	if(listen(sockfd, BACKLOG) == -1)	{  perror("erreur d'appel listen()!"); exit(1);	}
	else	  printf("appel de listen() OK...\n");

	/* ...code de traitement des connexions entrantes... */
	sin_size = sizeof(struct sockaddr_in);
    //accept() connexion avec la socket du client.c en acceptant son appel
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if(new_fd == -1)  {	perror("erreur d'appel de accept() !"); exit(1); }
	else		  printf("appel de accept() OK...\n");
	printf("adresse du client qui se connecte:%s\n",inet_ntoa(their_addr.sin_addr));
	printf("port distant du client qui se connecte:%u\n",their_addr.sin_port);

       
	/*.....reste du code de traitement avec read() et write() sur new_fd .......*/
      nb=read(new_fd,nbPointsChaine,sizeof(nbPointsChaine)); // reception du nombre de points de la part du client
      nbPointsChaine[nb]='\0';
      printf("le client envoi le nombre de points à génerer qui est de : %s \n", nbPointsChaine);
      nbPoints = atoi(nbPointsChaine); //Conversion du nombre de char à int pour pouvoir l'utiliser dans notre boucle for et génerer le nombre de points demandé par l'utilisateur
      printf("Processus de génération des nombres aléatoires et envoi au client \n");
      printf("pour l'affichage \n");

    for(i = 0; i < nbPoints; i++)
           {
                 nbAleatoire[i] = (rand() % (MAX - MIN + 1)) + MIN; //Generateur de nombres aléatoires en utilisants la fonction rand()
                 sprintf(nbAleatoireChaine," %d ",nbAleatoire[i]); //mettre le nombre aléatoire généré dans une chaine de caractere pour l'envoyer au client avec write()
                 printf("%s ",nbAleatoireChaine); //Imprimer le nombre aléatoire généré pour s'assurer que c'est le meme nombre qui a été envoyé au client
                 write(new_fd,nbAleatoireChaine,strlen(nbAleatoireChaine)); //envoyer le nombre aléatoire généré au client
         
          
           } 
    printf("\n");

	close(new_fd);
	close(sockfd);
	return 0;
}
