#include <stdio.h>
#include <stdlib.h>
#include "Pedina.h"
#include "Board.h"

#define CATTURA 10
#define PERDITA (-12)

int partita1v1(Board b, enum color player1, enum color player2)
{
    if(winner(b,player1,player2)!=player2)
    {
        int x1,y1,x2,y2;

        printf("\n\n");
        print_board(b);

        if(player1==BLUE)
            printf("\nGiocatore BLU: \n");
        else printf("\nGiocatore ROSSO: \n");

        printf("Inserisci le coordinate della pedina da muovere!\n x: ");
        scanf("%d",&x1);
        printf("y: ");
        scanf("%d",&y1);
        printf("\nOra inserisci le coordinate in cui muoverti.\n x2: ");
        scanf("%d",&x2);
        printf("y2: ");
        scanf("%d",&y2);

        if(entro_limiti(x1,y1) && entro_limiti(x2,y2) && b->vet[x1][y1] && b->vet[x1][y1]->colore == player1 && muovi_legale_wrapper(b->vet[x1][y1],x2,y2,b))
        {
            printf("\nMosso da x,y (%d,%d)",x1,y1);
            printf("\nA x2,y2 (%d,%d)",x2,y2);
            return partita1v1(b,player2,player1);
        }
        else
        {
            printf("\nMossa illegale, rifai!");
            return partita1v1(b,player1,player2);
        }
    }
    return winner(b,player1,player2);
}
/*
int checkmoves(Board b,int *vettore, Pedina_list p)
{

    for (int i = 0; i < 8; i+=2) {
        if(mangia_legale(b->vet[p->coordx][p->coordy],vettore[i],vettore[i+1],b))

    }
}

int ai(Board b, enum color pcplayer)
{
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore == pcplayer)
            {
                int vet[8] = {0,0,0,0,0,0,0,0};
                if(has_moves(b->vet[i][j],b,vet))
                {
                    int a[4];

                    if(checkmoves(b,a,b->vet[i][j]))
                }


            }
                if(checkmoves(pcplayer))
        }
    }
}

int partita_cpu(Board b, enum color player1, enum color player2)
{
    if(winner(b,player1,player2)!=player2)
    {
        int x1,y1,x2,y2;

        printf("\n\n");
        print_board(b);


        if(entro_limiti(x1,y1) && entro_limiti(x2,y2) && b->vet[x1][y1] && b->vet[x1][y1]->colore == player1 && muovi_legale_wrapper(b->vet[x1][y1],x2,y2,b))
        {
            printf("\nMosso da x,y (%d,%d)",x1,y1);
            printf("\nA x2,y2 (%d,%d)",x2,y2);
            return partita1v1(b,player2,player1);
        }
        else
        {
            printf("\nMossa illegale, rifai!");
            return partita1v1(b,player1,player2);
        }
    }
    return winner(b,player1,player2);
}
 */
void congratulations(Board b)
{
    if(winner(b,BLUE,RED) == BLUE)
        printf("\n\nCongratulazioni giocatore BLUE! Hai vinto\n");
    else if(winner(b,BLUE,RED) == RED)
        printf("\nCongratulazioni giocatore RED! Hai vinto");
}

int main_menu(int mode)
{
    Board b = init_board();

    if(mode == 1)
    {
        return partita1v1(b,BLUE,RED);
    }
    delete_board(b);
    return 0;
}

int main() {
    /*
    Board b = init_board();
    //partita1v1(b,RED,BLUE);
    muovi_legale_wrapper(b->vet[2][2],3,3,b);
    print_board(b);
    mangia(b->vet[4][4],2,2,b);
    print_board(b);
    congratulations(b);
    */
    main_menu(1);
}
