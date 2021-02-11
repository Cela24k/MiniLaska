//
// Created by alece on 11/02/2021.
//

#ifndef UNTITLED1_GFX_H
#define UNTITLED1_GFX_H

#define RASTERX 71
#define RASTERY 29

char raster[RASTERY][RASTERX];

void printa_raster()
{
    printf("  - -0------ --1------ --2------ ---3----  -- 4----  ---5----  ---6- ---\n");
    printf(" /||/|/|///  |/|/|///  /| /|///   /|Y|///     /|///     /|///    /  /|/||\n");
    for (int i = 0; i < RASTERY; ++i) {
        for (int j = 0; j < RASTERX; ++j) {
            printf("%c",raster[i][j]);
        }
        if(i==RASTERY-2)
            printf(" /\n");
        else if(i == RASTERY-1)
            printf(" \n");
        else if(i == RASTERY /2)
            printf("X|\n");
        else if((i-2)%4 == 0)
            printf("%d|\n",i/4);
        else
            printf(" |\n");
    }
}

char lettera_pedina(Pedina_list p)
{
    if(p)
    {
        if(p->colore == BLUE)
        {
            if(p->stato == GENERALE)
                return 'B';
            else return 'b';
        }
        else
        {
            if(p->stato == GENERALE)
                return 'R';
            else return 'r';
        }
    }
    return ' ';
}
void raster_con_sprite(Board b)
{
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if(b->vet[i][j])
            {
                raster[(i*4)+1][(j*10)+5] = lettera_pedina(b->vet[i][j]);
                if(b->vet[i][j]->next)
                {
                    raster[(i*4)+2][(j*10)+5] = lettera_pedina(b->vet[i][j]->next);
                    if(b->vet[i][j]->next->next)
                    {
                        raster[(i*4)+3][(j*10)+5] = lettera_pedina(b->vet[i][j]->next->next);
                    }
                }
            }
        }
    }
}

void init_raster()
{
    for (int i = 0; i < RASTERY; ++i) {
        for (int j = 0; j < RASTERX; ++j) {
            if(j %10 == 0 && i%4 == 0)
                raster[i][j] = '+';
            else if(i%4 == 0)
                raster[i][j] = '-';
            else if(j%10 == 0)
                raster[i][j] = '|';
            else raster[i][j] = ' ';
        }
    }
}

#endif //UNTITLED1_GFX_H
