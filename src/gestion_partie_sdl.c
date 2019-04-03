/**
	*\file gestion_partie_sdl.c
	*\brief Regroupent toutes les fonctions gestion_partie_sdl.c
	*\details Toutes les fonctions qui permettent de gerer une partie de blokus en respectant les règles.
	*\author JODEAU Alexandre

*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/distant.h"
#include "../include/commun.h"
#include "../include/joueur.h"
#include "../include/carre.h"
#include "../include/gestion_tour_sdl.h"
#include "../include/gestion_partie_sdl.h"
#include "../include/gestion_partie.h"
#include "../include/gestion_bot.h"
#include "../include/affichage_sdl.h"
#include "../include/son.h"

extern SDL_Renderer* renderer;
int son;
int effet;

/**
	*\fn void saisir_pseudo(Joueur **j)
	*\details Initialise le pseudo du joueur
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
	*\return Retourne 3 si le joueur appuis sur la croix de l'aficheur<br>
		Retourne 0 si l'affectation a bien fonctionné
*/


int saisir_pseudo_joueur(Joueur** j){
/********PARTIE SAISI NOM DES JOUEURS*********/
	SDL_Event event_saisie;
	int continuer=1;
	SDL_StartTextInput();
	Bouton* b_retour = init_bouton_sdl(RETOUR);
	/*Boucle d'évenement*/
	while(continuer == 1){

		SDL_RenderClear(renderer);
		/*Attend l'appuis d'une touche*/
		while(SDL_PollEvent(&event_saisie)){
			/*Si c'est la croix, on arrete*/
			if(event_saisie.type == SDL_QUIT)
				continuer= 3;
			/*Si c'est la touche entrée, on passe au joueur suivant*/
			else if(strlen((*j)->pseudo) > 0 && event_saisie.type == SDL_KEYDOWN && (event_saisie.key.keysym.sym == SDLK_RETURN || event_saisie.key.keysym.sym == SDLK_KP_ENTER) ) {
				jouer_son(BOUTON);
				continuer = 0;
			}
			/*Si c'est une touche supprimer, on efface le dernier caractère saisie*/
			else if(event_saisie.key.keysym.sym == SDLK_BACKSPACE && event_saisie.type == SDL_KEYDOWN){
				if (strlen((*j)->pseudo) > 0)
					(*j)->pseudo[strlen((*j)->pseudo) - 1] = '\0';
			}
			/*Si c'est une touche du clavier, on l'entre dans le pseudo*/
			else if(event_saisie.type == SDL_TEXTINPUT && strlen((*j)->pseudo) < TAILLE_PSEUDO) {
				strcat((*j)->pseudo, event_saisie.text.text);
			}
			else if(event_saisie.type == SDL_MOUSEBUTTONDOWN && curs_hover_bouton(b_retour)) {
				jouer_son(BOUTON_RETOUR);
				continuer=4;
			}

		}
		afficher_bouton_sdl(b_retour);
		afficher_saisie_pseudo_sdl(*j);
		SDL_RenderPresent(renderer);
		}
	SDL_StopTextInput();
	if(!continuer){
		/* Si le pseudo n'est pas trop grand */
		if (strlen((*j)->pseudo) < TAILLE_PSEUDO) {
			/* Réalloue la bonne taille pour le pseudo */
			(*j)->pseudo = realloc((*j)->pseudo, sizeof(char) * (strlen((*j)->pseudo) + 1));
			(*j)->pseudo[strlen((*j)->pseudo)]='\0';
		}
		else /* S'il est trop grand: troncature */
			(*j)->pseudo[TAILLE_PSEUDO]='\0';
	}
	free_bouton_sdl(&b_retour);

	return continuer;
}

