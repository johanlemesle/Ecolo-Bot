#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <string.h>
#include <time.h>

#include "structures.h"
#include "menu.h"
#include "editeurMap.h"

#define nbActions 5
#define tBtn 100

///Cree, initialise et renvoi l'adresse d'un element t_bot
t_bot *initPerso(int nbBot);
///Affichage de tout les elements necessaire a l'ecran
void affichage(BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map,BITMAP* mapCOl, BITMAP *dechet, t_bot *bot[5],t_nuage *nuage, int lancerClicker);
///Detecte l'appui sur un des boutons d'action par l'utilisateur et ajoute le caractere correspondant a la sequence d'action
void btnActions(t_bot *bot[5], BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet);
///Reinitialise une sequence(chaine de caractere) a 0
void resetSequence(char seq[33]);
///Sous programme qui fait avancer le bot avec une animation en fonction de son orientation actuelle
void avancer(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage,int aActuel);
///Sous programme qui fait sauter le bot au dessus de l'eau en fonction de son orientation actuelle(Pour pouvoir sauter il faut une case d'eau suivi d'une case de terre)
void sauter(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage,int aActuel);
///Sous programme permettant au bot de ramasser un dechet si il est sur une case avec dechet, ou de deposer tout ses dechets si il est sur un case avec poubelle
void ramasser(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage,int aActuel);
///Sous programme permettant au bot de danser lorsqu'il gagne
void danser(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage);
///Appelle le sous programme correspondant a l'action actuelle
void action(t_bot *bot[5], int aActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage, int botactuel);
///Detecte l'appui sur le bouton lancer par l'utilisateur
int btnLancer(BITMAP *lancer, BITMAP *bufferHUD);
///Permet a l'utilisateur de changer l'orientation de chacun des bot avant de lancer la sequence
void rotationBotSpawn(t_bot *bot);
///Sous programme d'affichage des dechets
void afficherDechet(BITMAP *map, BITMAP *mapCol, BITMAP *dechet);
///Sous programme qui modifie la map de colision afin d'y supprimer les dechets
void supprDechet(int posx, int posy, BITMAP *mapCol);
///Reinitialise les "attributs" de chacun des bot ainsi que la map de colision au cas ou elle aurait été modifie durant la partie
BITMAP *reinitialiserPartie(t_bot *bot[5], int nbMap);
///Detection de la victoire en fonction des différents niveaux
int DetectionVictoire(BITMAP *fond,BITMAP *bufferHUD,BITMAP *icone[nbActions],BITMAP *lancer,BITMAP  *map,BITMAP *mapCol,BITMAP *dechet,t_bot *bot[5],t_nuage *nuage,int Nbmap);


