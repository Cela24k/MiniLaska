//
// Created by alece on 08/02/2021.
//

#ifndef UNTITLED1_PEDINA_H
#define UNTITLED1_PEDINA_H
enum stato {PEDINA,GENERALE};
enum giocatore{BLUE,RED};

typedef struct pedina
{
    int coordx;
    int coordy;
    enum giocatore colore; /* colore della pedina in testa*/
    enum stato stato; /*stato della pedina, GENERALE/NORMALE*/
    struct pedina *next;
} *Pedina_list;

Pedina_list init_pedina(int x, int y, enum giocatore colore,enum stato s)
{
    Pedina_list p;
    p = malloc(sizeof(struct pedina));
    if(p)
    {
        p->coordx = x;
        p->coordy = y;
        p->colore = colore;
        p->stato = s;
        p->next = NULL;

        return p;
    }
    return NULL;
}

void delete_pedina(Pedina_list *p)
{
    if(*p)
    {
        Pedina_list *tmp;
        if((*p)->next) delete_pedina(&(*p)->next);
        tmp = p;
        free(*tmp);
        *p = NULL;
    }
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

int append(Pedina_list *p,struct pedina p1)
{
    Pedina_list tmp;
    Pedina_list scorri;
    tmp = (Pedina_list)malloc(sizeof(struct pedina));
    if(tmp)
    {
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
            *p = tmp;
        }
        return 1;
    }
    return 0;
}

#endif //UNTITLED1_PEDINA_H
