/**
	\file carre.c
	\brief Fichier de fonctions pour accesseurs et de modifications pour un Carre et une Piece
	\author WIDMER Alexis
	\version 1.0
	\date 01/03/2019

	Ce fichier permet d'avoir accès aux attributs d'un Carre et d'une Piece, de les créer, de les supprimer ou de les afficher.
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/carre.h"
#include "../include/piece.h"
#include "../include/commun.h"
#include "../include/gestion_tour.h"

/* Procédures d'accès pour un carré */

/* Permet de récupérer la coordonnée x d'un Carre */
/**
	\fn int carre_get_x(Carre* c);
	\brief Permet de récupérer la coordonnée x d'un Carre

	\param c Le Carre* dont on veut la coordonnée y
	\return La coordonnée x (dans la visualisation d'un tableau, ça serait la ligne)
*/
int carre_get_x(Carre* c) {
	return c->x;
}

/* Permet de récupérer la coordonnée y d'un Carre */
/**
	\fn int carre_get_y(Carre* c);
	\brief Permet de récupérer la coordonnée y d'un Carre

	\param c Le Carre* dont on veut la coordonnée y
	\return La coordonnée y (dans la visualisation d'un tableau, ça serait la colonne)
*/
int carre_get_y(Carre* c) {
	return c->y;
}

/* Permet de récupérer le carré suivant d'une pièce */
/**
	\fn Carre* carre_get_suiv(Carre* c);
	\brief Permet de récupérer le Carre suivant d'une liste de Carre

	\param c Le Carre* dont on veut le Carre suivant
	\return Le Carre* suivant (renvoie NULL si le paramètre est NULL)
*/
Carre* carre_get_suiv(Carre* c) {
	if (!carre_hors_liste(c))
		return c->suiv;
	else
		return NULL;
}

/**
	\fn int carre_hors_liste(Carre* c);
	\brief Renvoie VRAI si le Carre* est NULL, FAUX sinon

	\param c Le Carre* dont on veut savoir s'il est NULL ou non
	\return Un entier valant VRAI ou FAUX
*/
int carre_hors_liste(Carre* c) {
	return (c == NULL);
}

/**
	\fn void carre_reinit_tab(char tab[5][5]);
	\brief Permet d'initialiser un tableau char [5][5] (utilisé uniquement dans la fonction d'affichage)

	\param tab Le tableau à initialiser
*/
void carre_reinit_tab(char tab[5][5]) {
	int i, j;

	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			tab[i][j] = ' ';
}

/* Affiche une pièce */
/**
	\fn void carre_afficher(Carre* c);
	\brief Affiche un Carre* (sous forme de '*')

	\param c Le Carre* qu'on veut afficher
*/
void carre_afficher(Carre* c) {
	Carre* init = c;
	char tab[5][5]; /* Tableau représentant le Carre dans un plan 5/5 (pour l'affichage) */
	int i, j;

	if (!carre_hors_liste(c)) {
		carre_reinit_tab(tab);

		/* Tant que le Carre qu'on regarde n'est pas le premier qu'on a eu (== la boucle n'est pas terminée) */
		do {
			tab[4 - c->x][c->y] = '*'; /* Pour centrer la pièce en bas à gauche */
		} while ((c = carre_get_suiv(c)) != init);

		/* Affiche le tableau avec des ' ' (pas de Carre) ou des '*' (un Carre) */
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++)
				putchar(tab[i][j]);
			putchar('\n');
		}
	}
}


/* Libère un Carre */
/**
	\fn void carre_free(Carre** c);
	\brief Libère un Carre (et tout ses attributs)

	\param c L'adresse du Carre* qu'on veut libérer
*/
void carre_free(Carre** c) {
	if (!carre_hors_liste(carre_get_suiv(*c)))
		(*c)->suiv = NULL;

	free(*c);
	(*c) = NULL;
}

