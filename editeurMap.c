#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

#define NTUILE      12    // Nombre de tuiles graphiques utilisées
#define TXTUILE     34    // Largeur des tuiles
#define TYTUILE     34    // Hauteur des tuiles
#define NCOLTUILE    6    // Nombre de colonnes de tuiles dans le fichier image

#define NXMAP      25      // Nombre de tuiles en largeur sur le terrain
#define NYMAP      20     // Nombre de tuiles en hauteur sur le terrain


///code venant de http://www.ece.fr/~fercoq/allegro/Alleg_C4/4_1_decor_tableau_tilemap.html modifier


// Dessine la tuile numéro ituile sur la bitmap bmp au coordonnées xmap ymap
// (unité en nombre de tuiles, pas en pixels)
void dessineTuile(BITMAP *bmp, BITMAP *tableTuiles, int ituile, int xmap, int ymap)
{
    int lig, col; // ligne et colonne de la tuile source dans tableTuiles

    // Le numéro de ligne et de colonne dans un tableau 2D
    // à partir de l'indice du ième élément (en comptant de gauche à droite
    // et de haut en bas) s'obtiennent avec le quotient et le reste
    // dans la division de l'indice par le nombre de colonnes du tableau



    lig = ituile / NCOLTUILE;
    col = ituile % NCOLTUILE;

    // on copie juste le morceau concerné
    blit(tableTuiles, bmp, col*TXTUILE, lig*TYTUILE,
         xmap*TXTUILE, ymap*TYTUILE, TXTUILE, TYTUILE);
}

// Dessine un terrain complet sur une bitmap
void dessineTerrain(BITMAP *bmp, BITMAP *tableTuiles, int terrain[NYMAP][NXMAP])
{
    int xmap,ymap;

    for (ymap=0;ymap<NYMAP;ymap++)
            for (xmap=0;xmap<NXMAP;xmap++)
                dessineTuile(bmp,tableTuiles ,terrain[ymap][xmap],xmap,ymap);
}

// Chargement "sécurisé" d'une image :
// interrompt le programme avec un message si problème...
BITMAP * load_bitmap_check(char *nomImage)
{
    BITMAP *bmp;
    bmp=load_bitmap(nomImage,NULL);
    if (!bmp)
    {
        allegro_message("pas pu trouver %s",nomImage);
        exit(EXIT_FAILURE);
    }
    return bmp;
}

//Genere un terrain vide
void initTerrain(int terrain[NYMAP][NXMAP])
{
    int i,j;

    for (i=0; i<NYMAP; i++)
    {
        for (j=0; j<NXMAP; j++)
            terrain[i][j] = 11;
    }
}

//remplace le cases dechet par des sol normaux sur la map(rien ne change sur la map de colision)
void gestionDechets(BITMAP *bmp, BITMAP *tableTuiles, int terrain[NYMAP][NXMAP])
{
    int xmap,ymap;

    for (ymap=0;ymap<NYMAP;ymap++)
        for (xmap=0;xmap<NXMAP;xmap++)
            if (terrain[ymap][xmap] == 5)
            {
                terrain[ymap][xmap] = 1;
                dessineTuile(bmp,tableTuiles ,terrain[ymap][xmap],xmap,ymap);
            }
}

//recupere le nombre de map présente dans le dossier map du jeu
int chargerNbMap(int nbMapMax)
{
    char nomF[51];
    BITMAP *bmp;

    //On charge la map numero n
    sprintf(nomF,"Images/map/map%d.bmp", nbMapMax);
    bmp = load_bitmap(nomF, NULL);

    //Si on arrive pas a la charger
    if (!bmp)
    {
        destroy_bitmap(bmp);
        return nbMapMax;
    }

    //On appel le meme programme avec n+1
    nbMapMax = chargerNbMap(nbMapMax+1);

    //On retourne n
    return nbMapMax;
}

/******************************************/
/* PROGRAMME PRINCIPAL                    */
/* initialisation puis boucle de jeu      */
/******************************************/

