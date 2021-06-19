//
// Created by alece on 11/02/2021.
//

#ifndef UNTITLED1_AI_H
#define UNTITLED1_AI_H
#define RICORSIONI 4
#define MOSSETOT 16
/*
 *  Il metodo punti_percorso, data una scacchiera Board, il colore di un giocatore, il numero della chiamata ricorsiva, il colore di partenza,
 *  e delle variabili per restituire le coordinate finali:
 *      Simula ricorsivamente su una board temporanea le possibili mosse dei giocatori andando a generare un punteggio per ogni "percorso"
 *      esistente.
 *      Ogni pedina avrà associato un array di coordinate con tutte le mosse disponibili, che vengono analizzate e
 *      infine generano due coordinate che rappresentano la posizione più profiqua in cui muoversi.
 *      La posizione migliore è calcolata simulando una serie di mosse e aggiungendo ricorsivamente al punteggio finale +1 se si mangia
 *      o -1 se si viene mangiati. 0 invece se si muove e basta;
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
                    int vett[MOSSETOT] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

                    if (has_moves(b->vet[i][j], b, vett)) { //se la pedina esiste ed è del colore desiderato, se ha mosse le inserisco nell'array

                        int highest;
                        int xtmp,ytmp;
                        highest = -100;

                        for (int k = 0; k < MOSSETOT && vett[k] != -1; k += 2) { //studio tutte le mosse della pedina
                            Board tmp;
                            tmp = clone_board(b); // clono la board corrente per poter simulare una mossa senza corrompere la board vera

                            if(tmp && mangia_legale(tmp->vet[i][j],vett[k],vett[k+1],tmp)) //se può effettuare una mangiata legale
                            {
                                mangia(tmp->vet[i][j],vett[k],vett[k+1],tmp);   //effettua il movimento e lo simula nella board nuova
                                int chiamata_ricorsiva = (altro == color_start) ?
                                                         1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out)
                                                                                :  - 1 + punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);
                                //^^ fa la chiamata ricorsiva con i punti percorso +1 o -1 (rispettivamente se il colore di partenza ha mangiato o è stato mangiato)
                                if(chiamata_ricorsiva > highest){
                                    xtmp = vett[k];
                                    ytmp = vett[k+1];
                                    highest = chiamata_ricorsiva;
                                }
                            }
                            else if(tmp && cattura_forzata(b,player)==0 && mossa_legale(tmp->vet[i][j],vett[k],vett[k+1],tmp))
                            {
                                muovi_legale_wrapper(tmp->vet[i][j],vett[k],vett[k+1],tmp);
                                int chiamata_ricorsiva = punti_percorso(tmp,altro,rec+1,color_start,x1out,y1out,x2out,y2out);
                                //^^ fa la chiamata ricorsiva senza aggiungere un punteggio perchè ha mosso e basta
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
                                *y2out = ytmp; //assegna alle variabili di output le coordinate più efficienti di tutta la tabella
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

/*
 *  Inserisce nei puntatori ad intero passati in input le coordinate iniziali e finali della prima mossa disponibile di una pedina,
 *  restituisce 0 se non ha mosse disponibili, 1 altrimenti.
 */
int prima_mossa(Board b,int *x1,int *y1,int *x2, int *y2, enum giocatore player)
{
    int flag;
    int vett[MOSSETOT] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; //vettore inizializzato con sentinelle

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

//TODO
int cpu_strategy_move(Board b, enum giocatore player)
{

}

/*
 * Data una scacchiera b in input e un giocatore, la funzione ai_move utilizza la ausiliaria punti_percorso per determinare
 * quale percorso generi più punti e salvando le coordinate in delle variabili che saranno usate poi per muovere con la
 * muovi_legale_wrapper.
 */
int ai_move(Board b, enum giocatore pcplayer)
{
    int x,y,x2,y2;
    if(winner(b,BLUE,RED)==-1 ) // se non c'è nessun vincitore
    {
        if(prima_mossa(b,&x,&y,&x2,&y2,pcplayer)) //se il giocatore ha mosse disponibili prima_mossa restituisce 1 e salva le coordinate di quella mossa
            punti_percorso(b,pcplayer,0,pcplayer,&x,&y,&x2,&y2); // poi vengono passate la board, il player e le coordinate a punti percorso che valuta qual'è il percorso più efficiente
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
