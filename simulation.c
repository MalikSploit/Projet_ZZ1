#include "simulation.h"

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
