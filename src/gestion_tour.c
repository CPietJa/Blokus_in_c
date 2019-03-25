/**
 *	\file gestion_tour.c
 *	\brief Fichier de fonctions permettant de gérer le tour d'un joueur
 *  \details Contient les fonctions demandant la pièce à jouer, son orientation et ses coordonnées au joueur
     ainsi que les fonctions de vérification de position et de couleur et de pose de celle-ci
 *  \author RIGUIDEL Hugo
 *	\version 1.0
 *	\date 12/03/2019
 */

#include "../include/gestion_tour.h"
#include "../include/affichage.h"
#include "../include/carre.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * \fn Piece* demander_piece(Joueur* j)
 * \brief Demande au joueur la pièce qu'il souhaite jouer
 * \details Affiche les pièces qu'il reste au joueur avec un numéro et lui demande de choisir quelle pièce jouer
    et attend qu'il entre le numéro
 * \param j Pointeur sur le joueur devant choisir sa pièce
 */
/* Affiche les pièces qu'il reste au joueur avec un numéro et lui demande de choisir quelle pièce jouer et attend qu'il entre le numéro */
Piece* demander_piece(Joueur* j)
{
    int nb, i;
    Piece* p = joueur_liste_piece(j);

    printf("Quelle pièce voulez-vous jouer ?\n");
    afficher_pieces_dispo(j);
    scanf("%d", &nb);

    /* Tant que le joueur entre un numéro ne correspondant pas à une pièce disponible, on redemande un numéro valide */
    while((nb <= 0) && (nb > joueur_nb_piece_restantes(j)))
    {
        printf("Veuillez entrer une valeur correcte\n");
        printf("Quelle pièce voulez-vous jouer ?\n");
        afficher_pieces_dispo(j);
        scanf("%d", &nb);
    }

    /* on sélectionne la bonne pièce */
    for(i = 1; i < nb; i++)
        p = piece_suivant(p);

    return p;
}

/**
 * \fn void initialiser_matrice(int matrice[5][5])
 * \brief Fonction d'initialisation d'une matrice
 * \details Fonction initialisant une matrice de taille 5*5 en la remplissant de 0
 * \param matrice La matrice à initialiser
 */
void initialiser_matrice(int matrice[5][5])
{
    int i, j;

    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 5; j++)
            matrice[i][j] = 0;
    }
}

/* Fonction affectant une pièce à une matrice
     remplit la matrice de 1 en fonction des coordonnées des carres constituant la pièce */

/**
 * \fn void affecter_matrice(int matrice[5][5], Carre* c)
 * \brief Fonction d'initialisation d'une matrice
 * \details Fonction affectant une pièce à une matrice en remplissant la matrice de 1 en fonction des
    coordonnées des carres constituant la pièce
 * \param matrice La matrice à laquelle affecter la pièce
 * \param c Le premier carre de la pièce à affecter
 */
void affecter_matrice(int matrice[5][5], Carre* c)
{
    Carre* c2 = c;
    c2 = carre_get_suiv(c2);

    while(c2 != c)
    {
        matrice[carre_get_x(c2)][carre_get_y(c2)] = 1;
        c2 = carre_get_suiv(c2);
    }

    matrice[carre_get_x(c2)][carre_get_y(c2)] = 1;
}

/* Fonction permettant d'afficher une matrice 5*5

void afficher_matrice(int matrice[5][5])
{
    int i, j;

    for(i = 4; i >= 0; i--)
    {
        for(j = 0; j < 5; j++)
            printf("%d", matrice[i][j]);

        printf("\n");
    }

    printf("\n");
}
*/

/**
 * \fn void demander_orientation(Piece* p, Joueur* j)
 * \brief Demande au joueur l'orientation de la pièce qu'il souhaite jouer
 * \details Affiche les 4 orientations possibles de la pièce au joueur avec un numéro et attend qu'il entre un numéro.
    Modifie également les coordonnees relatives des carres constituant la pièce une fois l'orientation choisie
 * \param p Pointeur sur la pièce que le joueur souhaite jouer
 * \param j Pointeur sur le joueur devant jouer
 */
void demander_orientation(Piece* p, Joueur* j)
{
    int nb;
    Carre* c = piece_liste_carre(p);

    afficher_choix_orientation(p, j);
    printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
    scanf("%d", &nb);
    nb--;

    /* tant que le joueur n'entre pas un numéro d'orientation correct, on lui redemande un numéro valide */
    while((nb < HAUT) || (nb > GAUCHE))
    {
        printf("Veuillez entrer une orientation correcte\n");
        printf("Dans quelle orientation voulez-vous jouer la pièce ? :\n");
        afficher_choix_orientation(p, j);
        scanf("%d", &nb);
        nb--;
    }
    piece_pivoter(nb, c);
}

