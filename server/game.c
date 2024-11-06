#include <stdio.h>
#include <stdbool.h>

#define NB_TROUS 12
#define NB_GRAINES 48
#define CAPTURE_MIN 25

void initPlateau(int plateau[]) {
    for (int i = 0; i < NB_TROUS; i++) {
        plateau[i] = 4;
    }
}

void afficherLeScore(int scoreJoueur1, int scoreJoueur2) {
    printf("Score - Joueur 1: %d, Joueur 2: %d\n", scoreJoueur1, scoreJoueur2);
}

void afficherPlateau(int plateau[]) {
    printf("Camp joueur 2 : ");
    for (int i = NB_TROUS - 1; i >= NB_TROUS / 2; i--) {
        printf("[%d] ", plateau[i]);
    }
    printf("\nCamp joueur 1 : ");
    for (int i = 0; i < NB_TROUS / 2; i++) {
        printf("[%d] ", plateau[i]);
    }
}

bool peutJouer(int plateau[], int camp) {
    int debut = (camp == 1) ? 0 : NB_TROUS / 2;
    int fin = debut + NB_TROUS / 2;
    for (int i = debut; i < fin; i++) {
        if (plateau[i] > 0) return true;
    }
    return false;
}

int jouerCoup(int plateau[], int trou, int joueur, int* scoreJoueur, int sensRotation) {
    int graines = plateau[trou];
    plateau[trou] = 0;
    int position = trou;

    while (graines > 0) {
        position = (position + (sensRotation == 1 ? 1 : -1) + NB_TROUS) % NB_TROUS;
        if (position != trou) {
            plateau[position]++;
            graines--;
        }
    }

    while (position >= NB_TROUS / 2 && position < NB_TROUS && (plateau[position] == 2 || plateau[position] == 3)) {
        *scoreJoueur += plateau[position];
        plateau[position] = 0;
        position = (position + (sensRotation == 1 ? -1 : 1) + NB_TROUS) % NB_TROUS;
    }
    
    return *scoreJoueur >= CAPTURE_MIN;
}

int main() {
    int plateau[NB_TROUS];
    int scoreJoueur1 = 0, scoreJoueur2 = 0;
    int joueur = 1; // Le joueur 1 commence toujours la partie
    int sensRotation;

    printf("Choisissez le sens de rotation (1 pour anti-horaire, 2 pour horaire) : ");
    scanf("%d", &sensRotation);

    initPlateau(plateau);
    
    while (scoreJoueur1 < CAPTURE_MIN && scoreJoueur2 < CAPTURE_MIN && peutJouer(plateau, 1) && peutJouer(plateau, 2)) {
        afficherPlateau(plateau);
        afficherLeScore(scoreJoueur1, scoreJoueur2);
        
        printf("Joueur %d, choisissez un trou (0-5 pour joueur 1, 6-11 pour joueur 2): ", joueur);
        int trou;
        scanf("%d", &trou);

        if ((joueur == 1 && trou >= 0 && trou < NB_TROUS / 2 && plateau[trou] > 0) ||
            (joueur == 2 && trou >= NB_TROUS / 2 && trou < NB_TROUS && plateau[trou] > 0)) {
            if (jouerCoup(plateau, trou, joueur, joueur == 1 ? &scoreJoueur1 : &scoreJoueur2, sensRotation)) {
                break;
            }
            joueur = 3 - joueur;
        } else {
            printf("Coup invalide. Essayez de nouveau.\n");
        }
        printf("\n-------------------------------------------------------------\n");
    }

    afficherPlateau(plateau);
    afficherLeScore(scoreJoueur1, scoreJoueur2);
    
    if (scoreJoueur1 > scoreJoueur2) {
        printf("Joueur 1 gagne!\n");
    } else if (scoreJoueur2 > scoreJoueur1) {
        printf("Joueur 2 gagne!\n");
    } else {
        printf("Match nul!\n");
    }

    return 0;
}