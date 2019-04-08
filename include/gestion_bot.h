#ifndef GESTION_BOT_H
#define GESTION_BOT_H

#include "couleur.h"
#include "joueur.h"
#include "carre.h"
#include "commun.h"

typedef struct coup Coup;

/**
        \struct coup
        \brief Structure d'un Coup
*/
struct coup {
	Piece* piece_copie; /**< Copie de la Piece du Coup */
	Piece* piece_origine; /**< Piece de la liste du Joueur du Coup */
	Couleur c; /**< Couleur de la Piece */
	int x; /**< Coordonnée x à laquelle la Piece a été évaluée */
	int y; /**< Coordonnée y à laquelle la Piece a été évaluée */
	int valeur_coup; /**< Valeur d'évaluation du Coup */
};

/* Accesseurs */
Piece* coup_piece (Coup*);
Piece* coup_piece_origine (Coup*);
Couleur coup_couleur (Coup*);
int coup_coord_x (Coup*);
int coup_coord_y (Coup*);
int coup_valeur (Coup*);
Coup* coup_copie(Coup*);

int gestion_tour_bot (Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur*);

Coup* bot_jouer_tour (Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur*);

int bot_jouer (Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur*, int); /* int: la profondeur de coup (==nb_coup en prévision) */
int adversaire_jouer (Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur*, Joueur*, int); /* Premier Joueur: le bot qui doit jouer. Deuxième Joueur: L'adversaire pour lequel on projette le coup */

int eval_coup_bot (Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup*, Joueur*);
int eval_nb_carres_poses(Coup*);
int eval_emplacement_piece(Coup*);
int eval_cases_dispo(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup*);
int eval_nb_coups_bloques(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup*);
int eval_nb_nouveaux_coups(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup*, Joueur*);

#endif

/* Evaluer la valeur de la case (moins il y a de cases prises autour, plus elle a de valeur) */
