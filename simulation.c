#include "simulation.h"
#include <stdio.h>

void recupererNomBot(SDL_Renderer* renderer, int * quitterJeu, char *filename) {
    FILE *fichier;

    /* demandeNom du bot */

    char *pathBackgroud = "Images/DemanderBot_Background.jpg";
    char *pathMessage = "Images/Entrer-votre-Bot.png";
    char *nomBot;

    do {
	nomBot = DemanderQqch(renderer, quitterJeu, pathBackgroud, pathMessage);
	if (nomBot != NULL && strlen(nomBot) > 0) {
	    sprintf(filename, "bots/%s", nomBot);
	    fichier = fopen(filename, "r");
	    if (fichier == NULL) {
		printf("Le fichier n'existe pas ou n'a pas pu être ouvert. Veuillez réessayer.\n");
	    }
	} else exit(1);
    } while (fichier == NULL);

    // Fermer le fichier, car nous vérifions seulement s'il existe
    fclose(fichier);
}

void recupererBot(SDL_Renderer* renderer, int * quitterJeu, bot leBot, char * filename) {
    FILE *fichier;

    recupererNomBot(renderer, quitterJeu, filename);

    if (strlen(filename) > 5) {
	fichier = fopen(filename, "r");
	if (fichier == NULL) {
	    printf("Échec de l'ouverture du fichier: %s\n", filename);
	    exit(1);
	}

	for (int i = 0; i < NB_REGLES; i++) {
	    for (int j = 0; j < TAILLE_ETAT + 2; j++) {
		if (fscanf(fichier, "%d", &leBot[i][j]) != 1) {
		    printf("Échec de la lecture du nombre à la ligne %d, colonne %d\n", i, j);
		    fclose(fichier);
		    exit(1);
		}
	    }
	}

	fclose(fichier);      
    }
}


void sauvegarderNomBot(char *filename) {
    FILE *fichier;
    int reponse = 0;
    char nomBot[100] = "";

    /* demandeNom du bot */
    printf("Voulez vous sauvegarder votre bot ? 0: NON, 1: OUI\n");
    scanf("%d", &reponse);


    if(reponse){
	do {
	    printf("Donner le nom du bot :\n");
	    scanf("%s", nomBot);
	    sprintf(filename, "bots/%s", nomBot);
	    fichier = fopen(filename, "r");
	    if (fichier != NULL) {
		printf("Le bot existe déjà, veuillez choisir un autre nom\n");
		// Fermer le fichier, si il exister
		fclose(fichier);
	    }
	} while (fichier != NULL);
    }
    else filename = NULL;
}

void sauvegarderBot(bot leBot, char * filename) {
    FILE *fichier;

    sauvegarderNomBot(filename);

    if(filename != NULL){
	fichier = fopen(filename, "w+");
	if (fichier == NULL) {
	    printf("Échec de l'ouverture du fichier: %s\n", filename);
	    exit(1);
	}

	for (int i = 0; i < NB_REGLES; i++) {
	    for (int j = 0; j < TAILLE_ETAT + 2; j++) {
		fprintf(fichier, "%d ", leBot[i][j]);
	    }
	    fprintf(fichier, "\n");
	}
	fclose(fichier);    
    }
}

void lancerSimulation(){

    printf("Quelle simulation lancer ?\n");
    /* switch (expression) { */
    /* ${0:cases} */
    /* } */


    bot robot;
    algoGlouton(robot);

    bot temoin;
    initialiserBot(temoin);
  
    /* affichage robot */
    for(int i = 0; i < NB_REGLES; i++){
	for(int j = 0; j < TAILLE_ETAT + 2; j++) {
	    printf("%d ", robot[i][j]);
	}
	printf("\n");
    }
    printf("score du bot avec algoGlouton : %d\n", averageScore(robot, false));
    
    /* affichage temoin */
    for(int i = 0; i < NB_REGLES; i++){
	for(int j = 0; j < TAILLE_ETAT + 2; j++) {
	    printf("%d ", temoin[i][j]);
	}
	printf("\n");
    }

    printf("score du bot temoin : %d\n", averageScore(temoin, false));

    char filename[100] = "";
    sauvegarderBot(robot, filename);

}