/**
	*\fn void initialisation_partie_sdl(Joueur **j)
	*\details Initialise une partie <br> Crée une liste de n Joueur [2-4].
	<br>Initialise le pseudo des joueurs
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
	*\return Retourne 3 si le joueur appuis sur la croix de l'aficheur<br>
		Retourne 0 si l'affectation a bien fonctionné
*/
int saisir_nb_joueur(){
/********PARTIE SAISI NB DES JOUEURS*********/
	int nb_joueur=-1;
	int continuer=1;
	/*Creation des boutons + evenement */
	SDL_Event event;
	Bouton* b_nb_deux=init_bouton_sdl(NB_JOUEURS_2);
	Bouton* b_nb_trois=init_bouton_sdl(NB_JOUEURS_3);
	Bouton* b_nb_quatre=init_bouton_sdl(NB_JOUEURS_4);
	Bouton* b_retour = init_bouton_sdl(RETOUR);

	/*Tant que l'evenenement n'est pas fini*/
	while(continuer == 1){
		SDL_RenderClear(renderer);
		//Attend un événement
		while(SDL_PollEvent(&event)){
			//Si il appuis sur la croix
			if(event.type == SDL_QUIT)
				continuer = -1;
			//Si il appuis sur un bouton
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				/*Bouton 2 joueur*/
				if (curs_hover_bouton(b_nb_deux)) {
					jouer_son(BOUTON);
					nb_joueur=2;
				}

				/*Bouton 3 joueur*/
				else if (curs_hover_bouton(b_nb_trois)) {
					jouer_son(BOUTON);
					nb_joueur=3;
				}

				/*Bouton 4 joueur*/
				else if (curs_hover_bouton(b_nb_quatre)) {
					jouer_son(BOUTON);
					nb_joueur=4;
				}

				else if(curs_hover_bouton(b_retour)) {
					jouer_son(BOUTON_RETOUR);
					continuer= 5;
				}

			}
		}
		//Si on appuis sur un bouton, alors on arrete la boucle
		if( nb_joueur > 0){
			continuer=0;
		}
		/*Partie Affichage*/
		afficher_nb_joueurs_sdl();
		afficher_bouton_sdl(b_nb_deux);
		afficher_bouton_sdl(b_nb_trois);
		afficher_bouton_sdl(b_nb_quatre);
		afficher_bouton_sdl(b_retour);
		SDL_RenderPresent(renderer);

	}
	free_bouton_sdl(&b_nb_deux);
	free_bouton_sdl(&b_nb_trois);
	free_bouton_sdl(&b_nb_quatre);
	free_bouton_sdl(&b_retour);
	if(nb_joueur == -1)
		return continuer;
	else
		return nb_joueur;
}

/**
	*\fn void initialisation_partie_sdl(Joueur **j)
	*\details Initialise une partie <br> Crée une liste de n Joueur [2-4].
	<br>Initialise le pseudo des joueurs
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
	*\return Retourne 3 si le joueur appuis sur la croix de l'aficheur<br>
		Retourne 0 si l'affectation a bien fonctionné
*/

int saisir_type_joueur(Joueur** j){
/********PARTIE SAISI TYPE DES JOUEURS*********/
	int continuer=1;
	Type_Joueur type_tmp;
	/*Creation des boutons + evenement */
	SDL_Event event;
	Bouton* b_bot=init_bouton_sdl(TYPE_JOUEUR_BOT);
	Bouton* b_j_local=init_bouton_sdl(TYPE_JOUEUR_LOCAL);
	Bouton* b_j_distant=init_bouton_sdl(TYPE_JOUEUR_DISTANT);
	Bouton* b_retour = init_bouton_sdl(RETOUR);



	/*Tant que l'evenenement n'est pas fini*/
	while(continuer == 1){
		SDL_RenderClear(renderer);
		//Attend un événement
		while(SDL_PollEvent(&event)){
			//Si il appuis sur la croix
			if(event.type == SDL_QUIT)
				continuer = 3;
			//Si il appuis sur un bouton
		else if(event.type == SDL_MOUSEBUTTONDOWN){
				/*Bouton bot*/
				if (curs_hover_bouton(b_bot)){
					jouer_son(BOUTON);
					type_tmp=BOT;
					//Si on appuis sur un bouton, alors on arrete la boucle*/
					continuer=0;
				}
				/*Bouton local*/
				else if (curs_hover_bouton(b_j_local)){
					jouer_son(BOUTON);
					type_tmp=LOCAL;
					//Si on appuis sur un bouton, alors on arrete la boucle*/
					continuer=0;
				}

				/*Bouton distant*/
				else if (curs_hover_bouton(b_j_distant)){
					jouer_son(BOUTON);
					type_tmp=DISTANT;
					//Si on appuis sur un bouton, alors on arrete la boucle*/
					continuer=0;
				}
				else if(curs_hover_bouton(b_retour)) {
					jouer_son(BOUTON_RETOUR);
					continuer = 4;
				}

			}
		}

		/*Partie Affichage*/
		afficher_type_joueur_sdl(*j);
		afficher_bouton_sdl(b_bot);
		afficher_bouton_sdl(b_j_local);
		afficher_bouton_sdl(b_j_distant);
		afficher_bouton_sdl(b_retour);
		SDL_RenderPresent(renderer);

	}
	free_bouton_sdl(&b_bot);
	free_bouton_sdl(&b_j_local);
	free_bouton_sdl(&b_j_distant);
	free_bouton_sdl(&b_retour);
	if(continuer == 0)
		(*j)->type=type_tmp;
	return continuer;
}

