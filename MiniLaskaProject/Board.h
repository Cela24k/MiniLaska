//
// Created by alece on 08/02/2021.
//

#ifndef UNTITLED1_BOARD_H
#define UNTITLED1_BOARD_H
#define DIMENSION 7

typedef struct board {
    Pedina_list vet[DIMENSION][DIMENSION];
} *Board;

/*
 * Inizializza una Board in memoria e la restituisce come output, per ogni casella viene
 * inizializzata una nuova Pedina_list attraverso la primitiva init_pedina.
 * Il campo viene generato con le pedine rosse e blu nei rispettivi posti e il puntatore
 * delle caselle vuote viene inizializzato a null.
 */
Board init_board()
{
    Board b = malloc(sizeof(struct board));

    if(b)
    {
        for (int i = 0; i < DIMENSION; ++i) {
            for (int j = 0; j < DIMENSION; ++j) {
                b->vet[i][j] = NULL;
                if((i+j)%2 == 0)
                {
                    if(i<3) b->vet[i][j] = init_pedina(i,j,BLUE,PEDINA);
                    if(i == 3) b->vet[i][j] = NULL;
                    if(i>3) b->vet[i][j] = init_pedina(i,j,RED,PEDINA);
                }
            }
        }
        return b;
    }
    return NULL;
}

/*
 * Funzione di utility, inizializza una Board vuota senza le pedine.
 * Utilizzata nella clone_board.
 */
Board init_empty_board()
{
    Board b = malloc(sizeof(struct board));

    if(b)
    {
        for (int i = 0; i < DIMENSION; ++i) {
            for (int j = 0; j < DIMENSION; ++j) {
                b->vet[i][j] = NULL;
            }
        }
        return b;
    }
    return NULL;
}
/*
 * Funzione che viene chiamata alla fine del ciclo di vita di ogni Board per essere liberata dalla memoria.
 * Agisce specularmente alla board_create, utilizzando la funzione delete_pedina che si occupa di fare la free
 * di una singola pedina.
 */
void delete_board(Board *b)
{
    if(*b)
    {
        Board *tmp;
        tmp = b;
        for (int i = 0; i < DIMENSION; ++i) {
            for (int j = 0; j < DIMENSION; ++j) {
                delete_pedina(&(*b)->vet[i][j]);
            }
        }
        free(*tmp);
        *b = NULL;
    }
}
/*
 * Crea in memoria una nuova board, la inizializza clonando una board già esistente,
 * dopodichè restituisce in output il puntatore. Avendo così il riferimento ad una
 * board nuova di zecca. (Viene usata ad esempio nell'intelligenza artificiale per "simulare"
 * una mossa senza compromettere la board corrente)
 */
Board clone_board(Board b)
{
    Board tmp;
    tmp = init_empty_board();

    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            if(b->vet[i][j])
            {
                Pedina_list p;
                p = b->vet[i][j];

                while(p)
                {
                    if(append(&tmp->vet[i][j],*p))
                        p = p->next;
                }
            }
        }
    }
    if(tmp)
        return tmp;
    else return NULL;
}

// Controlla se due coordinate sono entro i limiti della scacchiera
int entro_limiti(int x, int y)
{
    return !(x<0 || x>6 || y<0 || y>6);
}

/* Data una pedina appartenente alla scacchiera, la scacchiera e delle coordinate finali,
 * muove una pedina, ATTENZIONE, NON controlla se la mossa è valida secondo le regole di gioco,
 * ma solo se la mossa è entro i limiti del campo.
 * Inoltre, dopo che ha mosso trasforma la pedina in GENERALE se si muove sul fondo del campo
 */
int muovi(Pedina_list p,int x, int y,Board b)
{
    if(!p) return 0;
    if(entro_limiti(x,y)&& !b->vet[x][y]) // se le coordinate finali sono nel campo e se nella destinazione non c'è nessuna pedina
    {
        Pedina_list tmp;
        tmp = b->vet[p->coordx][p->coordy];

        b->vet[p->coordx][p->coordy] = b->vet[x][y];
        b->vet[x][y] = tmp;

        if (x == 0 || x == 6) // se colpisce il fondo della mappa diventa GENERALE (un SOLDATO può andare solo in avanti)
            p->stato = GENERALE;
        p->coordx = x;
        p->coordy = y;
        return 1;
    }
    else return 0;
}

/*
 * Questa funzione controlla se una Pedina_list può essere mossa in una determinata
 * posizione secondo le regole di gioco.
 * - se non è possibile con gli argomenti proposti restituisce 0;
 * - se è possibile muovere restituisce 1;
 *
 *  restituisce 1 se la mossa proposta da p->coordx,p->coordy a x, y è legale, ma NON muove.
 */
int legale(Pedina_list p,int x, int y,Board b,int k)
{
    if(!p || b->vet[x][y] != NULL) return 0;
    if((x == p->coordx + k || x == p->coordx - k) && (y <= p->coordy +k || y == p->coordy -k))
        if(p->stato==GENERALE)
            return 1;
        else
        {
            if(p->colore == BLUE)
            {
                if((x == p->coordx + k) && (y == p->coordy - k) || (x == p->coordx + k) && (y == p->coordy + k))
                    return 1;
                else return 0;
            }
            else
            {
                if((x == p->coordx - k) && (y == p->coordy - k) || (x == p->coordx - k) && (y == p->coordy + k))
                    return 1;
                else return 0;
            }
        }
    else return 0;
}

/*
 * controlla se la mossa può essere fatta, senza svolgerla
 * */
int mossa_legale(Pedina_list p,int x, int y,Board b)
{
    return legale(p,x,y,b,1);
}


