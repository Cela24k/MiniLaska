#ifndef UNTITLED1_GFX_H
#define UNTITLED1_GFX_H

#define RASTERX 71
#define RASTERY 29

/**
 * "raster" è il BUFFER che serve ad immagazzinare la versione "grafica" della scacchiera.
 *
 * - Passo 1 per la stampa della scacchiera
 *
 *      Inizialmente il raster viene creato e popolato con la funzione ++ init_raster ++ , che inserisce nell'array
 *      di caratteri un pattern 7x7 di quadrati 10x4 char, generati attraverso l'algoritmo in questo modo:
 *
 *                      +---------+--------+ * 7 volte
 *                      |         |        |
 *                      |         |        |
 *                      |         |        |
 *     (Init_Raster)    +---------+--------+
 *                      |         |        |
 *                      |         |        |
 *                      |         |
 *                      +---------+- - - -
 *                      * 7 volte
 *
 *      In questo modo viene generata la struttura di base della grafica della scacchiera.
 *
 *  - Passo 2
 *
 *      Ora che c'è lo spazio per stampare fino a 3 pedine una sopra l'altra è necessario
 *      popolare la matrice con le corrispettive pedine nella posizione giusta, questo viene
 *      fatto con la funzione ++ raster_con_sprite ++ in questo modo:
 *                                 0
 *                             +---------+
 *                             |    B    |                  - questo nel caso in cui in posizione 0,0 della board data in input
 *      (Raster_con_sprite) 0  |    r    |                    ci sia un Generale Blu in testa, seguito da una pedina Soldato Rossa e
 *                             |    b    |                    infine da una Soldato Blu
 *                             +---------+
 *
 *  - Passo 3
 *
 *      La board è stata immagazzinata nel raster rappresentando caselle e pedine,
 *      ora manca solo la stampa a video attraverso la funzione ++ printa_raster ++,
 *      che semplicemente va a stampare a schermo la scacchiera finale con alcune
 *      decorazioni come il numero delle caselle e l'asse (X o Y).
 *
 *
 */
char raster[RASTERY][RASTERX];

/**
 * Inizializza nel buffer di caratteri "raster" la struttura delle celle della scacchiera,
 * ottenendo un pattern 7x7 di rettangoli del tipo:
 *                                      +----------+
 *                                      |          |
 *                                      |          |
 *                                      |          |
 *                                      +----------+
 */
void init_raster()
{
    int i,j;
    for (i = 0; i < RASTERY; ++i) {
        for (j = 0; j < RASTERX; ++j) {
            if(j %10 == 0 && i%4 == 0)
                raster[i][j] = '+';
            else if(i%4 == 0)
                raster[i][j] = '-';
            else if(j%10 == 0)
                raster[i][j] = '|';
            else {
                if((i/4 + j/10)%2 != 0)
                    raster[i][j] = '*';
                else raster[i][j] = ' ';
            }
        }
    }
}

/**
 * Funzione di utility che data una pedina, restituisce il carattere da inserire nel buffer.
 *
 *      - Restituisce 'r' se la pedina è ROSSA, 'b' se è BLU,
 *        in maiuscolo se è un GENERALE, minuscolo se è un SOLDATO.
 *
 *        @param p la pedina da leggere
 *        @return char contenente la lettera da stampare
 */
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

/**
 * Questa funzione data una Board come parametro, inserisce nel buffer grafico le lettere
 * corrispondenti allo stato delle pedine in quella stessa Board.
 *
 * Ad esempio:
 *
 *      - ho una Board b in cui in pos. (0,0) ho uno stack di pedine del tipo B->r->b
 *
 *      La funzione inserirà nel rettangolo 0,0 del raster le pedine come segue:
 *                                        0
 *                                  +----------+
 *                                  |     B    |
 *                               0  |     r    |
 *                                  |     b    |
 *                                  +----------+
 *  @param b la board dalla quale inserire le lettere nel buffer di caratteri
 */
void raster_con_sprite(Board b)
{
    int i,j;
    for (i = 0; i < 7; ++i) {
        for (j = 0; j < 7; ++j) {
            /* se una pedina in posizione (i,j) DELLA STRUTTURA DATI esiste -> inserisco la sua lettera in pos.
             ((i*4)+1,(j*10)+5) DEL BUFFER GRAFICO.
              E' calcolato in modo tale da essere al centro della rispettiva casella nel buffer.
             */
            if(b->vet[i][j])
            {
                raster[(i*4)+1][(j*10)+5] = lettera_pedina(b->vet[i][j]);
                if(b->vet[i][j]->next) /*se esiste una sua pedina successiva, la inserisco di 1 più in basso*/
                {
                    raster[(i*4)+2][(j*10)+5] = lettera_pedina(b->vet[i][j]->next);
                    if(b->vet[i][j]->next->next) /*se esiste una terza, la inserisco analogamente nello spazio più in basso*/
                    {
                        raster[(i*4)+3][(j*10)+5] = lettera_pedina(b->vet[i][j]->next->next);
                    }
                }
            }
        }
    }
}

/**
 * Funzione di utility che stampa il raster che deve essere già stato inizializzato,
 * inoltre lo decora con gli assi e dando un minimo effetto di profondità.
 */
void printa_raster()
{
    int i,j;
    printf("  - -0------ --1------ --2------ ---3----  -- 4----  ---5----  ---6- ---\n");
    printf(" /||/|/|///  |/|/|///  /| /|///   /|Y|///     /|///     /|///    /  /|/||\n");
    for (i = 0; i < RASTERY; ++i) {
        for (j = 0; j < RASTERX; ++j) {
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

#endif //UNTITLED1_GFX_H