/* Detruit une pièce entière */
/**
	\fn void carre_detruire(Carre** c);
	\brief Détruit un Carre* (== tous les Carre d'une liste de Carre)

	\param c L'adresse du Carre* qu'on veut détruire (donc tous les Carre* suivants)
*/
void carre_detruire(Carre** c) {
	Carre** supp = malloc(sizeof(Carre*));
	(*supp) = (*c);

	while (!carre_hors_liste((*supp))) {
/*		carre_afficher((*c));*/
/*printf("Destruction carre\n");*/
		(*supp) = carre_get_suiv((*c));


/*	if (carre_hors_liste((*c)))
		printf("c hors liste\n");
	else
		printf("c pas hors liste\n");*/

		if ((*c)->suiv == (*c))
			(*c) = NULL;
		else
			(*c)->suiv = (*supp)->suiv;

		carre_free(supp);

		(*supp) = (*c);
/*	if (carre_hors_liste((*supp)))
		printf("supp hors liste\n");
	else
		printf("supp pas hors liste\n");*/
	}

	free(supp);
	supp = NULL;
	(*c) = NULL;
}

Piece* piece_hors_liste(Piece* p) {
	return (p == NULL);
}

/**
	\fn Carre* piece_liste_carre(Piece* p);
	\brief Renvoie le premier Carre* de la Piece (== son premier Carre)

	\param p La Piece* dont on veut récupérer la liste de carre
	\return Le premier Carre* de la Piece
*/
Carre* piece_liste_carre(Piece* p) {
	if (piece_hors_liste(p))
		return NULL;

	return p->liste_carre;
}

/**
	\fn Piece* piece_suivant(Piece* p);
	\brief Renvoie la Piece suivante de la Piece

	\param p La Piece* dont on veut récupérer la Piece* suivante
	\return La Piece* suivante
*/
Piece* piece_suivant(Piece* p) {
	if (piece_hors_liste(p))
		return NULL;

	return p->suiv;
}

/**
	\fn Piece* piece_precedent(Piece* p);
	\brief Renvoie la Piece précédente de la Piece

	\param p La Piece* dont on veut récupérer la Piece* précédente
	\return La Piece* précédente
*/
Piece* piece_precedent(Piece* p) {
	if (piece_hors_liste(p))
		return NULL;

	return p->prec;
}

/* Création d'une liste de Piece */
/**
	\fn Piece* piece_liste_creation();
	\brief Crée toutes les Piece du jeu et renvoie la première Piece (le petit carré)

	\return La premiere Piece* créée
*/
Piece* piece_liste_creation () {
	Piece* p = malloc(sizeof(Piece)); /* Alloue l'espace pour une Piece */
	Piece* first = p;

	p->liste_carre = piece_petit_carre(); /* Récupère l'adresse de la liste de carre */
/*	carre_afficher((*p)->liste_carre);*/
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv; /* Change le pointeur */

	p->liste_carre = piece_petite_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_coin();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petite_moy_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_carre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_t();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_moy_grande_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_l();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_s();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grand_l();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grand_t();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_equerre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_long_s();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grand_z();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_grande_barre();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_pouce();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_escalier();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_petit_u();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_autre_1();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_croix();
	p->suiv = malloc(sizeof(Piece));
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */
	p = p->suiv;

	p->liste_carre = piece_autre_2();
	p->suiv = first;;
	p->suiv->prec = p; /* Le précédent de la Piece suivante est cette Piece */

	return first;
}

/**
	\fn void piece_pivoter(int nb, Carre * c) {
	\brief Fait pivoter la pièce de nb fois 90°

	\param nb Nombre de pivots à effectuer
        \param c Pièce à faire pivoter
*/
void piece_pivoter(int nb, Carre * c) {

    int i, j;
    int min_x;
    int min_y;
    Carre * c2 = c;

    /* Pour chaque orientation, on tourne la pièce de 90° un certain nombre de fois
         Orientation 1 : 0°
         Orientation 2 : 90°
         Orientation 3 : 180°
         Orientation 4 : 270°
    */
    while (nb > 0)
    {
        int initiale[5][5];
        initialiser_matrice(initiale);
        affecter_matrice(initiale, c);
        int finale[5][5];
        initialiser_matrice(finale);

        /* Modification des coordonnées des carres de la pièce dans une matrice temporaire */
        for(i = 0; i < 5; i++)
        {
            for(j = 0; j < 5; j++)
            {
                if (initiale[i][j])
                {
                    finale[4 - j][i] = 1;
                }
            }
        }

        min_x = 5;
        min_y = 5;

        /* On calcul les coordonnees minimales des carres constituant la pièce */
        for(i = 0; i < 5; i++)
        {
            for(j = 0; j < 5; j++)
            {
                if(finale[i][j])
                {
                    if(i < min_x)
                        min_x = i;

                    if(j < min_y)
                        min_y = j;

                    c->x = i;
                    c->y = j;
                    c = carre_get_suiv(c);
                }
            }
        }

        c = c2;

        /* On réaffecte les coordonnees minimales des carres constituant la pièce de sorte que x et y >= 0 (On ramène la pièce dans le coin inférieur gauche) */
        do {
            c->x -= min_x;
            c->y -= min_y;
        } while ((c = carre_get_suiv(c)) != c2);
        nb--;
    }
}

