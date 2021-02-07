#include <stdio.h>
#include <stdlib.h>

enum color{BLUE,RED};
enum stato {PEDINA,GENERALE};

typedef struct pedina
{
    int coordx;
    int coordy;
    enum color colore;
    enum stato stato;
    struct pedina *next;
} *Pedina_list;

typedef struct board{
    Pedina_list vet[7][7];
} *Board;

Pedina_list init_pedina(int x, int y, enum color colore,enum stato s)
{
    Pedina_list p;
    p = malloc(sizeof(struct pedina));
    p->coordx = x;
    p->coordy = y;
    p->colore = colore;
    p->stato = s;
    p->next = NULL;

    return p;
}

Board init_board()
{
    Board b = malloc(sizeof(struct board));

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
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

int contastack(Pedina_list pedina) // utility, restituisce quante pedine ha un determinato stack di pedine
{
    int c;
    Pedina_list tmp;
    c = 0;
    tmp = pedina;

    while(tmp)
    {
        tmp = tmp->next;
        c++;
    }

    return c;
}

void append(Pedina_list *p,struct pedina p1)
{
    Pedina_list tmp;
    Pedina_list scorri;
    tmp = (Pedina_list)malloc(sizeof(struct pedina));
    tmp->coordx = p1.coordx;
    tmp->coordy = p1.coordy;
    tmp->colore = p1.colore;
    tmp->stato = p1.stato;
    tmp->next = NULL;
    scorri = *p;

    if(scorri)
    {
        while(scorri->next)
        {
            scorri = (scorri)->next;
        }
        scorri->next = tmp;
    }
    else
    {
        scorri = tmp;
    }
}

void elimina_testa(Pedina_list *p)
{
    if(*p)
    {
        Pedina_list *tmp;
        tmp = p;
        *p = (*p)->next;
        free(tmp);
    }
}

void elimina_coda(Pedina_list *p)
{
    Pedina_list tmp;
    Pedina_list tmp2;
    tmp = *p;
    if(tmp)
    {
        while(tmp->next)
        {
            tmp2 = tmp;
            tmp = tmp->next;
        }
        tmp2->next = NULL;
        free(tmp);
    }
}

void print_row_occ(Board b,int r)
{
    printf("    ");
    for(int i = 0; i < 7; ++i) {
        printf("%d ",contastack(b->vet[r][i]));
    }
}

void print_board(Board board)
{
    if(!board) return;
    printf("  0 1 2 3 4 5 6 \n");
    for (int i = 0; i < 7; ++i) {
        printf("%d ",i);
        for (int j = 0; j < 7; ++j) {

            if(board->vet[i][j])
            {
                if(board->vet[i][j]->colore == BLUE)
                {
                    if(contastack(board->vet[i][j])>1)
                        printf("B ");
                    else printf("b ");
                }
                if(board->vet[i][j]->colore == RED)
                {
                    if(contastack(board->vet[i][j])>1)
                        printf("R ");
                    else printf("r ");
                }
            }
            else
            {
                printf("%c ",219);
            }
        }
        print_row_occ(board,i);
        printf("\n");
    }
}

/* muove una pedina, ATTENZIONE, NON controlla se la mossa è valida secondo le regole di gioco,
 * ma solo se la mossa è limitata al campo.
 */

int muovi(Pedina_list p,int x, int y,Board b)
{
    if(!p) return 0;
    if(x >= 0 && y >= 0 && x<7 && y<7 && !b->vet[x][y])
    {
        Pedina_list tmp;
        tmp = b->vet[p->coordx][p->coordy];

        b->vet[p->coordx][p->coordy] = b->vet[x][y];
        b->vet[x][y] = tmp;

        while(p)
        {
            if(x == 0 || x==6)
                p->stato = GENERALE;
            p->coordx = x;
            p->coordy = y;
            p = p->next;
        }
        return 1;
    }
    else return 0;
}

/*
 * muove una Pedina_list secondo le regole di gioco,
 * se non è possibile con gli argomenti proposti restituisce 0;
 */
int legale(Pedina_list p,int x, int y,Board b,int k)
{
    if(b->vet[x][y] != NULL) return 0;
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

int mossa_legale(Pedina_list p,int x, int y,Board b)
{
    return legale(p,x,y,b,1);
}

int mangia_legale(Pedina_list p,int x, int y,Board b)
{
    if(!p) return 0;
    int mangiatox = (p->coordx+x)/2;
    int mangiatoy = (p->coordy+y)/2;

    return (legale(p,x,y,b,2)&&b->vet[mangiatox][mangiatoy]&&(b->vet[mangiatox][mangiatoy]->colore != p->colore));
}

int mangia(Pedina_list p, int x, int y, Board b)
{
    if(mangia_legale(p,x,y,b))
    {
        int xmangiato, ymangiato;
        Pedina_list mangiato;

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
int muovi_legale_wrapper(Pedina_list p,int x, int y,Board b)
{
    if(mossa_legale(p,x,y,b))
        return muovi(p,x,y,b);
    else if(mangia_legale(p,x,y,b))
        return mangia(p,x,y,b);
    else return 0;
}

int has_all_pieces(enum color player, Board b)
{
    int flg = 1;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore != player)
                flg = 0;
        }
    }
    return flg;
}

// restituisce true o false (o -1 se non esiste la pedina)
// e UN ARRAY formato da coordinate di tipo: a[0] = x1 , a[1] = y1 , a[n] = xn, a[n+1] = yn;

int has_moves(Pedina_list p,Board b,int *coords)
{
    int c;
    int flag;
    flag = 0;
    c = 0;

    if(p)
    {
        for (int i = 1; i < 3; ++i) {
            if((p->coordx+i >= 0 && p->coordx+i <=6) && mossa_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy+i,b)
                || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy+i,b))
            {
                flag = 1;
                coords[c] = p->coordx+i;
                coords[c+1] = p->coordy+i;
                c+=2;
            }
            if((p->coordx-i >= 0 && p->coordx-i <=6) && mossa_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy-i,b)
                || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy-i,b))
            {
                flag = 1;
                coords[c] = p->coordx-i;
                coords[c+1] = p->coordy-i;
                c+=2;
            }
            if((p->coordx+i >= 0 && p->coordx-i <=6) && mossa_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy-i,b)
                || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx+i,p->coordy-i,b) )
            {
                flag = 1;
                coords[c] = p->coordx+i;
                coords[c+1] = p->coordy-i;
                c+=2;
            }
            if((p->coordx-i >= 0 && p->coordx+i <=6) && mossa_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy+i,b)
                || mangia_legale(b->vet[p->coordx][p->coordy],p->coordx-i,p->coordy+i,b))
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