t_bot *initPerso(int nbBot)
{
    int i;
    t_bot *bot;
    char nomFic[100];

    bot = (t_bot*)malloc(sizeof(t_bot));

    //au milieu de l'ecran
    bot->x = SCREEN_W/2;
    bot->y = SCREEN_H/2;
    //Coordonnées précédentes
    bot->precX=0;
    bot->precY=0;

    //en vie
    bot->statut = 0;
    //regarde vers le bas
    bot->orientation = 2;
    //nb de dechets ramassé par le bot
    bot->dechets = 0;

    bot->seq[0] = '\0';

    bot->anim=0;
    //Pas d'ombre
    bot->ombre=0;


    printf("%d    %s\n", strlen(bot->seq), bot->seq);
    //Chargement images
    for (i=0; i<9; i++)
    {
        //Chargement des images d'animations vers le haut
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dAr%d.bmp", nbBot, nbBot, i);
        bot->perso[0][i] = load_bitmap(nomFic,NULL);
        if (bot->perso[0][i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }

        //Chargement des images d'animations vers la droite
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dD%d.bmp", nbBot, nbBot, i);
        bot->perso[1][i] = load_bitmap(nomFic,NULL);
        if (bot->perso[1][i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }

        //Chargement des images d'animations vers le bas
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dAv%d.bmp", nbBot, nbBot, i);
        bot->perso[2][i] = load_bitmap(nomFic,NULL);
        if (bot->perso[2][i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }

        //Chargement des images d'animations vers la gauche
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dG%d.bmp", nbBot, nbBot, i);
        bot->perso[3][i] = load_bitmap(nomFic,NULL);
        if (bot->perso[3][i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }
    }

   for (i=0; i<3; i++)
    {
        //Chargement des images de ramassages
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dR%d.bmp", nbBot, nbBot, i);
        bot->interaction[0][i] = load_bitmap(nomFic,NULL);
        if (bot->interaction[0][i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }
        //Chargement des images de chute
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dTombe.bmp", nbBot, nbBot);
        bot->interaction[1][i] = load_bitmap(nomFic,NULL);
        if (bot->interaction[1][i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }
    }

    for(i=0; i<28;i++)
    {
        //Chargement de la danse vers le haut
        sprintf(nomFic,"Images/bot/bot%d.bmp/bot%dDanse%d.bmp", nbBot, nbBot, i);
        bot->danse[i] = load_bitmap(nomFic,NULL);
        if (bot->danse[i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }
    }



    printf("bot bien chargé\n");

    return bot;
}

t_nuage *initNuage()
{
    int i;
    t_nuage *nuage;
    nuage = (t_nuage*)malloc(sizeof(t_nuage));
    char nomFic[100];

    for (i=0; i<5; i++)
    {
        sprintf(nomFic,"Images/HUD/nuage%d.bmp",i);
        nuage->bmpnuage[i] = load_bitmap(nomFic,NULL);
        if (nuage->bmpnuage[i] == NULL)
        {
            allegro_message("pas pu trouver %s",nomFic);
            exit(EXIT_FAILURE);
        }
        nuage->coordX[i]=-(nuage->bmpnuage[0]->w);

    }

    printf("Nuages bien chargés\n");
    return nuage;
}

void affichage(BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map,BITMAP *mapCol, BITMAP *dechet, t_bot *bot[5],t_nuage *nuage, int lancerClicker)
{
    ///LancerClicker = -1 -> la séquence n'est pas joué, LancerClicker sert aussi à envoyer à ce sous-programme à quelle instruction on se trouve (quand > ou = 0)

    int i;
    int j;
    int val;

    rectfill(fond, 0, 0, fond->w, fond->h, makecol(165,199,210));
    rectfill(bufferHUD, 0, 0, bufferHUD->w, bufferHUD->h, makecol(255,0,255));


    //Bloc bleu d'affichage d'instructions s'agrandit en fonction du nombre d'instructions
    if(strlen(bot[0]->seq) == 32)
    {
        rectfill(bufferHUD,bufferHUD->w-313-TXTUILE ,9,bufferHUD->w-TXTUILE,477,makecol(125,160,220));
    }
    else
        rectfill(bufferHUD,bufferHUD->w-313-TXTUILE ,9,bufferHUD->w-TXTUILE,50+((strlen(bot[0]->seq)/4)*(50+9)+9),makecol(125,160,220));

    //Rectangle vert autour de l'affichage d'instructions lorsqu'on lance le protocole
    if(lancerClicker>-1)
        rect(bufferHUD,bufferHUD->w-313-TXTUILE,9, bufferHUD->w-TXTUILE, 50+((strlen(bot[0]->seq)/4)*(50+9)+9),makecol(255,0,0));

    //Affichage des nuages
    masked_blit(nuage->bmpnuage[0], bufferHUD,0, 0,nuage->coordX[0],bufferHUD->h-(nuage->bmpnuage[0]->h),nuage->bmpnuage[0]->w, nuage->bmpnuage[0]->h);
    nuage->coordX[0]=nuage->coordX[0]+5;
    if(nuage->coordX[0] > 1250)
        nuage->coordX[0]= -(nuage->bmpnuage[0]->w);

    masked_blit(nuage->bmpnuage[1], bufferHUD,0, 0,nuage->coordX[1],bufferHUD->h-(nuage->bmpnuage[0]->h),nuage->bmpnuage[1]->w, nuage->bmpnuage[1]->h);
    nuage->coordX[1]=nuage->coordX[1]+7;
    if(nuage->coordX[1] > 1250)
        nuage->coordX[1]= -(nuage->bmpnuage[1]->w);

    masked_blit(nuage->bmpnuage[2], bufferHUD,0, 0,nuage->coordX[2],bufferHUD->h-(nuage->bmpnuage[0]->h),nuage->bmpnuage[2]->w, nuage->bmpnuage[2]->h);
    nuage->coordX[2]=nuage->coordX[2]+10;
    if(nuage->coordX[2] > 1250)
        nuage->coordX[2]= -(nuage->bmpnuage[2]->w);

    masked_blit(nuage->bmpnuage[3], bufferHUD,0, 0,nuage->coordX[3],bufferHUD->h-(nuage->bmpnuage[0]->h),nuage->bmpnuage[3]->w, nuage->bmpnuage[3]->h);
    nuage->coordX[3]=nuage->coordX[3]+15;
    if(nuage->coordX[3] > 1250)
        nuage->coordX[3]= -(nuage->bmpnuage[3]->w);

    masked_blit(nuage->bmpnuage[4], bufferHUD,0, 0,nuage->coordX[4],bufferHUD->h-(nuage->bmpnuage[0]->h)-9,nuage->bmpnuage[4]->w, nuage->bmpnuage[4]->h);
    nuage->coordX[4]=nuage->coordX[4]+7;
    if(nuage->coordX[4] > 1250)
        nuage->coordX[4]= -(nuage->bmpnuage[4]->w);

    //Affichage de la séquence;
    int lenSeq = strlen(bot[0]->seq);
    int ligne=9;

    if(lenSeq > 0) //On affiche que si la séquence existe
    {
        int seqActu;
        int curs=-1;
        int xActu=0;
        int yActu=0;

        for(seqActu = 0; seqActu < lenSeq; seqActu++)
        {
            //Si la séquence arrive à un multiple de 4 on revient à la ligne
            if( seqActu == 4 || seqActu == 8 || seqActu == 12 || seqActu == 16 || seqActu == 20 || seqActu == 24 || seqActu == 28 || seqActu == 32 ) //max 32 instructions
            {
                curs= 0;
                ligne=ligne+59;
            }
            else
                curs++;
            int colonne=(bufferHUD->w-304)+59*curs;

           //On récupère les coordonnées de l'instruction en cours d'action
            if(lancerClicker == seqActu)
            {
                xActu= colonne;
                yActu= ligne;
            }

            if(bot[0]->seq[seqActu] == 'a' )
                stretch_blit(icone[0], bufferHUD, 0, 0, icone[0]->w, icone[0]->h, colonne, ligne, icone[0]->w/2, icone[0]->h/2);

            if(bot[0]->seq[seqActu] == 'g' )
                stretch_blit(icone[1], bufferHUD, 0, 0, icone[1]->w, icone[1]->h, colonne, ligne, icone[1]->w/2, icone[1]->h/2);

            if(bot[0]->seq[seqActu] == 'd' )
                stretch_blit(icone[2], bufferHUD, 0, 0, icone[2]->w, icone[2]->h, colonne, ligne, icone[2]->w/2, icone[2]->h/2);

            if(bot[0]->seq[seqActu] == 'r' )
                stretch_blit(icone[3], bufferHUD, 0, 0, icone[3]->w, icone[3]->h, colonne, ligne, icone[3]->w/2, icone[3]->h/2);

            if(bot[0]->seq[seqActu] == 's' )
                stretch_blit(icone[4], bufferHUD, 0, 0, icone[4]->w, icone[4]->h, colonne, ligne, icone[4]->w/2, icone[4]->h/2);

        }

        //On encadre l'instruction en cours de lecture si actionnement de la séquence
         if(lancerClicker > -1)
             {
                rect(bufferHUD,xActu,yActu,xActu+50,yActu+50,makecol(0,255,0));
                rect(bufferHUD,xActu-1,yActu-1,xActu+51,yActu+51,makecol(0,255,0));
             }

    }

    //Affichages des bots
    for(i=0; i<5; i++)
    {
        j = bot[i]->orientation;

        //Affichage du perso
        if (bot[i]->statut == 1)
        {
            //Affichage de l'ombre si saut de gauche ou de droite
            if(bot[i]->ombre == 1 && (bot[i]->precY-bot[i]->y) > 20 && (bot[i]->orientation == 1 || bot[i]->orientation == 3))
                circlefill(bufferHUD,bot[i]->x,bot[i]->precY,(bot[i]->precY-bot[i]->y)/5, makecol(30,30,30));

            //affiche le bot[i]
            masked_blit(bot[i]->perso[j][bot[i]->anim], bufferHUD, 0, 0, bot[i]->x-(bot[i]->perso[j][bot[i]->anim]->w/2), bot[i]->y-(bot[i]->perso[j][bot[i]->anim]->h/2), bot[i]->perso[j][bot[i]->anim]->w, bot[i]->perso[j][bot[i]->anim]->h);
            //Affiche le nombre de dechets recolté au dessus de la tete du bot[i]
            if (bot[i]->dechets > 0)
                textprintf_centre_ex(bufferHUD, font, bot[i]->x, bot[i]->y-(bot[i]->perso[j][bot[i]->anim]->h/2)-10, makecol(255,255,0), -1, "%d", bot[i]->dechets);
        }
        //Affichage de l'action de ramassage
        else if(bot[i]->statut == 2)
        {
            //affiche le bot[i]
            masked_blit(bot[i]->interaction[0][bot[i]->anim], bufferHUD, 0, 0, bot[i]->x-(bot[i]->interaction[0][bot[i]->anim]->w/2), bot[i]->y-(bot[i]->interaction[0][bot[i]->anim]->h/2), bot[i]->interaction[0][bot[i]->anim]->w, bot[i]->interaction[0][bot[i]->anim]->h);
            //Affiche le nombre de dechets recolté au dessus de la tete du bot[i]
            if (bot[i]->dechets > 0)
                textprintf_centre_ex(bufferHUD, font, bot[i]->x, bot[i]->y-(bot[i]->perso[j][bot[i]->anim]->h/2)-10, makecol(255,255,0), -1, "%d", bot[i]->dechets);

        }
        //Affichage de la chute
        else if(bot[i]->statut == 3)
        {
            if (bot[i]->orientation == 0)
                masked_stretch_blit(bot[i]->interaction[1][0], bufferHUD, 0, 0, bot[i]->interaction[1][0]->w, bot[i]->interaction[1][0]->h, (bot[i]->x-17+(bot[i]->interaction[1][0]->w/(bot[i]->anim)/2)), (bot[i]->y-17+(bot[i]->interaction[1][0]->h/(bot[i]->anim)/2)),  (bot[i]->interaction[1][0]->w/(bot[i]->anim)) , (bot[i]->interaction[1][0]->h/(bot[i]->anim)));
            if (bot[i]->orientation == 1)
                masked_stretch_blit(bot[i]->interaction[1][0], bufferHUD, 0, 0, bot[i]->interaction[1][0]->w, bot[i]->interaction[1][0]->h, (bot[i]->x+17+(bot[i]->interaction[1][0]->w/(bot[i]->anim)/2)), (bot[i]->y+(bot[i]->interaction[1][0]->h/(bot[i]->anim)/2)),  (bot[i]->interaction[1][0]->w/(bot[i]->anim)) , (bot[i]->interaction[1][0]->h/(bot[i]->anim)));
            if (bot[i]->orientation == 2)
                masked_stretch_blit(bot[i]->interaction[1][0], bufferHUD, 0, 0, bot[i]->interaction[1][0]->w, bot[i]->interaction[1][0]->h, (bot[i]->x-17+(bot[i]->interaction[1][0]->w/(bot[i]->anim)/2)), (bot[i]->y+17+(bot[i]->interaction[1][0]->h/(bot[i]->anim)/2)),  (bot[i]->interaction[1][0]->w/(bot[i]->anim)) , (bot[i]->interaction[1][0]->h/(bot[i]->anim)));
            if (bot[i]->orientation == 3)
                masked_stretch_blit(bot[i]->interaction[1][0], bufferHUD, 0, 0, bot[i]->interaction[1][0]->w, bot[i]->interaction[1][0]->h, (bot[i]->x-17+(bot[i]->interaction[1][0]->w/(bot[i]->anim)/2)), (bot[i]->y+(bot[i]->interaction[1][0]->h/(bot[i]->anim)/2)),  (bot[i]->interaction[1][0]->w/(bot[i]->anim)) , (bot[i]->interaction[1][0]->h/(bot[i]->anim)));
        }
        //Affichage de la danse
        else if(bot[i]->statut == 4)
        {
            masked_blit(bot[i]->danse[bot[i]->anim], bufferHUD, 0, 0, bot[i]->x-(bot[i]->danse[bot[i]->anim]->w/2), bot[i]->y-(bot[i]->danse[bot[i]->anim]->h/2), bot[i]->danse[bot[i]->anim]->w, bot[i]->danse[bot[i]->anim]->h);
        }

    }

    //Affichage du bouton lancer
    if(lancerClicker > -1)
        blit(lancer, bufferHUD,lancer->w/2, 0, bufferHUD->w-lancer->w/2-3, bufferHUD->h-lancer->h-3, lancer->w/2, lancer->h);
    else if(lancerClicker == -1)
        blit(lancer, bufferHUD,0, 0, bufferHUD->w-lancer->w/2-3, bufferHUD->h-lancer->h-3, lancer->w/2, lancer->h);

    //Affichage des boutons d'actions sur le buffer HUD
    for (i=0; i<nbActions; i++)
    {
        blit(icone[i], bufferHUD, 0, 0, (i*(icone[i]->w+3)+3), bufferHUD->h-icone[i]->h-3, icone[i]->w, icone[i]->h);
    }

    //Affichage de la map
    blit(map, fond, 0, 0, 0, 0, map->w, map->h);

    //Affichage des déchets
    for (i=0; i<mapCol->w; i++)
    {
        for(j=0; j<mapCol->h; j++)
        {
            if (getpixel(mapCol, i, j) == makecol(39,127,255))
            {
                if (getpixel(mapCol, i+32, j) == makecol(0,255,255) || getpixel(mapCol, i-32, j) == makecol(0,255,255) || getpixel(mapCol, i, j+32) == makecol(0,255,255)|| getpixel(mapCol, i, j-32) == makecol(0,255,255))
                    val = 1;
                else
                    val = 0;
                masked_blit(dechet, fond, val*(dechet->w/2), 0, i, j, dechet->w/2, dechet->h);
            }
        }
    }
    //Affichage du buffer HUD sur le fond
    masked_blit(bufferHUD, fond, 0, 0, 0, 0, bufferHUD->w, bufferHUD->h);
    //Affichage du fond sur l'ecran
    blit(fond, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    rest(40);
}

void btnActions(t_bot *bot[5], BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet)
{
    int i;
    int numero;
    int seqActu = strlen(bot[0]->seq);

    if (mouse_b&1 && mouse_x < (nbActions*(tBtn+3)+3) && mouse_y > (SCREEN_H-icone[0]->h-3) && seqActu < 32)
    {
        //on determine le numero de l'action en fonction de la coordonnées de la souris en x
        numero = mouse_x/(icone[0]->w+3);
        switch(numero)
        {
        case 0:
            //avancer
            for (i=0; i<5; i++)
                bot[i]->seq[seqActu] = 'a';
            break;
        case 1:
            //gauche
            for (i=0; i<5; i++)
                bot[i]->seq[seqActu] = 'g';
            break;
        case 2:
            //droite
            for (i=0; i<5; i++)
                bot[i]->seq[seqActu] = 'd';
            break;
        case 3:
            //ramasser
            for (i=0; i<5; i++)
                bot[i]->seq[seqActu] = 'r';
            break;
        case 4:
            //sauter
            for (i=0; i<5; i++)
                bot[i]->seq[seqActu] = 's';
            break;
        }
        //On met le caractere suivant a '\0' pour indiquer la fin de la chaine
        seqActu++;
        for (i=0; i<5; i++)
            bot[i]->seq[seqActu] = '\0';
    }
}

void resetSequence(char seq[33])
{
    int i;

    for (i=0; i<33; i++)
    {
        seq[i] = '\0';
    }
}

void avancer(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage,int aActuel)
{
    int x = bot[botActuel]->x;
    int y = bot[botActuel]->y;

    switch(bot[botActuel]->orientation)
    {
    case 0 :
        //Vers le haut
        while(bot[botActuel]->y > y - TXTUILE && bot[botActuel]->statut == 1)
        {
            //On decrement les y
            bot[botActuel]->y = bot[botActuel]->y - 2;
            //On passe a l'bot->animation suivante
            if (bot[botActuel]->anim == 8)
                bot[botActuel]->anim = 1;
            else
                bot[botActuel]->anim++;
            //On affiche l'bot->animation actuelle a l'ecran
            affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            //Detection de "vide" et d'eau
            if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,0,0) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,255,255))
                bot[botActuel]->statut = 3;
        }
        break;
    case 1 :
        //Vers la droite
        while(bot[botActuel]->x < x + TXTUILE && bot[botActuel]->statut == 1)
        {
            //On increment les x
            bot[botActuel]->x = bot[botActuel]->x + 2;
            //On passe a l'bot[botActuel]->animation suivante
            if (bot[botActuel]->anim == 8)
                bot[botActuel]->anim = 1;
            else
                bot[botActuel]->anim++;
            //On affiche l'bot[botActuel]->animation actuelle a l'ecran
            affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            //Detection de "vide" et d'eau
            if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,0,0) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,255,255))
                bot[botActuel]->statut = 3;
        }
        break;
    case 2 :
        //Vers le bas
        while(bot[botActuel]->y < y + TXTUILE && bot[botActuel]->statut == 1)
        {
            //On incremente les y
            bot[botActuel]->y = bot[botActuel]->y + 2;
            //On passe a l'bot[botActuel]->animation suivante
            if (bot[botActuel]->anim == 8)
                bot[botActuel]->anim = 1;
            else
                bot[botActuel]->anim++;
            //On affiche l'bot[botActuel]->animation actuelle a l'ecran
            affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            //Detection de "vide" et d'eau
            if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,0,0) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,255,255))
                bot[botActuel]->statut = 3;
        }
        break;
    case 3 :
        //Vers la gauche
        while(bot[botActuel]->x > x - TXTUILE && bot[botActuel]->statut == 1)
        {
            //On decrement les x
            bot[botActuel]->x = bot[botActuel]->x - 2;
            //On passe a l'bot[botActuel]->animation suivante
            if (bot[botActuel]->anim == 8)
                bot[botActuel]->anim = 1;
            else
                bot[botActuel]->anim++;
            //On affiche l'bot[botActuel]->animation actuelle a l'ecran
            affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            //Detection de "vide" et d'eau
            if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,0,0) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,255,255))
                bot[botActuel]->statut = 3;
        }
        break;
    }
}

