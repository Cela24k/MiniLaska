#include <stdio.h>
int scacchiera[7][7]; //ci dice le posizioni delle pedine
int tabella[7][7]; //ci dice quante pedine una sopra l'altra

/*inizializza una scacchiera di pedine int: 1 se giocatore1, 2 se giocatore 2, 0 se vuota */

void inizializza_scacchiera(int mat[7][7])
{
    int i,j;

    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++)
        {
            if((j%2==0 && i%2==0) || (j%2!=0 && i%2!=0) && i!=3)
            {
                if(i<3) mat[i][j] = 1;
                else mat[i][j] = 2;
            }

            else mat[i][j] = 0;
        }
    }
}

void inizializza_tabella(int mat[7][7])
{
    int i,j;

    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++)
        {
            if((j%2==0 && i%2==0) || (j%2!=0 && i%2!=0) && i!=3) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
}

void printa(int mat[7][7])
{
    int i,j;
    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++)
        {
            printf("%2d",mat[i][j]);
        }
        printf("\n");
    }
}

/* ritorna 0 se è riuscito a muovere 1 altrimenti*/
//da implementare la "mangiata"

int muovi(int player,int x1, int y1, int x2, int y2)
{
    int flag = 0;
    if(x2 < 7 && y2 < 7 && (x2+y2) %2 == 0 && scacchiera[x1][y1] == player && scacchiera[x2][y2] == 0)
    {
        scacchiera[x1][y1] = 0;
        tabella[x1][y1] = 0;
        scacchiera[x2][y2] = player;
        tabella[x2][y2] = 1;
        flag = 1;
    }
    else return flag;
}


int main() {
    int boh = 0;

    inizializza_scacchiera(scacchiera);
    inizializza_tabella(tabella);
    printa(scacchiera);
    printf("\n");
    printa(tabella);
    printf("\n");

    //boh=muovi(1,2,0,3,1);
    muovi(1,2,0,3,1);

    printf("\n");
    printa(scacchiera);
    printf("\n");
    printa(tabella);




    return 0;
}
