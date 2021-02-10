#include <stdio.h>
#include <stdlib.h>
#include "Pedina.h"
#include "Board.h"

#define MOSSE_DISPONIBILI 64

void congratulations(Board b)
{
    print_board(b);
    if(winner(b,BLUE,RED) == BLUE)
        printf("\n\Congratulazioni giocatore BLUE! Hai vinto\n");
    else if(winner(b,BLUE,RED) == RED)
        printf("\nCongratulazioni giocatore RED! Hai vinto");
}

int partita1v1(Board b, enum giocatore player1, enum giocatore player2)
{

    if(winner(b,player1,player2)==-1)
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
    congratulations(b);
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

    if(rec < 3 && winner(b,BLUE,RED) == -1) {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (b->vet[i][j] && b->vet[i][j]->colore == player) {
                    int vett[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                    if (has_moves(b->vet[i][j], b, vett)) {
                        int highest;
                        int xtmp,ytmp;
                        highest = -100;

                        for (int k = 0; k < 8 && vett[k] != 0; k += 2) {
                            Board tmp;
                            tmp = clone_board(b);
                            if(tmp && mangia_legale(tmp->vet[i][j],vett[k],vett[k+1],tmp))
                            {
                                mangia(tmp->vet[i][j],vett[k],vett[k+1],tmp);
                                int chiamata_ricorsiva = (altro == color_start) ?
                                    1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out)
                                    :  - 1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);

                                if(i==6 || i==2)
                                {
                                    if(b->vet[i][j]->stato != GENERALE)
                                        chiamata_ricorsiva+=1;
                                }
                                if(winner(tmp,player,altro) != -1)
                                {
                                    chiamata_ricorsiva = (player != color_start) ? chiamata_ricorsiva-10 : chiamata_ricorsiva+10;
                                }
                                if(chiamata_ricorsiva > highest){

                                    xtmp = vett[k];
                                    ytmp = vett[k+1];
                                    highest = chiamata_ricorsiva;
                                }
                            }
                            else if(tmp && mossa_legale(tmp->vet[i][j],vett[k],vett[k+1],tmp))
                            {
                                muovi_legale_wrapper(tmp->vet[i][j],vett[k],vett[k+1],tmp);
                                int chiamata_ricorsiva = punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);

                                if(i==6 || i==2)
                                {
                                    if(b->vet[i][j]->stato != GENERALE)
                                        chiamata_ricorsiva+=1;
                                }
                                if(winner(tmp,player,altro) != -1)
                                {
                                    chiamata_ricorsiva = (player != color_start) ? chiamata_ricorsiva-10 : chiamata_ricorsiva+10;
                                }
                                if(chiamata_ricorsiva > highest){
                                    if(player == color_start)
                                    {
                                        xtmp = vett[k];
                                        ytmp = vett[k+1];
                                    }
                                    highest = chiamata_ricorsiva;
                                }
                            }
                            delete_board(&tmp);
                        }
                        if(highest > maxpunti)
                        {
                            maxpunti = highest;
                            if(rec==0)
                            {
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
    }
    if(maxpunti == -100)
        return 0;
    return maxpunti;
}
int prima_mossa(Board b,int *x1,int *y1,int *x2, int *y2, enum giocatore player)
{
    int vett[8] = {0,0,0,0,0,0,0,0};
    int flag;
    flag = 0;

    for (int i = 0; i < 7 && !flag; ++i) {
        for (int j = 0; j < 7 && !flag; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore == player)
            {
                if(has_moves(b->vet[i][j],b,vett))
                {
                    flag = 1;
                    *x1 = i;
                    *y1 = j;
                    *x2 = vett[0];
                    *y2 = vett[1];
                }
            }
        }
    }
    return flag;
}

int ai_move(Board b, enum giocatore pcplayer)
{
    int x,y,x2,y2;
    if(winner(b,BLUE,RED)==-1 )
    {
        if(prima_mossa(b,&x,&y,&x2,&y2,pcplayer))
            punti_percorso(b,pcplayer,0,pcplayer,&x,&y,&x2,&y2);
        if(muovi_legale_wrapper(b->vet[x][y],x2,y2,b)){ //&& entro_limiti(x,y) && entro_limiti(x2,y2)
            printf("\n~ Ho mosso da (%d,%d) a (%d,%d)\n",x,y,x2,y2);
            return 1;
        }
        printf("non ho mosso da (%d,%d) a (%d,%d)\n",x,y,x2,y2);
        return 0;
    }
    return 0;
}

int partita1vCPU(Board b, enum giocatore player1, enum giocatore player2)
{
    if(winner(b,player1,player2)==-1)
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
    congratulations(b);
    return winner(b,player1,player2);
}

int partitaCPUvCPU(Board b, enum giocatore player1, enum giocatore player2)
{
    if(winner(b,player1,player2)==-1)
    {
        if(ai_move(b,player1))
            print_board(b);
        return partitaCPUvCPU(b,player2,player1);
    }
    congratulations(b);
    return winner(b,player1,player2);
}

int main_menu()
{
    int mode,gioc;

    Board b;
    enum giocatore player1 = BLUE;
    enum giocatore player2 = RED;
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
