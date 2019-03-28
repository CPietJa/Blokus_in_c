#include "../include/gestion_tour_bot.h"
#include "../include/gestion_tour.h"
#include "../include/couleur.h"
#include "../include/joueur.h"

#include <time.h>

static int poser_piece_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup) {
    if(!piece_hors_liste(coup_piece(coup)))
    {
        Carre* c = piece_liste_carre(coup_piece(coup));
        Piece** p = &(j->liste_piece);
        Piece* pivot = *p;

        do
        {
            pl[x+carre_get_x(c)][y+carre_get_y(c)] = coup_couleur(coup);
            c = carre_get_suiv(c);
        } while(c != piece_liste_carre(pi));
/* TODO */

    }
}

int eval_coup_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Coup* coup) {
	Couleur pl2[TAILLE_PLATEAU][TAILLE_PLATEAU];

	int i, j;
	for (i = 0; i < TAILLE_PLATEAU; i++)
		for (j = 0; j < TAILLE_PLATEAU; j++)
			pl2[i][j] = pl[i][j];

	poser_piece_bot(pl2, coup);
} /* TODO */

int gestion_tour_bot(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot) {
	Piece* p = bot_jouer_tour(pl, bot);
        poser_piece_sdl(pl, *p, bot, x, y);
}

int bot_jouer(Couleur pl[][], Joueur bot, int profondeur)
{
    if(profondeur)
    {

    }
}

int adversaire_jouer(Couleur pl[][], Joueur bot, Joueur joueur, int profondeur)
{

}

/* Gestion du premier tour du bot */
Coup* bot_jouer_tour(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur* bot)
{
    int i, j, k, nb;

    srand(time(NULL));

    /* Création d'un petit carre*/
/*    Piece* carre = malloc(sizeof(Piece));
    carre->liste_carre = piece_petit_carre();
    carre->suiv = carre;
    carre->prec = carre;*/

    Piece * p = joueur_liste_piece(bot);
    Piece * init = p;

    Coup** tab = NULL;
    int compteur_tab = 0;

    /* Pour chaque position de la matrice */
    for(i = 0; i < TAILLE_PLATEAU; i++)
    {
        for(j = 0; j < TAILLE_PLATEAU; j++)
        {
            /* Pour chaque pièces disponibles */
            do
            {
                /* Pour chaque orientation possible */
                for(k = 0; k < 4; k++)
                {
                    /* Si la pièce est posable */
                    if(verifier_coordonnees(pl, p, i, j, bot))
                    {
                        /* On enregistre le coup puis on estime sa valeur */
                        if (tab == NULL)
                            tab = malloc(sizeof(*tab));
                        else
                            tab = realloc(tab, sizeof(*tab) * (compteur + 1));

                        compteur++;

                        tab[compteur] = malloc(sizeof(*tab[compteur]));

                        tab[compteur]->p = piece_copie(p);

			/* Enlève la Piece actuelle de la liste temporairement */
			tab[compteur]->p->prec->suiv = tab[compteur]->p->suiv;

			/* Affecte le Coup dans le tableau */
                        tab[compteur]->x = i;
                        tab[compteur]->y = j;

                        /* tab[compteur]->valeur_coup = eval_coup_bot(pl, bot, tab[compteur]->p); */

                        adversaire_jouer(pl, joueur_suivant(bot));

			/* Remet la Piece dans la liste */
			tab[compteur]->p->prec->suiv = tab[compteur];
                    }

                    changer_orientation(p)
                }

                p = piece_suivant(p);

            } while (p != init);
        }
    }

    Coup* coup = NULL;

    if (compteur) {
        nb = rand() % compteur;

        coup = coup_copie(tab[nb]);
    }

    free_tab_coup(&tab, compteur);

    /* On retourne le coup estimé comme étant le meilleur. NULL si aucun coup n'est possible */
    return coup;
}

static void free_tab_coup(Coup*** tab, int taille)
{
    int i;

    if (*tab != NULL) {
        for(i = 0; i < taille; i++)
        {
            free((*tab)[i]);
        }

        free(*tab);
    }

    *tab = NULL;
}
