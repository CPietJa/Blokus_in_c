#include <stdio.h>
#include <string.h>

#include "../include/commun.h"
#include "../include/affichage.h"
#include "../include/joueur.h"
#include "../include/carre.h"

static void afficher_carre(Couleur couleur) {

    switch(couleur) {
     
        case BLEU:
            printf(COULEUR_BLEU);
            printf("██");
            printf(FIN_COULEUR);
            break;
        case JAUNE:
            printf(COULEUR_JAUNE);
            printf("██");
            printf(FIN_COULEUR);
            break;
        case ROUGE:
            printf(COULEUR_ROUGE);
            printf("██");
            printf(FIN_COULEUR);
            break;
        case VERT:
            printf(COULEUR_VERT);
            printf("██");
            printf(FIN_COULEUR);
            break;
        case VIDE:
        default:
            printf("  ");
    }
}

void afficher_plateau(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU]) {
    
    for (int i = 0; i < TAILLE_PLATEAU; i++) {
        printf("%3d ", TAILLE_PLATEAU - i);
        for (int j = 0; j < TAILLE_PLATEAU; j++) {
            afficher_carre(pl[i][j]);
        }
        printf("\n");
    }


    printf("    ");
    for (int i = 1; i <= TAILLE_PLATEAU; i++) {
        printf("%2d", i);
    }
    printf("\n");
}

