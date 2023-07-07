#include "glouton.h"

int valeurPossibleRegle[TAILLE_ETAT + 2][7] = {
        {5, -1, 1, 2, 3, 4},
        {5, -1, 1, 2, 3, 4},
        {5, -1, 1, 2, 3, 4},
        {6, -1, 1, 2, 3, 4, 5},
        {3, -1, 0, 1},
        {5, 1, 2, 3, 4, 5}
};

void initialiserBot(bot unBot){

    for (int i = 0; i < NB_REGLES; i++) {
        for (int j = 0; j <= 2; j++) {
            unBot[i][j] = rand() % 5;
            if (unBot[i][j] == 0) {
                unBot[i][j] = -1;
            }
        }

        unBot[i][3] = rand() % 6;
        if (unBot[i][3] == 0) {
            unBot[i][3] = -1;
        }

        unBot[i][4] = rand() % 3 - 1;
        unBot[i][5] = (rand() % 5) + 1;
    }

}

void * jeuPourpThread(void * parameters) {
    bot * robot = (bot *) parameters;
    int * score = malloc(sizeof(int));
    *score = Jeu(*robot);
    return score;
}

int jeuPourThread(void * parameters) {
    bot * robot = (bot *) parameters;
    return Jeu(*robot);
}

int averageScore(bot unBot, bool multithread) {
    int scoreTotal = 0;
    int nbScores = multithread ? NUM_THREADS : NUM_REPETITIONS;

    if(multithread) {
        pthread_t threads[NUM_THREADS];

        for (int thread = 0; thread < NUM_THREADS; thread++) {
            thrd_create(&threads[thread], jeuPourThread, &unBot);
        }

        for (int thread = 0; thread < NUM_THREADS; thread++) {
            int result;
            thrd_join(threads[thread], &result);
            scoreTotal += result;
        }
    } else {
        for(int i = 0; i < NUM_REPETITIONS; i++) {
            scoreTotal += Jeu(unBot);
        }
    }

    return scoreTotal / nbScores;

}

void algoGlouton(bot unBot){

    int indices[NOMBRE_INDICES];
    initIndices(indices);

    initialiserBot(unBot);

    int ligne;
    int colonne;

    for (int i = 0; i < NOMBRE_GLOUTON; i++) {
        for (int j = 0; j < NOMBRE_INDICES; j++) {
            colonne = indices[j] % (TAILLE_ETAT + 2);
            ligne = indices[j] / (TAILLE_ETAT + 2);

            int meilleurScore = MAX_ITER;
            int meilleurValeur = valeurPossibleRegle[colonne][1];
            int scoreActuel;

            for (int k = 1; k <= valeurPossibleRegle[colonne][0]; k++) {

                unBot[ligne][colonne] = valeurPossibleRegle[colonne][k];

                scoreActuel = averageScore(unBot, false);

                if (scoreActuel < meilleurScore)
                {
                    meilleurScore = scoreActuel;
                    meilleurValeur = valeurPossibleRegle[colonne][k];
                }
            }
            printf("score trouve %d\n", meilleurScore);

            unBot[ligne][colonne] = meilleurValeur;
        }

        /* on repermute les probas */
        melangeIndices(indices);
    }

    /* printf("Score du meilleur bot d'algo Glouton %d\n", Jeu(unBot)); */

}

void initIndices(int * indices){

    for (int i = 0; i < NOMBRE_INDICES; i++) {
        indices[i] = i;
    }
    melangeIndices(indices);
}

void melangeIndices(int * indices){

    for (int j = NOMBRE_INDICES - 1; j > 0; j--) {
        int k = rand() % (j + 1);
        swap(&indices[j], &indices[k]);
    }

}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

