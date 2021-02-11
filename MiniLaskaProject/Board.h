//
// Created by alece on 08/02/2021.
//

#ifndef UNTITLED1_BOARD_H
#define UNTITLED1_BOARD_H

typedef struct board {
    Pedina_list vet[7][7];
} *Board;

Board init_board()
{
    Board b = malloc(sizeof(struct board));

    if(b)
    {
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
    return NULL;
}

Board init_empty_board()
{
    Board b = malloc(sizeof(struct board));

    if(b)
    {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                b->vet[i][j] = NULL;
            }
        }
        return b;
    }
    return NULL;
}

void delete_board(Board *b)
{
    if(*b)
    {
        Board *tmp;
        tmp = b;
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                delete_pedina(&(*b)->vet[i][j]);
            }
        }
        free(*tmp);
        *b = NULL;
    }
}

Board clone_board(Board b)
{
    Board tmp;
    tmp = init_empty_board();

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
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

void print_row_occ(Board b,int r)
{
    printf("x    ");
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
                    if(board->vet[i][j]->stato==GENERALE)
                        printf("B ");
                    else printf("b ");
                }
                if(board->vet[i][j]->colore == RED)
                {
                    if(board->vet[i][j]->stato==GENERALE)
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
    printf("  y y y y y y y \n");
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

int muovi_legale_wrapper(Pedina_list p,int x, int y,Board b)
{
    if(mossa_legale(p,x,y,b))
        return muovi(p,x,y,b);
    else if(mangia_legale(p,x,y,b))
        return mangia(p,x,y,b);
    else return 0;
}

int has_all_pieces(enum giocatore player, Board b)
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

int entro_limiti(int x, int y)
{
    return !(x<0 || x>6 || y<0 || y>6);
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

int player_has_moves(enum giocatore player, Board b)
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

enum giocatore winner(Board board,enum giocatore player1,enum giocatore player2)
{
    if(!player_has_moves(player2,board) || has_all_pieces(player1,board))
        return player1;
    if(!player_has_moves(player1,board) || has_all_pieces(player2,board))
        return player2;
    return -1;
}

#endif //UNTITLED1_BOARD_H