/*
 * controlla se la pedina in input esiste,
 * poi calcola le coordinate della pedina che sta venendo mangiata
 * e controlla che in quelle coordinate ci sia una pedina di colore diverso
 * dalla pedina in input,
 * restituisce 1 se è una mossa di mangiata valida ma NON mangia ancora, 0 altrimenti
 * */
int mangia_legale(Pedina_list p,int x, int y,Board b)
{
    int mangiatox;
    int mangiatoy;

    if(!p) return 0;
    mangiatox = (p->coordx+x)/2;
    mangiatoy = (p->coordy+y)/2;

    return (legale(p,x,y,b,2) && b->vet[mangiatox][mangiatoy] && (b->vet[mangiatox][mangiatoy]->colore != p->colore));
}

/*
 * funzione per mangiare in modo safe, se la pedina esiste, la board esiste
 * e se la funzione "mangia_legale" restituisce true,
 * fa la append alla lista di pedine in posizione p->coordx e p->coordy dell'elemento in testa
 * alla lista di pedine che viene mangiata, poi lo elimina dalla pedina di partenza.
 * */
int mangia(Pedina_list p, int x, int y, Board b)
{
    if(p && b)
    {
        if(mangia_legale(p,x,y,b))
        {
            int xmangiato, ymangiato;

            xmangiato = (p->coordx + x) / 2;
            ymangiato = (p->coordy + y) / 2;

            append(&b->vet[p->coordx][p->coordy],*b->vet[xmangiato][ymangiato]);
            elimina_testa(&b->vet[xmangiato][ymangiato]);
            if(contastack(b->vet[p->coordx][p->coordy])>3)
                elimina_coda(&b->vet[p->coordx][p->coordy]);
            muovi(p,x,y,b);
            return 1;
        }
        return 0;
    }
    return 0;
}

/*
 * Questa è la funzione più importante che viene invocata dopo una mossa generica,
 * controlla se il movimento è legale come "semplice movimento" con la funzione "mossa_legale",
 * controlla in altro caso se è una "mossa per mangiare" con la funzione "mangia_legale",
 * infine a seconda del tipo di movimento muove con la "muovi" e restituisce 1 se va a buon fine,
 * restituisce 0 se non è una mossa legale e neanche un mangia legale
 * */
int muovi_legale_wrapper(Pedina_list p,int x, int y,Board b)
{
    if(mossa_legale(p,x,y,b))
        return muovi(p,x,y,b);
    else if(mangia_legale(p,x,y,b))
        return mangia(p,x,y,b);
    else return 0;
}

/* riceve un colore in input, se non viene trovata neanche una pedina del colore opposto ritorna 1,
 * altrimenti 0 */
int has_all_pieces(enum giocatore player, Board b)
{
    int flg = 1;
    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore != player)
                flg = 0;
        }
    }
    return flg;
}
/*
 * Questa funzione data una pedina, una board e un vettore per immagazzinare un output:
 *
 *      restituisce:
 *          1  se la pedina ha mosse
 *          0  se la pedina non ha mosse disponibili
 *         -1  se non esiste la pedina
*
 *      inoltre nell'array fornito in input inserisce le coordinate delle mosse disponibili con formato:
 *
 *          a[0] = x1 , a[1] = y1 , a[n] = xn, a[n+1] = yn;
 *
 */
int has_moves(Pedina_list p,Board b,int *coords)
{
    int c;
    int flag;
    flag = 0;
    c = 0;

    if(p)
    {
        for (int i = 1; i < 3; ++i) {
            if(entro_limiti(p->coordx+i,p->coordy+i) && (mossa_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy+i,b)
               || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy+i,b)))
            {
                flag = 1;
                coords[c] = p->coordx+i;
                coords[c+1] = p->coordy+i;
                c+=2;
            }
            if(entro_limiti(p->coordx-i,p->coordy-i) && (mossa_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy-i,b)
               || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy-i,b)))
            {
                flag = 1;
                coords[c] = p->coordx-i;
                coords[c+1] = p->coordy-i;
                c+=2;
            }
            if(entro_limiti(p->coordx+i,p->coordy-i) && (mossa_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy-i,b)
               || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy-i,b)))
            {
                flag = 1;
                coords[c] = p->coordx+i;
                coords[c+1] = p->coordy-i;
                c+=2;
            }
            if(entro_limiti(p->coordx-i,p->coordy+i) && (mossa_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy+i,b)
               || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy+i,b)))
            {
                flag = 1;
                coords[c] = p->coordx-i;
                coords[c+1] = p->coordy+i;
                c+=2;
            }
        }
        return flag;
    }
    else return -1;
}

/*
 * Dato un giocatore e una Board, utilizza la funzione "has_moves" per controllare
 * se tutte le pedine di un giocatore hanno finito le mosse disponibili,
 * in tal caso restuisce 0, se un giocatore ha ancora mosse restituisce 1;
 */
int player_has_moves(enum giocatore player, Board b)
{
    int flag = 0;
    int moves[8] = {0,0,0,0,0,0,0,0};

    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore == player && has_moves(b->vet[i][j],b,moves))
                flag = 1;
        }
    }
    return flag;
}

//ritorna il colore del vincitore, -1 se non esiste
enum giocatore winner(Board board,enum giocatore player1,enum giocatore player2)
{
    if(!player_has_moves(player2,board) || has_all_pieces(player1,board))
        return player1;
    if(!player_has_moves(player1,board) || has_all_pieces(player2,board))
        return player2;
    return -1;
}

#endif //UNTITLED1_BOARD_H
