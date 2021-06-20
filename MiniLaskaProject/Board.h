#ifndef UNTITLED1_BOARD_H
#define UNTITLED1_BOARD_H
#define DIMENSION 7

typedef struct board {
    Pedina_list vet[DIMENSION][DIMENSION];
} *Board;

/**
 * Inizializza una Board in memoria e la restituisce come output, per ogni casella viene
 * inizializzata una nuova Pedina_list attraverso la primitiva init_pedina.
 * Il campo viene generato con le pedine rosse e blu nei rispettivi posti e il puntatore
 * delle caselle vuote viene inizializzato a null.
 * @return una Board nuova in memoria, inizializzata con le pedine nei posti iniziali
 */
Board init_board()
{
    Board b = malloc(sizeof(struct board));

    if(b)
    {
        int i, j;
        for (i = 0; i < DIMENSION; ++i) {
            for (j = 0; j < DIMENSION; ++j) {
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

/**
 * Funzione di utility, inizializza una Board vuota senza le pedine.
 * Utilizzata nella clone_board.
 * @return una Board nuova, inizializzata senza pedine
 */
Board init_empty_board()
{
    Board b = malloc(sizeof(struct board));

    if(b)
    {
        int i, j;
        for (i = 0; i < DIMENSION; ++i) {
            for (j = 0; j < DIMENSION; ++j) {
                b->vet[i][j] = NULL;
            }
        }
        return b;
    }
    return NULL;
}
/**
 * Funzione che viene chiamata alla fine del ciclo di vita di ogni Board per essere liberata dalla memoria.
 * Agisce specularmente alla board_create, utilizzando la funzione delete_pedina che si occupa di fare la free
 * di una singola pedina.
 * @param b il puntatore alla scacchiera da eliminare
 */
void delete_board(Board *b)
{
    if(*b)
    {
        Board *tmp;
        tmp = b;
        int i, j;
        for (i = 0; i < DIMENSION; ++i) {
            for (j = 0; j < DIMENSION; ++j) {
                delete_pedina(&(*b)->vet[i][j]);
            }
        }
        free(*tmp);
        *b = NULL;
    }
}
/**
 * Crea in memoria una nuova board, la inizializza clonando una board già esistente,
 * dopodichè restituisce in output il puntatore. Avendo così il riferimento ad una
 * board nuova di zecca. (Viene usata ad esempio nell'intelligenza artificiale per "simulare"
 * una mossa senza compromettere la board corrente)
 * @param b la board da clonare
 * @return una nuova Board con un nuovo puntatore
 */
Board clone_board(Board b)
{
    Board tmp;
    tmp = init_empty_board();
    int i,j;

    for (i = 0; i < DIMENSION; ++i) {
        for (j = 0; j < DIMENSION; ++j) {
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

/**
 * Controlla se due coordinate sono entro i limiti della scacchiera
 * @param x la coordinata x da testare
 * @param y la coordinata y da testare
 * @return 1 se la mossa è entro i limiti della scacchiera, 0 altrimenti
 */
int entro_limiti(int x, int y)
{
    return !(x<0 || x>6 || y<0 || y>6);
}

/**
 * Data una pedina appartenente alla scacchiera, la scacchiera e delle coordinate finali,
 * MUOVE una pedina forzatamente, ATTENZIONE, NON controlla se la mossa è valida secondo le regole di gioco,
 * ma solo se la mossa è entro i limiti del campo.
 * Inoltre, dopo che ha mosso trasforma la pedina in GENERALE se si muove sul fondo del campo
 *
 * @param p la pedina da muovere
 * @param b la board in cui muovere
 * @param x le coordinate x in cui muovere
 * @param y le coordinate y in cui muovere
 * @return 1 se i parametri usati esistono, sono entro i limiti e se infine ho mosso, 0 altrimenti
 */
int muovi(Pedina_list p,int x, int y,Board b)
{
    if(!p) return 0;
    if(entro_limiti(x,y)&& !b->vet[x][y]) /*se le coordinate finali sono nel campo e se nella destinazione non c'è nessuna pedina*/
    {
        Pedina_list tmp;
        tmp = b->vet[p->coordx][p->coordy];

        b->vet[p->coordx][p->coordy] = b->vet[x][y];
        b->vet[x][y] = tmp;

        if (x == 0 || x == 6) /* se colpisce il fondo della mappa diventa GENERALE (un SOLDATO può andare solo in avanti)*/
            p->stato = GENERALE;

        /*assegno le nuove coordinate allo stack di pedine*/
        p->coordx = x;
        p->coordy = y;
        if(p->next)
        {
            p->next->coordx = x;
            p->next->coordy = y;
            if(p->next->next)
            {
                p->next->next->coordx = x;
                p->next->next->coordy = y;
            }
        }

        return 1;
    }
    else return 0;
}

/**
 * Questa funzione di utility controlla se una Pedina_list può essere mossa in una determinata
 * posizione secondo il movimento contemplato dal gioco (diagonale di 1 o 2 quadretti).
 * - se non è possibile con gli argomenti proposti restituisce 0;
 * - se è possibile muovere restituisce 1;
 *
 *  restituisce 1 se la mossa proposta da p->coordx,p->coordy a x, y è legale, ma NON muove.
 *
 *  @param p la pedina da muovere
 *  @param x la x in cui muovere
 *  @param y la y in cui muovere
 *  @param b la board da cui muovere
 *  @param k la lunghezza del "salto" ovvero 1 se muovo normalmente 2 se mangio
 *  @return 1 se il movimento della mosse è legale (ovvero se si muove diagonalmente di 1 o di 2), 0 altrimenti
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

/**
 * Controlla se una mossa (mossa ovvero movimento di 1 e non una "mangiata") può essere fatta, ATTENZIONE, SENZA svolgerla
 *
 * @param p la pedina da muovere
 * @param x la coordinata x in cui muovere
 * @param y la coordinata y in cui muovere
 * @param b la board da cui muovere
 * @return 1 se la pedina può effettuare il movimento, 0 altrimenti
 */
int mossa_legale(Pedina_list p,int x, int y,Board b)
{
    return legale(p,x,y,b,1);
}

/**
 * Controlla se una pedina può effettuare una "mangiata" nelle coordinate proposte ,
 * poi calcola le coordinate della pedina che sta venendo mangiata
 * e controlla che in quelle coordinate ci sia una pedina di colore diverso
 * dalla pedina in input,
 * restituisce 1 se è una mossa di mangiata valida ma NON mangia ancora, 0 altrimenti
 *
 * ATTENZIONE NON effettua la mossa controlla solo che sia legale
 *
 * @param p la pedina da muovere
 * @param x la coordinata x in cui muovere
 * @param y la coordinata y in cui muovere
 * @param b la board da cui muovere
 * @return 1 se la pedina può effettuare una "mangiata" in coordinate x e y, 0 altrimenti
 */
int mangia_legale(Pedina_list p,int x, int y,Board b)
{
    int mangiatox;
    int mangiatoy;

    if(!p) return 0;
    mangiatox = (p->coordx+x)/2;
    mangiatoy = (p->coordy+y)/2;

    return (legale(p,x,y,b,2) && b->vet[mangiatox][mangiatoy] && (b->vet[mangiatox][mangiatoy]->colore != p->colore));
}

/**
 * Funzione wrapper principale per mangiare in modo safe, se la pedina esiste, la board esiste
 * e se la funzione "mangia_legale" restituisce true,
 * fa la append alla lista di pedine in posizione p->coordx e p->coordy dell'elemento in testa
 * alla lista di pedine che viene mangiata, poi lo elimina dalla pedina di partenza.
 *
 * ATTENZIONE effettua il movimento
 *
 * @param p la pedina da muovere
 * @param x la coordinata x in cui muovere
 * @param y la coordinata y in cui muovere
 * @param b la board da cui muovere
 * @return 1 se la pedina ha mangiato correttamente un'altra pedina, 0 altrimenti
 */
int mangia(Pedina_list p, int x, int y, Board b)
{
    if(p && b)
    {
        if(mangia_legale(p,x,y,b)) /* se è possibile mangiare legalmente nelle coordinate proposte*/
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

/** riceve un colore in input, se non viene trovata neanche una pedina del colore opposto ritorna 1, altrimenti 0
 * @param player il colore del giocatore da controllare
 * @param b la board in cui controllare
 * @return 1 se non ci sono pedine del colore opposto a player, 0 altrimenti
 */
int has_all_pieces(enum giocatore player, Board b)
{
    int flg,i,j;
    flg = 1;
    for (i = 0; i < DIMENSION; ++i) {
        for (j = 0; j < DIMENSION; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore != player)
                flg = 0;
        }
    }
    return flg;
}
/**
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
 * @param p la pedina controllare
 * @param b la board che contiene la pedina
 * @param coords il vettore di mosse da dare in input che viene riempito per side effect con tutte le mosse di una pedina.
 * @return 1 se una pedina ha mosse, 0 altrimenti
 */
int has_moves(Pedina_list p,Board b,int *coords)
{
    int c;
    int flag;
    flag = 0;
    c = 0;

    if(p)
    {
        int i;
        for (i = 1; i < 3; ++i) {
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

/**
 * Funzione che viene invocato ogni volta che si prova a muovere legalmente, fa fallire la mossa
 * e avvisa che è obbligatorio mangiare in caso sia necessaria una cattura forzata
 * @param b la board in cui controllare
 * @param p1 il colore del giocatore da controllare
 * @return 1 se è necessario catturare per forza, 0 altrimenti
 */
int cattura_forzata(Board b, enum giocatore p1)
{
    int flag;
    int i,j,k;
    flag = 0;

    for(i = 0;i<DIMENSION;i++)
    {
        for(j = 0; j<DIMENSION; j++)
        {
            if(b->vet[i][j] && b->vet[i][j]->colore == p1)
            {
                int vet[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
                if(has_moves(b->vet[i][j],b,vet)) /*controllo se c'è una pedina da mangiare, in caso deve farlo!*/
                {
                    for (k = 0; k < 16 && vet[k] != -1; k+=2) {
                        if(mangia_legale(b->vet[i][j],vet[k],vet[k+1],b))
                            flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}

/**
 * Questa è la funzione più importante che viene invocata dopo una mossa generica,
 * controlla se il movimento è legale come "semplice movimento" con la funzione "mossa_legale",
 * controlla in altro caso se è una "mossa per mangiare" con la funzione "mangia_legale",
 * infine a seconda del tipo di movimento muove con la "muovi" e restituisce 1 se va a buon fine,
 * restituisce 0 se non è una mossa legale e neanche un mangia legale
 *
 * @param p la pedina da muovere
 * @param x la coordinata x in cui muovere
 * @param y la coordinata y in cui muovere
 * @param b la board da cui muovere
 * @return 1 se la mossa è stata effettuata con successo, 0 altrimenti. Nel primo caso muove la pedina per side effect.
 */
int muovi_legale_wrapper(Pedina_list p,int x, int y,Board b)
{
    if(cattura_forzata(b,p->colore) == 0)
    {
        if(mangia_legale(p,x,y,b))
            return mangia(p,x,y,b);
        else if(mossa_legale(p,x,y,b))
            return muovi(p,x,y,b);
        else return 0;
    }
    else
    {
        if(mangia_legale(p,x,y,b))
            return mangia(p,x,y,b);
        else
        {
            printf("\nAttento, devi mangiare\n");
            return 0;
        }
    }

}

/**
 * Dato un giocatore e una Board, utilizza la funzione "has_moves" per controllare
 * se tutte le pedine di un giocatore hanno finito le mosse disponibili,
 * in tal caso restuisce 0, se un giocatore ha ancora mosse restituisce 1;
 * @param player il giocatore da controllare
 * @param b la board in cui controllare
 * @return 1 se il giocatore player ha ancora mosse disponibili nella scacchiera, 0 altrimenti
 */
int player_has_moves(enum giocatore player, Board b)
{
    int flag = 0,i,j;
    int moves[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

    for (i = 0; i < DIMENSION; ++i) {
        for (j = 0; j < DIMENSION; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore == player && has_moves(b->vet[i][j],b,moves))
                flag = 1;
        }
    }
    return flag;
}

/**
 * ritorna il colore del vincitore, -1 se non esiste
 * @param board la board da cui controllare
 * @param player1 il primo giocatore
 * @param player2 il secondo giocatore
 * @return il colore del giocatore che ha vinto, -1 se nessuno dei due ha vinto
 */
enum giocatore winner(Board board,enum giocatore player1,enum giocatore player2)
{
    if(!player_has_moves(player2,board) || has_all_pieces(player1,board))
        return player1;
    if(!player_has_moves(player1,board) || has_all_pieces(player2,board))
        return player2;
    return -1;
}

#endif //UNTITLED1_BOARD_H