/**
	*\fn void initialisation_joueur_distant(Joueur **j)
	*\details Initialise une partie <br> Crée une liste de n Joueur [2-4].
	<br>Initialise le pseudo des joueurs
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
	*\return Retourne 3 si le joueur appuis sur la croix de l'aficheur<br>
		Retourne 0 si l'affectation a bien fonctionné
*/


int initialiser_joueur_distant(Joueur **j){

	SDL_Event event;
	int sockfd = -1;
	int sockfd_connexion = creer_socket_connexion(PORT_DEFAUT);
	int continuer = 0;
	Bouton* b_retour = init_bouton_sdl(RETOUR);

	while (sockfd == -1 && continuer == 0) {

		SDL_RenderClear(renderer);

		//Attend un événement
		while(SDL_PollEvent(&event)){
			//Si il appuis sur la croix
			if(event.type == SDL_QUIT) {
				fermer_connexion(sockfd_connexion);
				continuer = 3;
			}
			//Si il appuis sur un bouton
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				if(curs_hover_bouton(b_retour)) {
					jouer_son(BOUTON_RETOUR);
					fermer_connexion(sockfd_connexion);
					continuer = 4;
				}
			}
		}

		afficher_attente_connexion_sdl();
		afficher_bouton_sdl(b_retour);
		SDL_RenderPresent(renderer);

		sockfd = accepter_connexion(sockfd_connexion);
	}

	unsigned char buffer[TAILLE_BUFF];
	int r;

	if(sockfd > 0){
		do {
			SDL_RenderClear(renderer);
			//Attend un événement
			while(SDL_PollEvent(&event)){
				//Si il appuis sur la croix
				if(event.type == SDL_QUIT)
					continuer = 3;
				//Si il appuis sur un bouton
				else if(event.type == SDL_MOUSEBUTTONDOWN){
					if(curs_hover_bouton(b_retour))
						continuer = 4;
				}
			}
			afficher_bouton_sdl(b_retour);
			afficher_attente_pseudo_sdl();
			SDL_RenderPresent(renderer);
			r = recevoir_buffer(sockfd, buffer);
		} while(r == 0 && continuer == 0);
		free_bouton_sdl(&b_retour);
		if(!continuer){
			if (r < 0) {
				continuer = 2;
			}
			else {
				(*j)->sockfd=sockfd;
				recevoir_pseudo(buffer,(*j)->pseudo);
           			(*j)->type = DISTANT;
			}
		}
	}
	else {
		fprintf(stderr,"Problème de connexion");
	}
	return continuer;
}