void editeurDeMap()
{
    BITMAP *tableTuiles;
    BITMAP *tableCol;
    BITMAP *buffer;
    BITMAP *page;
    BITMAP *pageCol;
    BITMAP *HUD;

    // Un seul terrain de jeu
    int terrainExemple[NYMAP][NXMAP];

    int selection = 1;
    int xMap = 0, yMap = 0;
    int nbMap=0;
    int xSel, ySel;

    char nomFichier[51];

    // Charger le fichier avec les tuiles
    tableTuiles=load_bitmap_check("Images/editeurMap/tileMap.bmp");
    tableCol = load_bitmap_check("Images/editeurMap/tileMapCol.bmp");
    HUD = load_bitmap_check("Images/editeurMap/tileMap.bmp");
    page = create_bitmap(NXMAP*TXTUILE, NYMAP*TYTUILE);
    pageCol = create_bitmap(NXMAP*TXTUILE, NYMAP*TYTUILE);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);

    //Initioalisation du terrain
    initTerrain(terrainExemple);
    dessineTerrain(page, tableTuiles, terrainExemple);

    //On compte le nombre de map presente dans le dossier
    nbMap = chargerNbMap(0);
    printf("Il y a %d maps dans le dossier\n", nbMap);

    // Boucle de jeu (rien pour l'instant)
    while (!key[KEY_ESC])
    {
        clear_bitmap(buffer);

        //"Vide" le terrain actuel
        if (key[KEY_DEL])
            initTerrain(terrainExemple);

        //Selection de la case a dessiner
        if (mouse_b&1 && mouse_x > (NXMAP*TXTUILE) && mouse_x < ((NXMAP*TXTUILE)+HUD->w) && mouse_y < HUD->h)
        {
            xSel = mouse_x/TXTUILE-NXMAP;
            ySel = mouse_y/TYTUILE;

            selection = xSel+ySel*NCOLTUILE;
        }

        //Dessine la case selectionée a l'endroit choisi
        if (mouse_b&1 && mouse_x < (NXMAP*TXTUILE) && mouse_y < NYMAP*TYTUILE)
        {
            //On calcule les coordonée de la souris correspondante dans le tableau
            xMap = mouse_x/TXTUILE;
            yMap = mouse_y/TYTUILE;
            //On rempli la case du tableau selectionné avec la contenu choisi
            terrainExemple[yMap][xMap] = selection;
        }

        //On dessine la map en fonction du tableau
        dessineTerrain(page, tableTuiles, terrainExemple);
        //On dessine la map de colision en fonction du tableau
        dessineTerrain(pageCol, tableCol, terrainExemple);
        //Affichage de la map sur le buffer
        blit(page, buffer, 0, 0, 0, 0, page->w, page->h);
        //Affichage de l'HUD a droite de la map
        blit(HUD, buffer, 0, 0, page->w, 0, HUD->w, HUD->h);


        if (key[KEY_S])
        {
            //On remplace les cases dechets de la map par des cases sol
            gestionDechets(page, tableTuiles, terrainExemple);
            //Sauvegarde de la map nbMap
            sprintf(nomFichier, "Images/map/map%d.bmp", nbMap);
            save_bitmap(nomFichier, page, NULL);
            //Sauvegarde de la map de colision nbMap
            sprintf(nomFichier, "Images/map/mapCol%d.bmp", nbMap);
            save_bitmap(nomFichier, pageCol, NULL);
            //On vide les deux bitmaps, on incremente nbMap et on reinitialise le tableau
            clear_bitmap(page);
            clear_bitmap(pageCol);
            //On incremente le nombre de map
            nbMap++;
            //On indique a l'utilisateur le nombre de map actuellement dans le dossier map
            printf("Il y a %d maps dans le dossier\n", nbMap);
            //On reinitialise le terrain a vide
            initTerrain(terrainExemple);
            //On met un rest de 0.5sec afin de pas enregistrer pour laisser le temps de relacher la touche et ne pas enregistrer de bitmap vide
            rest(500);
        }

        //Affichage sur l'ecran
        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
        rest(60);
    }
}
