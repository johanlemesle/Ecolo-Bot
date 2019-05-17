#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>



BITMAP* chargerMapMenu(int *i)
{
    char nomF[51];
    BITMAP *bmp;

    sprintf(nomF,"Images/map/map%d.bmp", *i);
    bmp = load_bitmap(nomF, NULL);

    if (!bmp)
    {
        *i = 0;
        bmp = chargerMapMenu(i);
    }

    return bmp;
}

void menuAide()
{
    BITMAP *aide;

    aide = load_bitmap("Images/HUD/aide.bmp", NULL);
    if (!aide)
    {
        allegro_message("l'image n'a pas pu etre chargée");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    while(!key[KEY_ESC])
    {
        //BOUTON QUITTER
        if (mouse_b&1 && mouse_x <= 265 && mouse_y <= 108)
            break;

        blit(aide, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
}

int menu(int *nbMap)
{
    BITMAP *page;//bitmap du buffer page
    BITMAP *map;
    BITMAP *menu;

    int choix=0;

    //bitmap de buffer page
    page = create_bitmap(SCREEN_W, SCREEN_H);
    //bitmap de fond du menu
    menu = load_bitmap("Images/HUD/menu.bmp", NULL);
    //bitmap de la map selectionnée
    map = load_bitmap("Images/map/map0.bmp", NULL);

    if (!menu || !map)
    {
        allegro_message("l'image n'a pas pu etre chargée");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    while (!key[KEY_ESC])
     {
        clear_bitmap(page); // on clear page

        //on charge la bitmap de map en fonction de nbMap
        map = chargerMapMenu(nbMap);

        //BOUTON + MAP
        if (mouse_b&1 && mouse_x >= 817 && mouse_y >= 243 && mouse_x <= 951 && mouse_y <= 371)
            (*nbMap)++;

        //BOUTON - MAP
        if (mouse_b&1 && mouse_x >= 817 && mouse_y >= 381 && mouse_x <= 951 && mouse_y <= 509)
            (*nbMap)--;
        //BOUTON JOUER
        if (mouse_b&1 && mouse_x >= 435 && mouse_y >= 558 && mouse_x <= 717 && mouse_y <= 675)
        {
            printf("jouer\n");
            choix = 1;
            break;
        }

        //BOUTON AIDE
        if (mouse_b&1 && mouse_x >= 1018 && mouse_y <= 107)
        {
            printf("aide\n");
            choix = 2;
            break;
        }

        //BOUTON EDITEUR DE MAP
        if (mouse_b&1 && mouse_x >= 956 && mouse_y >= 626)
        {
            printf("editeur de map\n");
            choix = 3;
            break;
        }


        //BOUTON QUITTER
        if (mouse_b&1 && mouse_x <= 265 && mouse_y <= 108)
        {
            printf("quitter\n");
            choix = 4;
            break;
        }


        if (*nbMap < 0)
            *nbMap=0;

        rest(50);

        blit(menu, page, 0, 0, 0, 0, menu->w, menu->h);
        stretch_blit(map, page, 0, 0, map->w, map->h, 360, 246, 430, 256);
        textprintf_ex(page, font, 355, 232, makecol(255,255,255), -1, "Map numero : %d", *nbMap);
        blit(page, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        rest(50);
     }

    return choix;
}