/**
	*\fn void initialisation_partie_sdl(Joueur **j)
	*\details Initialise une partie <br> Crée une liste de n Joueur [2-4].
	<br>Initialise le pseudo des joueurs
	* Si la liste existe, on la supprime puis on en crée une autre.
	*\param j Pointeur sur un Joueur pour créer la liste de Joueur.
	*\return Retourne 3 si le joueur appuis sur la croix de l'aficheur<br>
		Retourne 0 si l'affectation a bien fonctionné
*/
int initialisation_partie_sdl(Joueur** j ){ /*Initialisation de la partie, appel des fonctions pour crées les joueurs, le plateau*/

	int nb;
	nb=saisir_nb_joueur();
	int retour=4;
	if(nb == -1)
		return 3;
	else if(nb == 5)
		return 2;
	if(*j)
		joueur_liste_detruire(j);
	*j=joueur_liste_creation(nb);
	Joueur* j_pivot = *j;
	/*Tant que tous les joueurs n'ont pas de pseudo*/
	do{
		while(retour == 4){
			retour=saisir_type_joueur(j);
			if(retour)
				return retour;


			switch((*j)->type){
				case BOT: sprintf((*j)->pseudo,"Bot %s",couleur_tostring((*j)->couleur));break;

				case LOCAL:
					retour=saisir_pseudo_joueur(j);
					if(retour == 3)
						return 3;
					break;

				case DISTANT:
					retour=initialiser_joueur_distant(j);
					if(retour == 3)
						return 3;
					else if (retour == 2) {
						erreur_reseau();
						return 3;
					}
					break;
					default:return 3;
			}
		}
		retour=4;
		*j=joueur_suivant(*j);
	} while (*j != j_pivot);

	do{
		if((*j)->sockfd)
			envoyer_liste_joueurs((*j)->sockfd, j_pivot);
		*j=joueur_suivant(*j);
	} while(*j != j_pivot);
	return 0;
}




/**
	*\fn int fin_de_partie_sdl(Joueur** j)
	*\brief Vérifie si c'est vraiment la fin de la partie,modifie les scores  et réalise les choix à faire.
	*\details Si le Joueur à une liste_vide, on le fait abandonner.
	*Une fois que tous les Joueurs ont abandoné,mets à jour le score ,affiche les résultats et demande a l'utilisateur un choix:<br>
	*-Recommencez une manche.
	<br>
	*-Recommencez une partie.
	<br>
	*-Quittez le programme.
	*\param j Liste de tous les Joueurs pour vérifier s'ils ont tous abandonné.
	\return Retourne le choix de l'utilisateur (ou 0 s'il reste un Joueur en jeu):
		*1 - Recommence une manche. <br>
		*2 - Retourne au titre. <br>

*/


/* Affiche les résultats,mets à jour le score ,propose les options de fin de partie et renvoie le résultat correspondant */

int fin_de_partie_sdl(Joueur** j){
	/*Si le joueur n'a plus de piece dans sa liste, fait abandonner le joueur automatiquement*/
	if(joueur_liste_piece(*j) == NULL)
		joueur_abandonne(*j);

	/* S'il reste un joueur n'ayant pas abandonné */
	if(!(joueur_abandon(*j)))
		return 0;

	int continuer= -1;

	/*Creation des boutons + evenement */
        SDL_Event event_fin;
        Bouton* b_continuer=init_bouton_sdl(CONTINUER);
        Bouton* b_quitter=init_bouton_sdl(QUITTER_PARTIE);




	/*Mise a jour du score vue que c'est la fin de la partie*/
	maj_scores(j);
	afficher_scores_sdl(*j);

	/*On demande a l'utilisateur les choix de fin de partie */
	while(continuer == -1){

		SDL_RenderClear(renderer);
		/*On attend la touche du joueur*/
        	while(SDL_PollEvent(&event_fin)){
			//Si il appuis sur la croix
			if(event_fin.type == SDL_QUIT)
				continuer = 3;
			/*En attendant qu'il appuis sur le bouton*/
                	else if(event_fin.type == SDL_MOUSEBUTTONDOWN){
                        	if (curs_hover_bouton(b_continuer)) {
                        		jouer_son(BOUTON);
                                	continuer = 1;
                        	}

                        	else if (curs_hover_bouton(b_quitter)) {
                        		jouer_son(BOUTON_RETOUR);
                               		continuer = 2;
                        	}

			        }
		}
		afficher_resultats_sdl(*j);
		afficher_bouton_sdl(b_continuer);
		afficher_bouton_sdl(b_quitter);
	        SDL_RenderPresent(renderer);
	}
	free_bouton_sdl(&b_continuer);
	free_bouton_sdl(&b_quitter);
	return continuer;
}

