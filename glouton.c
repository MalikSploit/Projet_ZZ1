#include "glouton.h"
#include <stdio.h>

bot creerBot(){
    bot robot;

    for (int i = 0; i < NB_REGLES; i++) {
	for (int j = 0; j <= 2; j++) {
	    robot[i][j] = rand() % 5;
	    if (robot[i][j] == 0) {
		robot[i][j] = -1;
	    }
	}
	
	robot[i][3] = rand() % 6;
	if (robot[i][3] == 0) {
	    robot[i][3] = -1;
	}

	robot[i][4] = rand() % 3 - 1;
	robot[i][5] = (rand() % 5) + 1;
    }

    return robot;

}

/* Jeu(robot) */

bot algoGlouton(bot unBot){

  int indices[NOMBRE_INDICES];
  initIndices(indices);
  

    for (int i = 0; i < NOMBRE_GLOUTON; i++) {



      /* on repermute les probas */
      permutation(indices);
    }

    printf("Score du meilleur bot d'algoGlouton %d", Jeu(unBot));
    return unBot;

}

void * permutation(int * indices){


}

void * initIndices(int * indices){

  for (int i = 0; i < NOMBRE_INDICES; i++) {
    indices[i] = i;
  }
  permutation(indices);
}

// changement d'etat par swap de deux valeurs aleatoires
void permutation(int * actuel) {

        int i = rand() % (NOMBRE_INDICES);
	int j = rand() % (NOMBRE_INDICES);
	while(j == i) j = rand() % (NOMBRE_INDICES);

	swap(&actuel[i], &actuel[j]);

}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

