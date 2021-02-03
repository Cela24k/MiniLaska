#include <stdio.h>
#include <stdlib.h>

enum color{BLUE,RED,NONE};

typedef struct pedina
{
    int coordx;
    int coordy;
    enum color colore;
} *Pedina;

typedef struct board{
    Pedina* vet[7][7];
} *Board;

Pedina* init_pedina(int x, int y, enum color colore)
{
    Pedina *p;
    p = (Pedina*)malloc(sizeof(Pedina)*3);
    p[0] = malloc(sizeof(struct pedina));
    p[1] = malloc(sizeof(struct pedina));
    p[2] = malloc(sizeof(struct pedina));

    p[0]->colore = colore;
    p[0]->coordy = y;
    p[0]->coordx = x;

    p[1]->colore = NONE;
    p[1]->coordy = y;
    p[1]->coordx = x;

    p[2]->colore = NONE;
    p[2]->coordy = y;
    p[2]->coordx = x;

    return p;
}

Board init_board()
{
    Board b = malloc(sizeof(struct board));

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            b->vet[i][j] = init_pedina(i,j,NONE);
            if((i+j)%2 == 0)
            {
                if(i<3) b->vet[i][j] = init_pedina(i,j,BLUE);
                if(i == 3) b->vet[i][j] = init_pedina(i,j,NONE);
                if(i>3) b->vet[i][j] = init_pedina(i,j,RED);
            }
        }
    }
    return b;
}

int contastack(Pedina* pedina) // utility, restituisce quante pedine ha un determinato stack di pedine
{
    int c;
    c=0;
    for (int i = 0; i < 3; ++i) {
        if(pedina[i]->colore != NONE)
        {
            c++;
        }
    }
    return c;
}

void print_board(Board board)
{
    if(!board) return;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if((*(board->vet[i][j]))->colore == BLUE)
            {
                if(contastack((board->vet[i][j]))>1)
                    printf("B ");
                else printf("b ");
            }
            if((*(board->vet[i][j]))->colore == NONE)
            {
                printf("%c ",219);
            }
            if((*(board->vet[i][j]))->colore == RED)
            {
                if(contastack((board->vet[i][j])))
                    printf("r ");
                else printf("R ");
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
    if(x<7 && y<7 && (*(b->vet[x][y]))->colore == NONE)
    {
        Pedina *tmp;
        tmp = b->vet[p->coordx][p->coordy];
        b->vet[p->coordx][p->coordy] = b->vet[x][y];
        b->vet[x][y] = tmp;

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
