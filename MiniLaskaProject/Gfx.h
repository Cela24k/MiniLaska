//
// Created by alece on 11/02/2021.
//

#ifndef UNTITLED1_GFX_H
#define UNTITLED1_GFX_H

#define RASTERX 71
#define RASTERY 29

/*
 *          +---------+---------+
 *          |    R    |
 *          |    b    |
 *          |    r    |
 *          *---------+
 */

int color_casella(Pedina_list p,int k)
{
    int out;

    for (int i = 0; i < k && p; ++i) {
        out = p->colore;
        p = p->next;
    }
    return out;
}

char** crea_raster(Board b)
{
    char **out;
    out = malloc(sizeof(char *)*RASTERX);

    for (int i = 0; i < RASTERX; ++i) {
        out[i] = malloc(sizeof(char)*RASTERY);
    }
    return out;
}

void printa_raster(char **vet)
{
    for (int i = 0; i < RASTERY; ++i) {
        for (int j = 0; j < RASTERX; ++j) {
            printf("%c",vet[i][j]);
        }
        printf("\n");
    }
}

void riempi_raster(Board b,char **vet)
{
    for (int i = 0; i < RASTERY ; i+=4) {
            vet[i] =    "+---------+---------+---------+---------+---------+---------+---------+";
            vet[i+1] =  "|         |         |         |         |         |         |         |";
            vet[i+2] =  "|         |         |         |         |         |         |         |";
            vet[i+3] =  "|         |         |         |         |         |         |         |";
    }

}


#endif //UNTITLED1_GFX_H
