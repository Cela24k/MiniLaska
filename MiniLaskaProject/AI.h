//
// Created by alece on 11/02/2021.
//

#ifndef UNTITLED1_AI_H
#define UNTITLED1_AI_H

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
