#include <stdio.h>
#include <stdlib.h>

enum color{BLUE,RED};

typedef struct pedina
{
    int coordx;
    int coordy;
    enum color colore;
    struct pedina *next;
} *Pedina;

typedef struct board{
    Pedina vet[7][7];
} *Board;

Pedina init_pedina(int x, int y, enum color colore)
{
    Pedina p;
    p = malloc(sizeof(struct pedina));
    p->coordx = x;
    p->coordy = y;
    p->colore = colore;
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
                if(i<3) b->vet[i][j] = init_pedina(i,j,BLUE);
                if(i == 3) b->vet[i][j] = NULL;
                if(i>3) b->vet[i][j] = init_pedina(i,j,RED);
            }
        }
    }
    return b;
}

int contastack(Pedina pedina) // utility, restituisce quante pedine ha un determinato stack di pedine
{
    int c;
    Pedina tmp;
    c = 0;
    tmp = pedina;

    while(tmp)
    {
        tmp = tmp->next;
        c++;
    }

    return c;
}

void aggiungi_pedina(Pedina p, Pedina testa)
{
    if(p)
    {
        if(p->next) aggiungi_pedina(p->next,testa);
        else
        {
            p->next = testa;
        }
    }
}

Pedina head(Pedina p)
{
    if(p)
    {
        if(p->next)
        {
            return head(p->next);
        }
        else return p;
    }
    return NULL;
}

Pedina pop(Pedina p)
{
    Pedina tmp;
    while(p && p->next)
    {
        p = p->next;
    }
    tmp = p->next;
    p->next = NULL;
    return tmp;
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
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if(board->vet[i][j])
            {
                if(head(board->vet[i][j])->colore == BLUE)
                {
                    if(contastack(board->vet[i][j])>1)
                        printf("B ");
                    else printf("b ");
                }
                if((board->vet[i][j])->colore == RED)
                {
                    if(contastack(board->vet[i][j]))
                        printf("r ");
                    else printf("R ");
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
int muovi(Pedina p,int x, int y,Board b)
{
    if(!p) return 0;
    if(x >= 0 && y >= 0 && x<7 && y<7 && !b->vet[x][y])
    {
        Pedina tmp;
        tmp = b->vet[p->coordx][p->coordy];

        b->vet[p->coordx][p->coordy] = b->vet[x][y];
        b->vet[x][y] = tmp;

        p->coordx = x;
        p->coordy = y;

        return 1;
    }
    else return 0;
}

/*
 * muove una pedina secondo le regole di gioco,
 * se non è possibile con gli argomenti proposti restituisce 0;
 */

int muovi_legale_wrapper(Pedina p,int x, int y,Board b)
{
    if((x == p->coordx + 1 || x == p->coordx - 1) && (y == p->coordy +1 || y == p->coordy -1))
        if(contastack(p)>1)
            return muovi(p,x,y,b);
        else
        {
            if(p->colore == BLUE)
            {
                if((x == p->coordx + 1) && (y == p->coordy - 1) || (x == p->coordx + 1) && (y == p->coordy + 1))
                    return muovi(p,x,y,b);
                else return 0;
            }
            else
            {
                if((x == p->coordx - 1) && (y == p->coordy - 1) || (x == p->coordx - 1) && (y == p->coordy + 1))
                    return muovi(p,x,y,b);
                else return 0;
            }
        }
    else return 0;
}

/*
 * TODO: far si che la pedina "salga" sull'altra dopo aver mangiato
 */

void hop(Pedina mangiato, Pedina mangiante,Board b)
{
    aggiungi_pedina(mangiante,pop(mangiato));
    b->vet[mangiato->coordx][mangiato->coordy] = NULL;
}

int mangia(Pedina p, int x, int y, Board b)
{
    if((x == p->coordx + 2 || x == p->coordx - 2) && (y == p->coordy + 2 || y == p->coordy - 2))
    {
        int xmangiato, ymangiato;
        Pedina mangiato;

        xmangiato = (p->coordx + x) / 2;
        ymangiato = (p->coordy + y) / 2;
        mangiato = b->vet[xmangiato][ymangiato];

        if(mangiato && head(mangiato)->colore != p->colore)
        {
            hop(mangiato,p,b);
            mangiato->coordy = y;
            mangiato->coordx = x;
            return muovi(p,x,y,b);
        }
        else return 0;
    }
    return 0;
}

int main() {
    Board b = init_board();
    print_board(b);
    printf("\n%d",muovi_legale_wrapper(b->vet[2][4],3,3,b));
    printf("\n");
    print_board(b);
    printf("\n%d",mangia(b->vet[4][2],2,4,b));
    printf("\n");
    print_board(b);
    //printf("%d",contastack(*b->vet[0][0]));/*
}