void sauter(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage,int aActuel)
{
    int x = bot[botActuel]->x;
    int y = bot[botActuel]->y;

    bot[botActuel]->precY=y+20;
    bot[botActuel]->precX=x;

    int Decal=0;


    switch(bot[botActuel]->orientation)
    {
    case 0 :
        //Vers le haut
        //On verifie les conditions suivantes : la case en face de nous est de l'eau et celle d'apres est de la terre ou un dechet
        if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)-TXTUILE) == makecol(0,255,255) && (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)-(2*TXTUILE)) == makecol(128,128,128) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)-(2*TXTUILE)) == makecol(255,127,39)))
            //Tant que l'on est pas arrivé a destination et que l'on est pas tombé dans le vide
            while(bot[botActuel]->y > y - (2*TXTUILE) && bot[botActuel]->statut == 1)
            {
                bot[botActuel]->ombre=1;
                //On déincrement les y
                bot[botActuel]->y = bot[botActuel]->y - 2;
                Decal++;

                if(Decal <= 16)
                {
                    bot[botActuel]->y=bot[botActuel]->y - 2;
                    bot[botActuel]->anim=3;
                }

                else if(Decal>18)
                {
                     bot[botActuel]->y=bot[botActuel]->y + 2;
                     bot[botActuel]->anim=2;
                }
                else
                    bot[botActuel]->anim=4;

                //On affiche l'bot[botActuel]->animation actuelle a l'ecran
                affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            }
        break;
    case 1 :
        //Vers la droite
        //On verifie les conditions suivantes : la case en face de nous est de l'eau et celle d'apres est de la terre ou un dechet
        if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2)+TXTUILE, bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,255,255) && (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2)+(2*TXTUILE), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(128,128,128) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2)+(2*TXTUILE), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(255,127,39)))
           {
            //Tant que l'on est pas arrivé a destination et que l'on est pas tombé dans le vide
            while(bot[botActuel]->x < x + (2*TXTUILE) && bot[botActuel]->statut == 1)
            {
                bot[botActuel]->ombre=1;
                //On increment les x
                bot[botActuel]->x = bot[botActuel]->x + 2;
                Decal++;

                if(Decal <= 16)
                {
                    bot[botActuel]->y=bot[botActuel]->y - 2;
                    bot[botActuel]->anim=3;
                }

                else if(Decal>18)
                {
                     bot[botActuel]->y=bot[botActuel]->y + 2;
                     bot[botActuel]->anim=2;
                }
                else
                    bot[botActuel]->anim=4;

                //On affiche l'bot[botActuel]->animation actuelle a l'ecran
                affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            }
           }
        break;
    case 2 :
        //Vers le bas
        //On verifie les conditions suivantes : la case en face de nous est de l'eau et celle d'apres est de la terre ou un dechet
        if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)+TXTUILE) == makecol(0,255,255) && (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)+(2*TXTUILE)) == makecol(128,128,128) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)+(2*TXTUILE)) == makecol(255,127,39)))
            //Tant que l'on est pas arrivé a destination et que l'on est pas tombé dans le vide
            while(bot[botActuel]->y < y + (2*TXTUILE) && bot[botActuel]->statut == 1)
            {
                bot[botActuel]->ombre=1;
                //On increment les y
                bot[botActuel]->y = bot[botActuel]->y + 2;
                Decal++;

                if(Decal <= 16)
                {
                    bot[botActuel]->y=bot[botActuel]->y - 2;
                    bot[botActuel]->anim=3;
                }

                else if(Decal>18)
                {
                     bot[botActuel]->y=bot[botActuel]->y + 2;
                     bot[botActuel]->anim=2;
                }
                else
                    bot[botActuel]->anim=4;

                //On affiche l'bot[botActuel]->animation actuelle a l'ecran
                affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            }
        break;
    case 3 :
        //Vers la gauche
        //On verifie les conditions suivantes : la case en face de nous est de l'eau et celle d'apres est de la terre ou un dechet
        if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2)-TXTUILE, bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(0,255,255) && (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2)-(2*TXTUILE), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(128,128,128) || getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2)-(2*TXTUILE), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(255,127,39)))
        {
            //Tant que l'on est pas arrivé a destination et que l'on est pas tombé dans le vide
            while(bot[botActuel]->x > x - (2*TXTUILE) && bot[botActuel]->statut == 1)
            {
               bot[botActuel]->ombre=1;
                //On déincrement les x
                bot[botActuel]->x = bot[botActuel]->x - 2;
                Decal++;

                if(Decal <= 16)
                {
                    bot[botActuel]->y=bot[botActuel]->y - 2;
                    bot[botActuel]->anim=3;
                }

                else if(Decal>18)
                {
                     bot[botActuel]->y=bot[botActuel]->y + 2;
                     bot[botActuel]->anim=2;
                }
                else
                    bot[botActuel]->anim=4;

                //On affiche l'bot[botActuel]->animation actuelle a l'ecran
                affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
            }
        }
        break;
    }
    bot[botActuel]->ombre=0;
}