/* Supprime la "pos"ième pièce (si elle n'est pas NULL) */
/**
	\fn void liste_piece_suppr_elem(Piece** p);
	\brief Supprime la Piece* courante du paramètre et reconstruit la liste

	\param p L'adresse de la Piece* qu'on souhaite supprimer
*/
void liste_piece_suppr_elem(Piece** p) {/*, int pos) {*/
	if (!piece_hors_liste(p)) {
		Carre** c = &((*p)->liste_carre);
/*		printf("Destruction carres\n");*/
/*		Carre* suiv = carre_get_suiv(*c);*/

		/* Réctifie l'ordre de la liste */

/*printf("Début\n");*/
		Piece* suiv;
		if ((*p) == piece_suivant(*p))
			suiv = NULL;
		else
			suiv = piece_suivant(*p);
/*printf("Milieu\n");*/
		(*p)->prec->suiv = piece_suivant(*p);
/*printf("Fin\n");*/
		(*p)->suiv->prec = piece_precedent(*p);

/*printf("Destructino\n");*/
		carre_detruire(c);

/*printf("Free de c\n");*/

		/* Libère le pointeur de la liste de Carre */
		if (!carre_hors_liste(*c))
			free(*c);

/*		(*c) = suiv;*/
		(*c) = NULL;
/*printf("Free de p\n");*/
		/* Libère la Piece */
		if ((*p) != NULL)
			free(*p);

		(*p) = suiv;

	}
}

/**
	\fn void liste_piece_detruire(Piece** p);
	\brief Supprime toutes les Piece* dans cette liste

	\param p L'adresse d'une Piece* afin de supprimer toute la liste
*/
void liste_piece_detruire(Piece** p) {
	Piece** supp = malloc(sizeof(Piece*));

	(*supp) = (*p);

	/* Tant qu'il reste une Piece à supprimer */
	while (!piece_hors_liste(*supp)) {
		(*supp) = piece_suivant(*p);

		/* Si c'est la dernière Piece (== la suivante est elle-même) */
		if ((*supp) == (*p))
			(*p) = NULL;
		else
			(*p)->suiv = piece_suivant((*supp));

		/* Supprime et libère la Piece courante */
		liste_piece_suppr_elem(supp);

		(*supp) = (*p);
	}

	free(supp);
	supp = NULL;
	(*p) = NULL;
}
/*
		for (i = 0; i < NB_PIECES; i++) {*/
/*			printf("Destruction piece %d\n", i);*/
/*		carre_afficher((*((*p) + i))->liste_carre);*/
/*			liste_piece_suppr_elem(&((*((*p) + i))->liste_carre));
			free(*((*p) + i));
		}

		free(*p);
		(*p) = NULL;
	}
}*/

/**
	\fn int piece_nb_carre(Piece*);
	\brief Renvoie le nombre de Carre d'une Piece

	\param p L'adresse de la Piece* pour laquelle on veut connaître le nombre de Carre
*/
int piece_nb_carre(Piece* p) {
	/* Si la Piece est NULL */
	if (piece_hors_liste(p))
		return 0; /* Il n'y a aucun Carre */

	Carre* c = piece_liste_carre(p);
	Carre* init = c;
	int nb = 0;

	/* Si le Carre n'est pas vide, on compte le nombre de Carre */
	if (!carre_hors_liste(c))
		do {
			nb++;
			init = carre_get_suiv(init);
		} while (init != c);

	return nb;
}