//Verifie a la fin de la partie si tous les joueurs veullent continuez la partie ou quittez
/*int verification_continue(Joueur* j, int choix){
	Joueur* pivot=j;
	pivot=joueur_suivant(pivot);
	int valeur_r;
        SDL_Event event_fin;

	while(j != pivot){
		SDL_RenderClear(renderer);
		//On attend la touche du joueur
        	while(SDL_PollEvent(&event_fin)){
			//Si il appuis sur la croix
			if(event_fin.type == SDL_QUIT)
				return 3;
		}
		//reçois le resultat de l'autre
		//afficher_attente_continue_sdl();
		SDL_RenderPresent(renderer);
		valeur_r = 1;//?
		if(choix != valeur_r || choix != 1){
			return valeur_r;
		}
		pivot=joueur_suivant(pivot);

	}

	return choix;
}
*/

/**
	*\fn int jouer_tour_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur** j)
	*\details Réalise le fonctionnement d'un tour en appellant les fonctions de gestion_tour .
	*\param pl Plateau de jeu pour posez les Piece.
	*\param j Joueur qui joue actuellement.
	*\return 3 si la croix a été saisie <br>
	  renvoie le resultat de la fonction gestion_tour_sdl<br>
	*1 = Abandon du Joueur
	*2 = Quitte le jeu ( Appuis sur la croix)

*/

/*Appel toute les fonctions pour réalisé un tour*/
int jouer_tour_bot_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j){
	int valeur_r = 4;
	if(joueur_a_abandonne(*j)){
//		printf("\n Ce joueur à abandonne\n");
		*j=joueur_suivant(*j);

	}
	else{

		//valeur_r=gestion_tour_bot(pl, *j);/*fonction bot*/
		valeur_r=gestion_tour_bot(pl, *j);
		if(valeur_r == 1){//Le joueur a abandoné
//			printf("Vous avez abandonné\n");
			joueur_abandonne(*j);
		}
		else if(valeur_r == 2){
			return 3;//Quitte le jeu
		}
		*j=joueur_suivant(*j);
	}
	return valeur_r;
}

/*Appel toute les fonctions pour réalisé un tour*/
int jouer_tour_joueur_distant_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j){
	int valeur_r = 4;
	Piece* p = NULL;
	int id_piece;
	unsigned char buffer[TAILLE_BUFF];
    Reserves* r = init_afficher_pieces_dispo_sdl((*j));
	SDL_Event event;

	if(joueur_a_abandonne(*j)){
//		printf("\n Ce joueur à abandonne\n");
		*j=joueur_suivant(*j);
	}
	else{
		valeur_r = 0;
		while(valeur_r == 0){
			SDL_RenderClear(renderer);
	    		while(SDL_PollEvent(&event)) {
      				if(event.type == SDL_QUIT){
           				valeur_r= 2;
        			}
			}
			afficher_plateau_sdl(pl);
                        afficher_pieces_dispo_sdl(r, (*j), p);
                        afficher_scores_sdl((*j));
                        afficher_tour_sdl((*j));
                        SDL_RenderPresent(renderer);

                        if (valeur_r == 0) {
                            valeur_r = recevoir_buffer((*j)->sockfd, buffer);
                            if (valeur_r == -1) {
                            	(*j)->type = BOT;
                            	return -1;
                            }
                        }
		}

		valeur_r = recup_type(buffer);

		if (valeur_r == PLATEAU) { // Le joueur a jouer
			id_piece = recevoir_plateau(buffer, pl);
			if ( id_piece > 0) {
                                Piece** p = &((*j)->liste_piece);
                                Piece* pivot = *p;

				while(id_piece > piece_id(*p)){
					*p = piece_suivant(*p);
				}
				if( id_piece == piece_id(*p)){
                                        if (pivot == *p)
                                                pivot = NULL;

                                        if(joueur_nb_piece_restantes(*j) == 1 && piece_id(*p) == 1)
                                            (*j)->score += 5;

                                        liste_piece_suppr_elem(p);

                                        while (pivot != NULL && (*p) != pivot)
                                            *p = piece_suivant(*p);

                                        (*j)->liste_piece = *p;
				}
			}

			valeur_r = id_piece * -1; /* Le joueur a réussi à poser sa Piece */
		}
		else if(valeur_r == ABANDON_JOUEUR){//Le joueur a abandoné
			joueur_abandonne(*j);
			valeur_r = 1; /* Le joueur a abandonné */
		}
		else {
			return 3;//Quitte le jeu
		}
		*j=joueur_suivant(*j);

	}
	return valeur_r;
}



