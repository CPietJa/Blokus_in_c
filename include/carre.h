#ifndef CARRE_H
#define CARRE_H

/* Structure d'un carré */
typedef struct carre Carre;

struct carre {
	int x, y; /* Coordonnées x et y par rapport au point de référence d'une pièce */
	Carre* suiv;
};

/* Procédures d'accès pour un carré */

/* Permet de récupérer la coordonnée x d'un Carre */
int carre_get_x (Carre*);

/* Permet de récupérer la coordonnée y d'un Carre */
int carre_get_y (Carre*);

/* Permet de récupérer le carré suivant d'une pièce */
Carre* carre_get_suiv (Carre*);

int carre_hors_liste(Carre*);

void carre_reinit_tab(char tab[5][5]);

void carre_afficher(Carre*);

/* Libère un Carre */
void carre_free(Carre**);

/* Detruit une pièce entière */
void carre_detruire(Carre**);

/* Supprime tous les carrés d'une pièce */
void liste_piece_suppr_elem(Carre**);



/* Structure d'une pièce (qui est une liste de Carré avec comme point de référence le bas gauche de la pièce)*/
typedef struct piece {
	Carre* liste_carre; /* Premier carré de la liste */
} Piece;

Piece** piece_liste_creation();

void liste_piece_suppr_elem(Carre**);

void piece_liste_detruire(Piece***);

#endif
