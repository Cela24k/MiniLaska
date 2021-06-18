//
// Created by alece on 11/02/2021.
//

#ifndef UNTITLED1_AI_H
#define UNTITLED1_AI_H
#define RICORSIONI 4

/*
 *
 */
int punti_percorso(Board b,enum giocatore player, int rec, int color_start,int *x1out, int *y1out ,int *x2out,int *y2out)
{
    enum giocatore altro;
    int maxpunti;

    altro = (player == BLUE) ? RED : BLUE;
    maxpunti = -100; //vengono immagazzinati i punti di un percorso nella chiamata corrente

    if(rec < RICORSIONI && winner(b,BLUE,RED) == -1) { //rec < 5 se ho fatto meno di 5 chiamate ricorsive

        for (int i = 0; i < 7; ++i) { //vado a studiare tutte le pedine con un for annidato per assegnare a ciascuna pedina un set di mosse disponibili
            for (int j = 0; j < 7; ++j) {
                if (b->vet[i][j] && b->vet[i][j]->colore == player) {
                    int vett[8] = {0, 0, 0, 0, 0, 0, 0, 0};

                    if (has_moves(b->vet[i][j], b, vett)) { //se la pedina esiste ed è del colore desiderato, se ha mosse le inserisco nell'array

                        int highest;
                        int xtmp,ytmp;
                        highest = -100;

                        for (int k = 0; k < 8 && vett[k] != 0; k += 2) { //studio tutte le mosse dell'array
                            Board tmp;
                            tmp = clone_board(b); // clono la board corrente per poter simulare una mossa senza corrompere la board vera

                            if(tmp && mangia_legale(tmp->vet[i][j],vett[k],vett[k+1],tmp)) //se può mangiare predi
                            {
                                mangia(tmp->vet[i][j],vett[k],vett[k+1],tmp);
                                int chiamata_ricorsiva = (altro == color_start) ?
                                                         1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out)
                                                                                :  - 1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);
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
        if(muovi_legale_wrapper(b->vet[x][y],x2,y2,b)){
            printf("\n~ Ho mosso da (%d,%d) a (%d,%d)\n",x,y,x2,y2);
            return 1;
        }
        printf("non ho mosso da (%d,%d) a (%d,%d)\n",x,y,x2,y2);
        return 0;
    }
    return 0;
}

#endif //UNTITLED1_AI_H