void ramasser(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage,int aActuel)
{
    int i;

    //Si on est sur un dechet, on le ramasse
    if (getpixel(mapCol, bot[botActuel]->x+(bot[botActuel]->perso[0][0]->w/2), bot[botActuel]->y+(bot[botActuel]->perso[0][0]->h/2)) == makecol(255,127,39))
    {
        //On change le statut du personnage en tant que ramassage
        bot[botActuel]->statut=2;
        //Animation de ramassage
        for(i=0;i<3;i++)
        {
            bot[botActuel]->anim=i;
            affichage(fond,bufferHUD,icone,lancer,map,mapCol,dechet,bot,nuage,aActuel);
            rest(20);
        }
        //On incremente les dechets du personnage et on supprime le dechet de la map
        bot[botActuel]->dechets = bot[botActuel]->dechets+1;
        supprDechet(bot[botActuel]->x, bot[botActuel]->y, mapCol);
        //On repasse le statut du personnage a l'etat normal
        bot[botActuel]->statut=1;
    }
}

void danser(t_bot *bot[5], int botActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage)
{
    int i, j;

    for (j=0; j<28; j++)
    {
        for(i=0;i<5;i++)
        {
            if(bot[i]->statut == 1)
            {
                bot[i]->statut = 4;
            }
            bot[i]->anim=j;

        }
                affichage(fond,bufferHUD,icone,lancer,map,mapCol,dechet,bot,nuage,1);
                rest(60);
    }
    for(i=0;i<5;i++)
    {
        if (bot[i]->statut == 4)
            bot[i]->statut = 1;
    }

}