int player_has_moves(enum color player, Board b)
{
    int flag = 0;
    int moves[8] = {0,0,0,0,0,0,0,0};

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if(b->vet[i][j] && b->vet[i][j]->colore == player && has_moves(b->vet[i][j],b,moves))
                flag = 1;
        }
    }
    return flag;
}

//ritorna il colore del vincitore, -1 se non esiste

enum color winner(Board board,enum color player1,enum color player2)
{
    if(!player_has_moves(player2,board) || has_all_pieces(player1,board))
        return player1;
    if(!player_has_moves(player1,board) || has_all_pieces(player2,board))
        return player2;
    return -1;
}
int entro_limiti(int x, int y)
{
    return  !(x <0 || x>6 || y<0 || y>6);
}
int partita1v1(Board b, enum color player1, enum color player2)
{
    if(winner(b,player1,player2)!=player2)
    {
        int x1,y1,x2,y2;

        printf("\n\n");
        print_board(b);

        if(player1==BLUE)
            printf("\nGiocatore BLU inizia: \n");
        else printf("\nGiocatore ROSSO inizia: \n");

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
void congratulations(Board b)
{
    if(winner(b,BLUE,RED) == BLUE)
        printf("\n\nCongratulazioni giocatore BLUE! Hai vinto\n");
    else if(winner(b,BLUE,RED) == RED)
        printf("\nCongratulazioni giocatore RED! Hai vinto");
}

int main() {
    Board b = init_board();
    partita1v1(b,RED,BLUE);
    congratulations(b);
}
