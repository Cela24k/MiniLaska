#include <stdio.h>
#include <stdlib.h>
#include "Pedina.h"
#include "Board.h"

#define MOSSE_DISPONIBILI 64

int partita1v1(Board b, enum giocatore player1, enum giocatore player2)
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
    return winner(b,player1,player2);
}

int check_move(Board b,int x,int y, Pedina_list p)
{
    if(mangia_legale(b->vet[p->coordx][p->coordy],x,y,b) && (x==6 || x == 0))
        return 1+2;
    if(mangia_legale(b->vet[p->coordx][p->coordy],x,y,b))
        return 1;
    if(mossa_legale(b->vet[p->coordx][p->coordy],x,y,b) && (x==6 || x == 0))
        return 2;
    if(mossa_legale(b->vet[p->coordx][p->coordy],x,y,b))
        return 0;
}

int punti_percorso(Board b,enum giocatore player, int rec, int color_start,int *x1out, int *y1out ,int *x2out,int *y2out)
{
    enum giocatore altro;
    int maxpunti;

    altro = (player == BLUE) ? RED : BLUE;
    maxpunti = -100;

    if(rec < 10 && winner(b,BLUE,RED) != player) {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (b->vet[i][j] && b->vet[i][j]->colore == player) {
                    int vett[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                    if (has_moves(b->vet[i][j], b, vett)) {
                        int highest;
                        int xtmp,ytmp;
                        highest = -100;

                        for (int k = 0; k < 8; k += 2) {
                            Board tmp;
                            tmp = clone_board(b);

                            if(mangia_legale(tmp->vet[i][j],vett[k],vett[k+1],tmp))
                            {
                                mangia(tmp->vet[i][j],k,k+1,tmp);
                                int chiamata_ricorsiva = (altro == color_start) ?
                                    1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out)
                                    :  -1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);

                                if(i==6 || i==2) chiamata_ricorsiva+=2;

                                if(chiamata_ricorsiva > highest){

                                    xtmp = vett[k];
                                    ytmp = vett[k+1];
                                    highest = chiamata_ricorsiva;
                                }
                            }
                            else if(mossa_legale(tmp->vet[i][j],k,k+1,tmp))
                            {
                                muovi(tmp->vet[i][j],k,k+1,tmp);
                                int chiamata_ricorsiva = punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);

                                if(i==6 || i==2) chiamata_ricorsiva+=2;

                                if(chiamata_ricorsiva > highest){
                                    xtmp = vett[k];
                                    ytmp = vett[k+1];
                                    highest = chiamata_ricorsiva;
                                }
                            }
                        }
                        if(highest > maxpunti)
                        {
                            maxpunti = highest;
                            *x1out = i;
                            *y1out = j;
                            *x2out = xtmp;
                            *y2out = ytmp;
                        }
                    }
                }
            }
        }
    }
    if(maxpunti == -100)
        return 0;
    return maxpunti;
}

int ai_move(Board b, enum giocatore pcplayer)
{
    int x,y,x2,y2;
    punti_percorso(b,pcplayer,0,pcplayer,&x,&y,&x2,&y2);
    if(muovi_legale_wrapper(b->vet[x][y],x2,y2,b)){
        printf("~ Ho mosso da (%d,%d) a (%d,%d)",x,y,x2,y2);
        return 1;
    }
    printf("non ho mosso da (%d,%d) a (%d,%d)",x,y,x2,y2);
    return 0;
}

int partita1vCPU(Board b, enum giocatore player1, enum giocatore player2)
{
    if(winner(b,player1,player2)!=player2)
    {
        int x1,y1,x2,y2;

        printf("\n\n");
        print_board(b);

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
    return winner(b,player1,player2);
}

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
    enum giocatore player1 = BLUE;
    enum giocatore player2 = RED;

    if(mode == 1)
    {
        return partita1v1(b,player1,player2);
    }
    delete_board(b);
    return 0;
}

int main() {

    Board tmp;
    Board b = init_board();
    /*
    muovi_legale_wrapper(b->vet[2][2],3,3,b);
    //mangia(b->vet[4][4],2,2,b);
    //tmp = clone_board(b);
    print_board(b);

    //main_menu(1);

    Board b = init_board();
    muovi_legale_wrapper(b->vet[2][2],3,3,b);
    muovi_legale_wrapper(b->vet[4][4],2,2,b);
    muovi_legale_wrapper(b->vet[1][3],3,1,b);
    ai_move(b,RED);
    */
    partita1vCPU(b,BLUE,RED);

}