void action(t_bot *bot[5], int aActuel, BITMAP *mapCol, BITMAP *fond, BITMAP* bufferHUD, BITMAP *icone[nbActions], BITMAP *lancer, BITMAP *map, BITMAP *dechet,t_nuage *nuage, int botactuel)
{

    int i;

    //On effectue l'action correspondante a la lettre d'indice aActuel de la sequence
    switch(bot[botactuel]->seq[aActuel])
    {
    case 'a':
        //Avancer
        avancer(bot, botactuel, mapCol, fond, bufferHUD, icone, lancer, map, dechet, nuage,aActuel);
        break;
    case 'g':
        //On decale la valeur de l'orientation vers la gauche (-1)
        if (bot[botactuel]->orientation == 0)
            bot[botactuel]->orientation = 3;
        else
            bot[botactuel]->orientation = bot[botactuel]->orientation-1;
        affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage,aActuel);

        break;
    case 'd':
        //On decale la valeur de l'orientation vers la droite (+1)
        if (bot[botactuel]->orientation == 3)
            bot[botactuel]->orientation = 0;
        else
            bot[botactuel]->orientation = bot[botactuel]->orientation+1;
        affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, aActuel);
        break;
    case 's':
        //Sauter
        sauter(bot, botactuel, mapCol, fond, bufferHUD, icone, lancer, map, dechet, nuage,aActuel);
        break;
    case 'r':
        //Ramasser
        ramasser(bot, botactuel, mapCol, fond, bufferHUD, icone, lancer, map, dechet, nuage,aActuel);
        break;
    }

    //Si il tombe on effectu l'animation de chute
    if(bot[botactuel]->statut == 3)
    {
        for(i=1;i<5;i++)
        {
            bot[botactuel]->anim=i;
            affichage(fond,bufferHUD,icone,lancer,map,mapCol,dechet,bot,nuage,1);
            rest(50);
        }
        //On change le statut du bot a 0(mort) une fois l'animation terminée
        bot[botactuel]->statut = 0;
    }

    bot[botactuel]->anim = 0;
}