/**
 * \fn int verification_position(Couleur pl[20][20], int x, int y, Piece* p)
 * \brief Vérifie si la pièce peut être placée
 * \details Vérifie si chacun des carrés constituant la pièce peuvent être placés (La case qu'ils occuperont
    doit être VIDE)
 * \param pl Plateau de jeu
 * \param x Coordonnée x du carre d'origine de la pièce (coordonnée entrée par l'utilisateur)
 * \param y Coordonnée y du carre d'origine de la pièce (coordonnée entrée par l'utilisateur)
 * \param p Pointeur sur la pièce que l'utilisateur souhaite jouer
 */
int verification_position(Couleur pl[20][20], int x, int y, Piece* p)
{
    Carre* c = piece_liste_carre(p);

    /* Vérifie que chaque carré d'une pièce se situent sur une case VIDE */
    do
    {
        if(pl[x+carre_get_x(c)][y+carre_get_y(c)] != VIDE)
        {
            return 0;
        }
        c = carre_get_suiv(c);

    } while(c != piece_liste_carre(p));

    return 1;
}

/* Fonction qui vérifie si aucun Carre de la Couleur du Joueur n'est adjacant aux Carre que le Joueur veut poser
	et qu'il y a au moins un Carre de la Couleur du Joueur qui est en diagonale d'un Carre que le Joueur veut poser */

/**
 * \fn int verification_couleur(Couleur pl[20][20], int x, int y, Couleur col, Piece* p)
 * \brief Vérifie la position des pièces d'une même couleur entre elles
 * \details Fonction qui vérifie si aucun carré de la couleur du Joueur n'est adjacant aux carrés que le joueur veut
    poser et qu'il y a au moins un carré de la couleur du Joueur qui est en diagonale d'un carré que le joueur veut
    poser
 * \param pl Plateau de jeu
 * \param x Coordonnée x du carre d'origine de la pièce entrée par le joueur
 * \param y Coordonnée y du carre d'origine de la pièce entrée par le joueur
 * \param col Couleur du joueur souhaitant jouer la pièce
 * \param p Pointeur sur la pièce que l'utilisateur souhaite jouer
 */