/*Appel toute les fonctions pour réalisé un tour*/
int jouer_tour_joueur_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU], Joueur** j){
	int valeur_r = 4;

	if(joueur_a_abandonne(*j)){
//		printf("\n Ce joueur à abandonne\n");
		*j=joueur_suivant(*j);

	}
	else{
		valeur_r=gestion_tour_sdl(pl,*j);

		if(valeur_r == 1){//Le joueur a abandonné
//			printf("Vous avez abandonné\n");
			joueur_abandonne(*j);
		}
		else if(valeur_r == 2){
			return 3;//Quitte le jeu
		}
		*j=joueur_suivant(*j);

	}
	return valeur_r;
}


/**
	*\fn int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j)
	*\brief Réalise le fonctionnement d'une manche.
	*\param pl Plateau de jeu .
	*\param j La liste de Joueur qui joue durant la manche.
	*\return Renvoie le choix des joueurs: <br>
		*2 - Retourne au menu. <br>
		*3 - Quitte le programme.
*/


int jouer_manche_sdl(Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU],Joueur* j){
	int choix;
        Joueur * init;
	do{
		initialisation_manche(pl,&j);

		do{

            init = j;

            if(j->type == BOT) {
				choix=jouer_tour_bot_sdl(pl,&j);
            }
			else if(j->type == DISTANT) {
				choix=jouer_tour_joueur_distant_sdl(pl,&j);
            }
			else {
				choix=jouer_tour_joueur_sdl(pl,&j);
            }

			if(choix == 3) {
				fermer_connexions_distantes(j);
				return choix;
			}
			//Si le joueur n'a pas déjà abandonné
			if(choix != 4){
					// Sons
					if(joueur_a_abandonne(init)) {
                        jouer_son(ABANDON);
                    }
                    else {
                        jouer_son(POSE_PIECE);
                    }

					// Envoi aux joueurs distants
                    while (j != init) {
                        		if (j->sockfd) {
                                		if(!joueur_a_abandonne(init)) {
                                    			envoyer_plateau(j->sockfd, pl,(choix * -1));
                                		}
                               			else {
                                  			envoyer_abandon_joueur(j->sockfd, init);
                                		}
                            	}
                           		j = joueur_suivant(j);
                    }
                    j = joueur_suivant(j);
			}
			choix=fin_de_partie_sdl(&j);
		} while(!(choix));





	} while(choix == 1 );

	return choix;
}


/**
	*\fn int jouer_partie_sdl()
	*\brief Réalise l'appelle de fonction pour jouer une partie.
	*\details Crée une liste de joueur et un plateau de jeu.
	*\puis appelle initalisation_partie et debut_manche.
*/

int type_partie(){

	int val_retour=-1;
	SDL_Event event;
	Bouton* b_creer = init_bouton_sdl(CREER_PARTIE);
    Bouton* b_rejoindre = init_bouton_sdl(REJOINDRE_PARTIE);
  	Bouton* b_retour = init_bouton_sdl(RETOUR);
	while(val_retour < 0){
		/* Ecouter les EVENT */
                SDL_RenderClear(renderer);
       	        while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				val_retour = 3;
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				if(curs_hover_bouton(b_creer)) {
					jouer_son(BOUTON);
					val_retour= 1;
				}
				else if(curs_hover_bouton(b_rejoindre)) {
					jouer_son(BOUTON);
					val_retour= 2;
				}

				else if(curs_hover_bouton(b_retour)) {
					jouer_son(BOUTON_RETOUR);
					val_retour= 4;
				}
			}
		}
	/* Affiche le menu type partie */
		afficher_titres_sdl();
	 	afficher_bouton_sdl(b_creer);
		afficher_bouton_sdl(b_rejoindre);
		afficher_bouton_sdl(b_retour);
		SDL_RenderPresent(renderer);
	}
 	free_bouton_sdl(&b_creer);
	free_bouton_sdl(&b_rejoindre);
	free_bouton_sdl(&b_retour);

	return val_retour;
}