void afficher_pieces_dispo(Joueur* j) {

    int n = joueur_nb_piece_restantes(j);
    Piece* l = joueur_liste_piece(j);
    Couleur couleur = joueur_couleur(j);

    // Contient une rangée de 4 pièces
    // Chaque pièce est représenté dans une matrice [5][5]
    Couleur rangee[4][5][5] = {0};

    // Représente le numéro de la pièce en cours
    int i = 0;

    // Pour chaque rangée
    for (int r = 0; i < n; r++) {
	
        // Représentation des pièces dans leur matrice
        for (int j = 0; j < 4; j++) {
            if (i < n) {
                Carre * init = l->liste_carre;
                Carre * c = init;
                do {
	            rangee[j][4 - c->x][c->y] = couleur;
                } while ((c = carre_get_suiv(c)) != init);
                l = piece_suivant(l);
            }
            i++;
        }

        i -= 4;

        // Affichage de la rangée
        for (int y = 0; y < 5; y++) {
            for (int p = 0; p < 4; p++) {
                
                if (y == 1 && i < n) {
                    printf("%2d) ", ++i);
                }
                else {
                    printf("    ");
                }

                for (int x = 0; x < 5; x++) {
                    afficher_carre(rangee[p][y][x]);
                    rangee[p][y][x] = 0;
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}

void afficher_choix_orientation(Piece* p) {

    // Matrice qui contiendra une représentation de la pièce
    Couleur mp[5][5] = {0};
    Carre * init = p->liste_carre;
    Carre * c = init;

    // Représentation de la pièce dans la matrice
    do {
        mp[4 - c->x][c->y] = BLEU;
    } while ((c = carre_get_suiv(c)) != init);
    
    // Affichage de toutes les orientations de la pièce en une ligne
    for (int i = 0; i < 5; i++) {
        for (int n = 1; n <= 4; n++) {
            if (i == 1) {
                printf("%d) ", n);
            }
            else {
                printf("   ");
            }

            if (n == 1) {
                for (int j = 0; j < 5; j++) {
                    afficher_carre(mp[i][j]);
                }
            }
            else if (n == 2) {
                for (int j = 0; j < 5; j++) {
                    afficher_carre(mp[4-i][j]);
                }
            }
            else if (n == 3) {
                for (int j = 0; j < 5; j++) {
                    afficher_carre(mp[4-i][4-j]);
                }
            }
            else if (n == 4) {
                for (int j = 0; j < 5; j++) {
                    afficher_carre(mp[i][4-j]);
                }
            }
            printf(" ");
        }
        printf("\n");
    }
}

void afficher_scores(Joueur* j) {
    
    // Variable pour garder le premier joueur
    Joueur * pj = j;

    // Affichage des noms avec la bonne couleur
    do {
        printf("|");
        switch (joueur_couleur(j)) {

            case BLEU:
                printf(COULEUR_BLEU);
                break;
            case JAUNE:
                printf(COULEUR_JAUNE);
                break;
            case ROUGE:
                printf(COULEUR_ROUGE);
                break;
            case VERT:
                printf(COULEUR_VERT);
        }
        int marges = (TAILLE_PSEUDO - strlen(j->pseudo)) / 2;
        int decalage = strlen(j->pseudo)%2;
        printf("%*s%s%*s", marges + decalage, "", j->pseudo, marges, "");
        printf(FIN_COULEUR);
        j = joueur_suivant(j);
    } while (j != pj);
    printf("|\n");
    
    // Affichage de ligne avec juste les barres
    printf("|%*s|%*s|%*s|%*s|\n", TAILLE_PSEUDO, "", TAILLE_PSEUDO, "", TAILLE_PSEUDO, "", TAILLE_PSEUDO, "");
    
    // Affichage des scores
    do {
        printf("|");
        char score[TAILLE_PSEUDO];
        sprintf(score, "%d", j->score);
        int marges = (TAILLE_PSEUDO - strlen(score)) / 2;
        int decalage = strlen(score)%2;
        printf("%*s%s%*s", marges + decalage, "", score, marges, "");
        j = joueur_suivant(j);
    } while (j != pj);
    printf("|\n");
}

void afficher_resultats(Joueur* j) {

    
    // Variable pour garder le premier joueur
    Joueur * pj = j;
    // Variable pour savoir si le joueur a été placé dans le classement
    int p = 0;
    // Variable pour le décalage des joueurs
    Joueur * tmp1;
    Joueur * tmp2;
    
    // Tri des joueurs par ordre croissant de score dans un tableau
    Joueur * classement[4] = {NULL};

    do {
        p = 0;
        for (int i = 0; i < 4 && !p; i++) {
            if (classement[i] == NULL) {
                classement[i] = j;
                p++;
            }
            else if (j->score > classement[i]->score) {
                // Placement du joueur et décalage des autres
                tmp1 = j;
                while (i < 4) {
                    tmp2 = classement[i];
                    classement[i] = tmp1;
                    tmp1 = tmp2;
                    i++;
                }
            }
        }
        j = joueur_suivant(j);
    } while (j != pj);

    // Affichage des résultats
    printf("%*s%s%*s\n", 12, "", "RESULTATS DE LA PARTIE", 12, "");
    printf("\n");
    for (int i = 0; i < 4 && classement[i]; i++) {

        switch (joueur_couleur(classement[i])) {

            case BLEU:
                printf(COULEUR_BLEU);
                break;
            case JAUNE:
                printf(COULEUR_JAUNE);
                break;
            case ROUGE:
                printf(COULEUR_ROUGE);
                break;
            case VERT:
                printf(COULEUR_VERT);
        }

        // Gestion des égalités
        int r = i; // rang réel
        while (r-1 >= 0 && classement[r]->score == classement[r-1]->score) {
            r--;
        }
        r++; // +1 pour l'affichage
        
        // Affichage rang
        char rang[TAILLE_PSEUDO];
        if (r == 1) {
            sprintf(rang, "Vainqueur");
        }
        else {
            sprintf(rang, "%deme", r);
        }
        int marges = (TAILLE_PSEUDO/2 - strlen(rang)) / 2;
        int decalage = strlen(rang)%2;
        printf("%*s%s%*s", marges + decalage, "", rang, marges, "");

        // Affichage ex aequo ou non
        if (r != i+1) {
            printf(" ex aequo ");
        }
        else {
            printf("          ");
        }
    
        // Affichage pseudo
        char pseudo[TAILLE_PSEUDO];
        strcpy(pseudo,classement[i]->pseudo);
        marges = (TAILLE_PSEUDO - strlen(pseudo)) / 2;
        decalage = strlen(pseudo)%2;
        printf("%*s%s%*s", marges + decalage, "", pseudo, marges, "");
        
        // Affichage score
        char score[10];
        sprintf(score, "%d", classement[i]->score);
        marges = (10 - strlen(score)) / 2;
        decalage = strlen(score)%2;
        printf("%*s%s%*s", marges + decalage, "", score, marges, "");
        printf(FIN_COULEUR);
        printf("\n");
    }
}
