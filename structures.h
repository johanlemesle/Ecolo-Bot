#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

typedef struct bot{
    //Position
    int x;
    int y;
    //Précédent X et Y
    int precX;
    int precY;
    //Statut actuel du bot : 0 = mort, 1 = en vie, 2 = ramassage, 3 = chute
    int statut;
    //Orientation du personnage
    int orientation;
    //dechets ramasser
    int dechets;
    //Bitmap de toute les sprites du perso
    BITMAP *perso[4][9];
    //Bitmap de ramassage et tombe
    BITMAP *interaction[2][3];
    //Bitmap de danse
    BITMAP *danse[28];
    //Sequence d'instruction
    char seq[33];
    //Var de l'animation
    int anim;
    //Var de l'ombre (lors du saut)
    int ombre;
}t_bot;

typedef struct nuage{
    //Bitmaps des nuages
    BITMAP *bmpnuage[5];
    //Tableaux de coordonnées des nuages
    int coordX[5];

}t_nuage;

#endif // STRUCTURES_H_INCLUDED
