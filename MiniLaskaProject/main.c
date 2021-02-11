#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Pedina.h"
#include "Board.h"
#include "AI.h"
#include "Gfx.h"

void print_game(Board b)
{
    init_raster();
    raster_con_sprite(b);
    printa_raster();
}

void congratulations(Board b)
{
    printf("\n");
    print_game(b);
    if(winner(b,BLUE,RED) == BLUE)
        printf("\nCongratulazioni giocatore BLUE! Hai vinto\n");
    else if(winner(b,BLUE,RED) == RED)
        printf("\nCongratulazioni giocatore RED! Hai vinto");
}

int partita1v1(Board b, enum giocatore player1, enum giocatore player2)
{

    if(winner(b,player1,player2)==-1)
    {
        int x1,y1,x2,y2;

        printf("\n\n");
        print_game(b);

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

        if(entro_limiti(x1,y1) && entro_limiti(x2,y2) && b->vet[x1][y1]
            && b->vet[x1][y1]->colore == player1 && muovi_legale_wrapper(b->vet[x1][y1],x2,y2,b))
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
    congratulations(b);
    return winner(b,player1,player2);
}


int partita1vCPU(Board b, enum giocatore player1, enum giocatore player2)
{
    if(winner(b,player1,player2)==-1)
    {
        int x1,y1,x2,y2;

        printf("\n\n");
        print_game(b);

        if(player1==BLUE)
        {
            printf("\nGiocatore BLU: \n");
            printf("Inserisci le coordinate della pedina da muovere!\n x: ");
            scanf("%d",&x1);
            printf("y: ");
            scanf("%d",&y1);
            printf("\nOra inserisci le coordinate in cui muoverti.\n x2: ");
            scanf("%d",&x2);
            printf("y2: ");
            scanf("%d",&y2);

            if(entro_limiti(x1,y1) && entro_limiti(x2,y2) && b->vet[x1][y1]
               && b->vet[x1][y1]->colore == player1 && muovi_legale_wrapper(b->vet[x1][y1],x2,y2,b))
            {
                printf("\nMosso da x,y (%d,%d)",x1,y1);
                printf("\nA x2,y2 (%d,%d)",x2,y2);
                return partita1vCPU(b,player2,player1);
            }
            else
            {
                printf("\nMossa illegale, rifai!");
                return partita1vCPU(b,player1,player2);
            }
        }
        else
        {
            printf("\nL'intelligenza del computer ha scelto saggiamente...\n");
            ai_move(b,RED);
            return partita1vCPU(b,player2,player1);
        }

    }
    congratulations(b);
    return winner(b,player1,player2);
}

int partitaCPUvCPU(Board b, enum giocatore player1, enum giocatore player2)
{
    if(winner(b,player1,player2)==-1)
    {
        print_game(b);
        ai_move(b,player1);
        return partitaCPUvCPU(b,player2,player1);
    }
    congratulations(b);
    return winner(b,player1,player2);
}

int main_menu()
{
    int mode,gioc;

    Board b;
    b = init_board();

    printf("\nSeleziona la modalita: \n- 1 vs 1 -> [1] \n- 1 vs CPU -> [2] \n- CPU vs CPU -> [3] \n- Exit -> [tasto qualunque]\n");
    scanf("%d",&mode);

    printf("Seleziona il giocatore: \n- BLU -> [1]\n- ROSSO -> [2]\n");
    scanf("%d",&gioc);
    if(mode == 1)
    {
        if(gioc == 1)
            partita1v1(b,BLUE, RED);
        else
            partita1v1(b,RED,BLUE);
    }
    if(mode == 2)
    {
        if(gioc == 1)
            partita1vCPU(b,BLUE, RED);
        else
            partita1vCPU(b,RED,BLUE);
    }
    if(mode == 3)
    {
        if(gioc == 1)
            partitaCPUvCPU(b,BLUE, RED);
        else
            partitaCPUvCPU(b,RED,BLUE);
    }
    delete_board(&b);
    return 0;
}

int main() {

    main_menu();

}