int verification_couleur(Couleur pl[20][20], int x, int y, Couleur col, Piece* p)
{
    Carre* c = piece_liste_carre(p);
    int angle = 0;

    do
    {
	/* Vérifie qu'il n'y a aucun Carre adjacant aux Carre que le Joueur pose */
        if (pl[x + carre_get_x(c) - 1][y + carre_get_y(c)] == col || /* Au dessus */
            pl[x + carre_get_x(c) + 1][y + carre_get_y(c)] == col || /* En dessous */
            pl[x + carre_get_x(c)][y + carre_get_y(c) - 1] == col || /* A gauche */
            pl[x + carre_get_x(c)][y + carre_get_y(c) + 1] == col) /* A droite */
        {
            return 0;
        }

	/* Vérifie qu'il y a au moins un Carre que le Joueur pose qui touche diagonalement un Carre déjà posé de même Couleur */
        if ((pl[x + carre_get_x(c) - 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Haut - Gauche */
            (pl[x + carre_get_x(c) + 1][y + carre_get_y(c) - 1] == col) || /* Diagonale Bas - Gauche */
            (pl[x + carre_get_x(c) - 1][y + carre_get_y(c) + 1] == col) || /* Diagonale Haut - Droit */
            (pl[x + carre_get_x(c) + 1][y + carre_get_y(c) + 1] == col)) /* Diagonale Bas - Droit */
        {
            angle = 1;
        }

        c = carre_get_suiv(c);

    } while(c != piece_liste_carre(p));

    /* Si au moins un Carre est en diagonale d'un Carre de même Couleur déjà posé */
    if(angle)
    {
        return 1;
    }

    /* Si aucun return n'a été fait précédemment */
    return 0;
}

/* Demande au joueur les coordonnees ou il désire jouer sa pièce */
/* Tant qu'il n'est pas possible de jouer aux coords, redemande des coordonnees valides */

/**
 * \fn void choisir_coordonnee(Couleur pl[20][20], Piece* pi, int* x, int* y, Joueur* j)
 * \brief Demande au joueur à quelles coordonnées il souhaite jouer la pièce
 * \details Demande au joueur les coordonnees ou il désire jouer sa pièce
    Tant qu'il n'est pas possible de jouer aux coords, redemande des coordonnees valides
    poser
 * \param pl Plateau de jeu
 * \param pi Pointeur sur la pièce à placer
 * \param x Variable dans laquelle stocker la coordonnée x du carre d'origine de la pièce
 * \param y Variable dans laquelle stocker la coordonnée y du carre d'origine de la pièce
 * \param j Pointeur sur le joueur choisissant les coordonnees
 */
void choisir_coordonnee(Couleur pl[20][20], Piece* pi, int* x, int* y, Joueur* j)
{
    int x_depart;
    int y_depart;
    Carre* c;

    /* Récupère les coordonnees de départ de chaque joueur en fonction de sa couleur */
    switch(joueur_couleur(j))
    {
        case BLEU:
            x_depart = BLUE_X;
            y_depart = BLUE_Y;
            break;

        case JAUNE:
            x_depart = YELLOW_X;
            y_depart = YELLOW_Y;
            break;

        case ROUGE:
            x_depart = RED_X;
            y_depart = RED_Y;
            break;

        case VERT:
            x_depart = GREEN_X;
            y_depart = GREEN_Y;
            break;

        default:
            break;
    }

    /* Si la Piece que le Joueur pose est sa toute première Piece, doit jouer dans son coin */
    if(joueur_nb_piece_restantes(j) == NB_PIECES)
    {
        int coin = 0;
        Carre* c2;

        while(!coin)
        {
            c = piece_liste_carre(pi);

            int valide;

            /* Tant que les coordonnées saisies ne sont pas dans le Plateau */
            do
            {
    	        c2 = piece_liste_carre(pi);

                valide = 1;

        		printf("Vous devez jouer dans votre coin\n");
        		printf("A quelles coordonnees voulez-vous jouer la pièce ? (1 a 20) :\n");
        		printf("Entrez la ligne : ");
        		scanf("%d", x);
        		printf("Entrez la colonne : ");
        		scanf("%d", y);

                /* Repasse les coordonnées utilisateur en coordonnées matricielles */
        		*x = *x - 1;
        		*y = *y - 1;

                /* Pour chaque carré constituant la pièce... */
                do
                {
        		    /* ... si il n'est pas dans le plateau, alors les coordonnees ne sont pas valides */
        		    if(((*x + carre_get_x(c) < 0) || (*x + carre_get_x(c) > 19)) || ((*y + carre_get_y(c) < 0) || (*y + carre_get_y(c) > 19)))
        		    {
                            	valide = 0;
        		    }

        		    c = carre_get_suiv(c);

                } while(c != c2);

            } while((((*x < 0) || (*x > 19)) || ((*y < 0) || (*y > 19))) || !(valide));

            /* Si aucun des carrés constituant la pièce n'occupe la case de départ du joueur, alors les coordonnées
               ne sont pas valides */
            do
            {
                if((*y + carre_get_y(c) == y_depart) && (*x + carre_get_x(c) == x_depart))
                {
                    coin = 1;
                }

                c = carre_get_suiv(c);
            } while(c != c2);
        }
    }
    else
    {
        c = piece_liste_carre(pi);
        Carre *c2 = c;
        carre_get_suiv(c);

        int dans_plateau = 1;

        /* Tant que les coordonnées ne sont pas dans le plateau, on redemande des coordonnées valides */
        do
        {
            printf("A quelles coordonnees voulez-vous jouer la pièce ? :\n");
        	printf("Entrez la ligne : ");
        	scanf("%d", x);
        	printf("Entrez la colonne : ");
        	scanf("%d", y);

            /* Repasse les coordonnées utilisateur en coordonnées matricielles */
       		*x = *x - 1;
            *y = *y - 1;

            /* On vérifie pour chaque carré constituant la pièce */
            while(c != c2)
            {
                if(((*x < 0) || (*x > 19)) || ((*y < 0) || (*y > 19)))
                {
                    dans_plateau = 0;
                }
            }
        } while(!dans_plateau);

        /* Si les conditions de pose de la pièce ne sont pas remplies, on redemande des coordonnées valides */
        if(!verification_position(pl, *x, *y, pi) || !verification_couleur(pl, *x, *y, joueur_couleur(j), pi))
        {
            printf("Impossible de placer la piece aux coordonnees indiquees\n");
            *x = *y = -1;
        }
    }
}

/**
 * \fn void poser_piece(Couleur pl[20][20], Piece* pi, Joueur* j, int x, int y)
 * \brief Pose une pièce
 * \details Pose la pièce que le joueur a choisi aux coordonnees indiquées
 * \param pl Plateau de jeu
 * \param pi Pointeur sur la pièce à poser
 * \param x Coordonnée x du carre d'origine de la pièce entrée par le joueur
 * \param y Coordonnée y du carre d'origine de la pièce entrée par le joueur
 */
void poser_piece(Couleur pl[20][20], Piece* pi, Joueur* j, int x, int y)
{
    Carre* c = piece_liste_carre(pi);
    Piece** p = &(j->liste_piece);
    Piece* pivot = *p;

    /* Remplit les cases où l'on pose la pièce avec la couleur du joueur */
    do
    {
        pl[x+carre_get_x(c)][y+carre_get_y(c)] = joueur_couleur(j);
        c = carre_get_suiv(c);
    } while(c != piece_liste_carre(pi));

    while ((*p) != pi)
    {
        *p = piece_suivant(*p);
    }

	if (pivot == *p)
		pivot = NULL;

    /* Si la dernière pièce est le petit carré, alors le joueur gagne 5 points supplementaires */
    if(joueur_nb_piece_restantes(j) == 1 && c == carre_get_suiv(c))
        j->score += 5;

    liste_piece_suppr_elem(p);

    /* On redécale la liste de pièce pour la remettre dans l'ordre */
    while (pivot != NULL && (*p) != pivot)
        *p = piece_suivant(*p);

    j->liste_piece = *p;
}
















/* */
