#ifndef EDITEURMAP_H_INCLUDED
#define EDITEURMAP_H_INCLUDED

#define NTUILE      12    // Nombre de tuiles graphiques utilisées
#define TXTUILE     34    // Largeur des tuiles
#define TYTUILE     34    // Hauteur des tuiles
#define NCOLTUILE    6    // Nombre de colonnes de tuiles dans le fichier image

#define NXMAP      25      // Nombre de tuiles en largeur sur le terrain
#define NYMAP      20     // Nombre de tuiles en hauteur sur le terrain

/// Dessine la tuile numéro ituile sur la bitmap bmp au coordonnées xmap ymap
void dessineTuile(BITMAP *bmp, BITMAP *tableTuiles, int ituile, int xmap, int ymap);

/// Dessine un terrain complet sur une bitmap
void dessineTerrain(BITMAP *bmp, BITMAP *tableTuiles, int terrain[NYMAP][NXMAP]);

/// Chargement "sécurisé" d'une image
BITMAP * load_bitmap_check(char *nomImage);

///Reinitialise le terrain a vide
void initTerrain(int terrain[NYMAP][NXMAP]);

///Remplace les cases dechets par des cases sol normales sur la map(rien ne change sur la map de colision)
void gestionDechets(BITMAP *bmp, BITMAP *tableTuiles, int terrain[NYMAP][NXMAP]);

///Recupere le nombre de map présente actuellement dans le dossier Images/map
int chargerNbMap(int nbMapMax);

///Permet a l'utilisateur de dessiner et d'enregistrer sa propre map
void editeurDeMap();

#endif // EDITEURMAP_H_INCLUDED
