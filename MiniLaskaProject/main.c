#include <stdio.h>
#include <stdlib.h>

enum color{BLUE,RED,NONE};

typedef struct pedina
{
    int coordx;
    int coordy;
    enum color colore;
    struct pedina *next;
} *Pedina;

typedef struct board{
    Pedina* vet[7][7];
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
            b->vet[i][j] = malloc(sizeof(Pedina*));
            *b->vet[i][j] = NULL;
            if((i+j)%2 == 0)
            {
                if(i<3) *b->vet[i][j] = init_pedina(i,j,BLUE);
                if(i == 3) *b->vet[i][j] = NULL;
                if(i>3) *b->vet[i][j] = init_pedina(i,j,RED);
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

void print_board(Board board)
{
    if(!board) return;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if(*board->vet[i][j])
            {
                if((*board->vet[i][j])->colore == BLUE)
                {
                    if(contastack((*board->vet[i][j]))>1)
                        printf("B ");
                    else printf("b ");
                }
                if((*board->vet[i][j])->colore == RED)
                {
                    if(contastack(*(board->vet[i][j])))
                        printf("r ");
                    else printf("R ");
                }
            }
            else
            {
                printf("%c ",219);
            }
        }
        printf("\n");
    }
}

/* muove una pedina, ATTENZIONE, NON controlla se la mossa è valida secondo le regole di gioco,
 * ma solo se la mossa è limitata al campo.
 */

int muovi(Pedina p,int x, int y,Board b)
{
    if(!p) return 0;
    if(x<7 && y<7 && !*b->vet[x][y])
    {
        Pedina tmp;
        tmp = *b->vet[p->coordx][p->coordy];

        *b->vet[p->coordx][p->coordy] = *b->vet[x][y];
        *b->vet[x][y] = tmp;

        p->coordx = x;
        p->coordy = y;

        return 1;
    }
    else return 0;
}

/* muove una pedina secondo le regole di gioco,
 * se non è possibile con gli argomenti proposti restituisce 0;
 */

int muovi_legale_wrapper(Pedina p,int x, int y,Board b)
{
    if((x == p->coordx + 1 || x == p->coordx - 1) && (y == p->coordy +1 || y == p->coordy -1))
        return muovi(p,x,y,b);
    else return 0;
}

/* TODO: far si che la pedina "salga" sull'altra dopo aver mangiato
 *
 */

int mangia_legale(Pedina p, int x, int y, Board b)
{
    if((x == p->coordx + 2 || x == p->coordx - 2) && (y == p->coordy + 2 || y == p->coordy - 2))
    {
        int xmangiato, ymangiato;
        xmangiato = (p->coordx + x) / 2;
        ymangiato = (p->coordy + y) / 2;

        if((*(b->vet[xmangiato][ymangiato]))->colore != NONE && (*(b->vet[xmangiato][ymangiato]))->colore != p->colore)
        {

            return muovi(p,x,y,b);
        }
        else return 0;
    }
    return 0;
}

int main() {
    Board b = init_board();
    print_board(b);
    printf("\n%d",muovi_legale_wrapper(*(b->vet[2][4]),3,3,b));
    printf("\n");
    print_board(b);
}