int btnLancer(BITMAP *lancer, BITMAP *bufferHUD)
{
    if (mouse_b&1 && mouse_x > bufferHUD->w-lancer->w/2-3 && mouse_y > bufferHUD->h-lancer->h-3)
        return 1;
    else
        return 0;
}

void spawnBot(t_bot *bot[5], BITMAP *coli)
{
    int i, j;

    for (i=0; i<SCREEN_W; i++)
    {
        for(j=0; j<SCREEN_H; j++)
        {
            //Bot numero 0
            if (getpixel(coli, i, j) == makecol(255,255,255))
            {
                bot[0]->x = i;
                bot[0]->y = j-TXTUILE;
                bot[0]->statut = 1;
            }
            //Bot numero 1
            if (getpixel(coli, i, j) == makecol(255,0,0))
            {
                bot[1]->x = i;
                bot[1]->y = j-TXTUILE;
                bot[1]->statut = 1;
            }
            //Bot numero 2
            if (getpixel(coli, i, j) == makecol(0,2,255))
            {
                bot[2]->x = i;
                bot[2]->y = j-TXTUILE;
                bot[2]->statut = 1;
            }
            //Bot numero 3
            if (getpixel(coli, i, j) == makecol(0,255,0))
            {
                bot[3]->x = i;
                bot[3]->y = j-TXTUILE;
                bot[3]->statut = 1;
            }
            //Bot numero 4
            if (getpixel(coli, i, j) == makecol(255,255,0))
            {
                bot[4]->x = i;
                bot[4]->y = j-TXTUILE;
                bot[4]->statut = 1;
            }
        }
    }
}

