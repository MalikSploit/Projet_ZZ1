#include "simulation.h"

void recupererNomBot(SDL_Renderer* renderer, int * quitterJeu, char *filename) {
  FILE *fichier;

  /* demandeNom du bot */

    char *pathBackgroud = "Images/DemanderBot_Background.jpg";
    char *pathMessage = "Images/Entrer-votre-Bot.png";
    char *nomBot;

  do {
    nomBot = DemanderQqch(renderer, quitterJeu, pathBackgroud, pathMessage);
    sprintf(filename, "bots/%s", nomBot);
    fichier = fopen(filename, "r");
    if (fichier == NULL) {
      printf("Le fichier n'existe pas ou n'a pas pu être ouvert. Veuillez réessayer.\n");
    }
  } while (fichier == NULL);

  // Fermer le fichier, car nous vérifions seulement s'il existe
  fclose(fichier);
}

void recupererBot(SDL_Renderer* renderer, int * quitterJeu, bot leBot, char * filename) {
  FILE *fichier;

  recupererNomBot(renderer, quitterJeu, filename);

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

}