int jouer_partie_sdl(){ /*Appel de toute les fonctions partie */

	jouer_son(MUSIQUE_FOND);

	Joueur * j = NULL;
	Couleur pl[TAILLE_PLATEAU][TAILLE_PLATEAU] = {0};
	int retour = 2;
	int val_partie=1;
	SDL_Event event;
	Bouton* b_jouer = init_bouton_sdl(JOUER);
	Bouton* b_quitter_jeu = init_bouton_sdl(QUITTER_JEU);
	Bouton* b_son = init_bouton_sdl(SON);
	son = 0;
	Bouton* b_effet = init_bouton_sdl(EFFET);
	effet = 1;

	while (retour == 2){

		/* Menu */

		/* Ecouter les EVENT */
                SDL_RenderClear(renderer);
       	        while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				retour = 3;
			else if(event.type == SDL_MOUSEBUTTONDOWN){
				if(curs_hover_bouton(b_jouer)) {
					jouer_son(BOUTON);
					retour= 1;
				}
				else if(curs_hover_bouton(b_quitter_jeu)) {
					jouer_son(BOUTON_RETOUR);
					retour= 3;
				}
				else if(curs_hover_bouton(b_son)) {
					son = (son+1)%2;
					jouer_son(MUSIQUE_FOND);
				}
				else if(curs_hover_bouton(b_effet)) {
					effet = (effet+1)%2;
					jouer_son(BOUTON);
				}
			}
		}
		/* Appuie du bouton JOUER */
		if (retour == 1) { /*Jouer*/
			while((retour == 1) ||(retour == 2 && val_partie != 4)){
				val_partie = type_partie();

				/*Partie local*/
				if(val_partie == 1){
					retour=4;
					while((val_partie == 1)&&(retour ==4)){//Cas 4 si on appuis sur retour après nb nombre
						retour = initialisation_partie_sdl(&j);
					}

				}
				/*Partie rejoindre */
				else if(val_partie == 2)
					retour = initialisation_partie_distant_sdl(&j);

				/*Retour au menu*/

				else if(val_partie == 4)
					retour = 2;
				//Si il appuis  sur la croix
				else
					retour = val_partie;

			}
			if(retour == 4)
				retour =2;

			else if (retour != 3){ /* Si les Joueurs arrêtent le programme pendant la saisie des pseudos / nb_joueur */
				if(val_partie == 1)
					retour = jouer_manche_sdl(pl,j);
				else if(val_partie == 2)
					retour = jouer_manche_distant_sdl(pl, j, retour);
			/*?*/	joueur_liste_detruire(&j);

				//if (retour == 3) /* Si les Joueurs (à la fin de la partie) ne veulent plus refaire de parties */
				//	return retour;
			}
		}
		/* else if Appuie sur le bouton REGLE // TODO*/
		//else if (retour == 3) { /*Appuie sur le bouton Quitter || Appuie sur la croix*/
		//	return retour;
		//}

		/* Affiche le menu */
		afficher_titres_sdl();
	 	afficher_bouton_sdl(b_jouer);
	    afficher_bouton_sdl(b_quitter_jeu);
	    afficher_bouton_sdl(b_son);
	    afficher_bouton_sdl(b_effet);
		SDL_RenderPresent(renderer);
	}

	if (j) { /* Si les Joueurs arrêtent le programme pendant la saisie des pseudos / nb_joueur */
		joueur_liste_detruire(&j);
	}

	free_bouton_sdl(&b_jouer);
        free_bouton_sdl(&b_quitter_jeu);

	return retour;
}