void rotationBotSpawn(t_bot *botte)
{
    if (mouse_b&1)
    {
        //Si on a cliquer a des coordonnées comprise dans le rectangle de hitbox du personnage alors on change l'orientation de celui ci
        if (mouse_x >= botte->x-(botte->perso[0][0]->w/2) && mouse_x <= botte->x+(botte->perso[0][0]->w/2) && mouse_y >= botte->y-(botte->perso[0][0]->h/2) && mouse_y <= botte->y+(botte->perso[0][0]->h/2))
        {
            if (botte->orientation == 3)
                botte->orientation = 0;
            else
                botte->orientation = botte->orientation + 1;
        }
    }
}

void supprDechet(int posx, int posy, BITMAP *mapCol)
{
    int i, j;
    int x=0, y=0;

    for (i=posx-17; i< posx+17; i++)
    {
        for (j=posy-17; j<posy+17; j++)
        {
            if (getpixel(mapCol, i, j) == makecol(39,127,255))
            {
                x = i;
                y = j;
            }
        }
    }

    for(i=x; i<x+TXTUILE; i++)
    {
        for (j=y; j<y+TXTUILE; j++)
        {
            putpixel(mapCol, i, j, makecol(128,128,128));
        }
    }
}

BITMAP *reinitialiserPartie(t_bot *bot[5], int nbMap)
{
    int i;
    BITMAP *mapCol;
    char nomImg[100];

    sprintf(nomImg,"Images/map/mapCol%d.bmp", nbMap);

    //On recharge la map de colision au cas ou elle a été modifiée (dechet ramassé)
    mapCol = load_bitmap_check(nomImg);

    //On reinitialise les "attributs" de chacun des bots
    for (i=0; i<5; i++)
    {
        bot[i]->dechets = 0;
        bot[i]->orientation = 2;
        bot[i]->statut = 0;
        bot[i]->anim = 0;

        resetSequence(bot[i]->seq);
    }
    //On re spawn les bot a leur emplacements
    spawnBot(bot, mapCol);

    return mapCol;
}

int DetectionVictoire(BITMAP *fond,BITMAP *bufferHUD,BITMAP *icone[nbActions],BITMAP *lancer,BITMAP  *map,BITMAP *mapCol,BITMAP *dechet,t_bot *bot[5],t_nuage *nuage,int Nbmap)
{
    //Verif est le nombre de bot qui doivent être à la poubelle avec au moins 1 déchet
    int verif=0;
    //Validation compte si le nombre de bots qui persévère (verif) est le même que le nombre de bots qui doivent persévérer
    int validation=0;
    int i;

    for (i=0; i<5; i++)
    {
        //On compte le nombre de bot en vie
        if (bot[i]->statut == 1)
            verif++;
    }

    for(i=0;i<5;i++)
    {
        if((getpixel(mapCol, bot[i]->x+(bot[i]->perso[0][0]->w/2), bot[i]->y+(bot[i]->perso[0][0]->h/2)) == makecol(0,128,128)) && bot[i]->dechets > 0 && bot[i]->statut==1)
        {
            //printf("detection victoire");
            //On incremente le nb de bot etant a une poubelle avec au moins un dechet
            validation++;
        }
    }

    if(validation == verif)
    {
        //on fait danser les bot vivants
        danser(bot,i,mapCol,fond,bufferHUD,icone,lancer,map,dechet,nuage);
        //affichage
        affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, 1);
        //petite pause pour apprecier la victoire
        rest(500);
        return 1;
    }
    else
        return 0;
}

