#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

///Menu d'aide
void menuAide();
///Charge la a afficher sur le menu
BITMAP* chargerMapMenu(int *i);
///Boucle du menu du jeu
int menu(int *nbMap);

#endif // MENU_H_INCLUDED
