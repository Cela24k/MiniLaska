#include <stdio.h>
#include <stdlib.h>

enum color{BLUE,RED};

typedef struct pedina
{
    int coordx;
    int coordy;
    enum color colore;
    struct pedina *next;
} *Pedina_list;

typedef struct board{
    Pedina_list vet[7][7];
} *Board;

Pedina_list init_pedina(int x, int y, enum color colore)
{
    Pedina_list p;
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
    tmp = (Pedina_list)malloc(sizeof(struct pedina));
    tmp->coordx = p1.coordx;
    tmp->coordy = p1.coordy;
    tmp->colore = p1.colore;
    tmp->next = NULL;

    if(*p)
    {
        while((*p)->next)
        {
            *p = (*p)->next;
        }
        (*p)->next = tmp;
    }
    else
    {
        *p = tmp;
    }
}

void elimina(Pedina_list *p)
{
    if(*p)
    {
        Pedina_list *tmp;
        tmp = p;
        *p = (*p)->next;
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
    for (int i = 0; i < 7; ++i) {
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
int mossa_legale(Pedina_list p,int x, int y,Board b)
{
    if((x == p->coordx + 1 || x == p->coordx - 1) && (y <= p->coordy +1 || y == p->coordy -1))
        if(contastack(p)>1)
            return 1;
        else
        {
            if(p->colore == BLUE)
            {
                if((x == p->coordx + 1) && (y == p->coordy - 1) || (x == p->coordx + 1) && (y == p->coordy + 1))
                    return 1;
                else return 0;
            }
            else
            {
                if((x == p->coordx - 1) && (y == p->coordy - 1) || (x == p->coordx - 1) && (y == p->coordy + 1))
                    return 1;
                else return 0;
            }
        }
    else return 0;
}
int mangia_legale(Pedina_list p,int x, int y,Board b)
{
    if((x == p->coordx + 2 || x == p->coordx - 2) && (y <= p->coordy +2 || y == p->coordy -2))
        if(contastack(p)>1)
            return 1;
        else
        {
            if(p->colore == BLUE)
            {
                if((x == p->coordx + 2) && (y == p->coordy - 2) || (x == p->coordx + 2) && (y == p->coordy + 2))
                    return 1;
                else return 0;
            }
            else
            {
                if((x == p->coordx - 2) && (y == p->coordy - 2) || (x == p->coordx - 2) && (y == p->coordy + 2))
                    return 1;
                else return 0;
            }
        }
    else return 0;
}
int muovi_legale_wrapper(Pedina_list p,int x, int y,Board b)
{
    if(mossa_legale(p,x,y,b)||mangia_legale(p,x,y,b))
        return muovi(p,x,y,b);
    else return 0;
}


int mangia(Pedina_list p, int x, int y, Board b)
{
    if((x == p->coordx + 2 || x == p->coordx - 2) && (y == p->coordy + 2 || y == p->coordy - 2))
    {
        int xmangiato, ymangiato;
        Pedina_list mangiato;

        xmangiato = (p->coordx + x) / 2;
        ymangiato = (p->coordy + y) / 2;
        mangiato = b->vet[xmangiato][ymangiato];

        if(mangiato && mangiato->colore != p->colore)
        {
            append(&b->vet[p->coordx][p->coordy],*b->vet[xmangiato][ymangiato]);
            elimina(&b->vet[xmangiato][ymangiato]);
            return muovi_legale_wrapper(p,x,y,b);
        }
        else return 0;
    }
    return 0;
}

int main() {
    Board b = init_board();
    print_board(b);
    printf("\n");
    printf("\n%d",muovi_legale_wrapper(b->vet[2][4],3,3,b));
    printf("\n");
    print_board(b);
    printf("\n%d",mangia(b->vet[4][2],2,4,b));
    printf("\n");
    print_board(b);
    printf("\n");
    //printf("%d",contastack())
}