int main()
{
    BITMAP *fond;//bitmap fond d'ecran
    BITMAP *bufferHUD;//buffer de l'interface
    BITMAP *icone[nbActions];//boutons d'action
    BITMAP *lancer;//bouton lancer
    BITMAP *map;//map
    BITMAP *mapCol;//map de colision
    BITMAP *dechet;//bitmap de dechet

    ///Variables
    t_bot *bot[5];
    t_nuage *nuage;
    int i, j, play=0;
    int nbMap;
    int choixMenu = 0;
    char nomFic[100];

    //Aleatoire init
    srand(time(NULL));

    // Lancer allegro et le mode graphique
    allegro_init();
    install_keyboard();
    install_mouse();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,1200,800,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    //Creation de la bitmap de buffer de l'interface
    bufferHUD = create_bitmap(SCREEN_W, SCREEN_H);
    //Creation de la bitmap de buffer de la map
    fond = create_bitmap(SCREEN_W, SCREEN_H);
    //Chargement du bouton lancer
    lancer = load_bitmap_check("Images/HUD/Lancer.bmp");
    //Chargement de la bitmap de dechet
    dechet = load_bitmap_check("Images/Items/dechet.bmp");

    //chargement des boutons instructions
    for (i=0; i<nbActions; i++)
    {
        sprintf(nomFic,"Images/HUD/bouton%d.bmp", i);
        icone[i] = load_bitmap_check(nomFic);
    }

    nbMap = 0;

    //On initialise les 5 bots
    for(i=0; i<5; i++)
        bot[i] = initPerso(i);

    //On initialise les nuages
    nuage = initNuage();

    // Montrer la souris à l'écran
    show_mouse(screen);

    while (choixMenu == 0)
    {
        choixMenu = menu(&nbMap);

        //On charge la map selectionnée avant d'entrer dans la boucle de jeu
        if (choixMenu == 1)
        {
            //Chargement des bitmaps de map et de map de colision
            sprintf(nomFic,"Images/map/map%d.bmp", nbMap);
            map = load_bitmap_check(nomFic);
            sprintf(nomFic,"Images/map/mapCol%d.bmp", nbMap);
            mapCol = load_bitmap_check(nomFic);

            //On place les bots a leur emplacement respectif sur la map
            spawnBot(bot, mapCol);
        }

        // Boucle de jeu
        while (choixMenu == 1 && !key[KEY_ESC])
        {
            //for (i=0; i<5; i++)
               // printf("bot %d : statut: %d\ns : %s\n", i, bot[i]->statut, bot[i]->seq);
            //printf("x: %d\ny : %d\n", bot->x, bot->statut);

            affichage(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage, -1);

            //Reset la sequence d'actions en appuyant sur Suppr et respawn le bot
            if (key[KEY_DEL])
            {
                mapCol = reinitialiserPartie(bot, nbMap);
            }

            //Si on a pas appuyer sur play
            if (play == 0)
            {
                //Permet de changer l'orientation initiale d'un bot avant de lancer la sequence
                for (i=0; i<5; i++)
                    rotationBotSpawn(bot[i]);

                //Detecte l'appui d'un bouton d'action et l'ajoute a la sequence
                btnActions(bot,mapCol,fond,bufferHUD,icone,lancer,map,dechet);
            }

            //Si on a appuyer sur play
            if (play == 1)
            {
                //Boucle de parcours de la sequence d'action
                for (i=0; i<strlen(bot[0]->seq); i++)
                {
                    //On parcours les bots
                    for (j=0; j<5; j++)
                    {
                        //Si le bot j est actif, on effectu l'action i de la sequence
                        if (bot[j]->statut == 1)
                        {
                            action(bot, i, mapCol, fond, bufferHUD, icone, lancer, map, dechet,nuage, j);
                        }
                        //Si on appui sur A on arrete tout
                        if (key[KEY_DEL])
                        {
                            for (i=0; i<5; i++)
                                bot[i]->statut = 0;
                        }
                    }
                 //   printf("%d/%d\n", i, strlen(bot[0]->seq));
                }
                play = 0;

                //Detection victoire
                if(DetectionVictoire(fond, bufferHUD, icone, lancer, map, mapCol, dechet, bot,nuage,nbMap) == 1)
                {
                    choixMenu = 0;
                    nbMap++;
                }

                //Reinitialise la partie
                mapCol = reinitialiserPartie(bot, nbMap);
            }

            //Detecte l'appui sur bouton lancer
            if (btnLancer(lancer, bufferHUD) == 1)
            {
                play = !play;
                j=0;
            }

            rest(30);
         }

        ///Si on quitte la boucle de jeu avec echap
        if (choixMenu == 1)
        {
            //On remet le choix du menu a 0
            choixMenu = 0;
            //et on reinitialise le statut de chaque bots
            for (i=0; i<5; i++)
                bot[i]->statut = 0;
        }

        ///Menu Aide
        if (choixMenu == 2)
        {
            menuAide();
            rest(400);
            choixMenu = 0;
        }
        ///Editeur de map
        if (choixMenu == 3)
        {
            editeurDeMap();
            choixMenu = 0;
        }
    }

    //On libere l'allocation des bots
    for (i=0; i<5; i++)
        free(bot[i]);

    free(nuage);

    return 0;
}END_OF_MAIN();
