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
    enum giocatore colore;  /* colore della pedina in testa*/
    enum stato stato;       /*stato della pedina, GENERALE/NORMALE*/
    struct pedina *next;
} *Pedina_list;

/*
 * Inizializza in memoria una pedina con determinate coordinate, un colore e uno stato,
 * dopodichè restituisce il suo riferimento.
 * Se la malloc non va a buon fine, restituisce NULL;
 */
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

/*
 * Questa funzione viene chiamata specularmente alla allocazione in memoria della pedina,
 * libera la pedina e le sue successive dalla memoria.
 */
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

// Utility, data una Pedina_list restituisce quante pedine si susseguono in quel determinato stack.
int contastack(Pedina_list pedina)
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
// Data una Pedina_list*, rimuove la testa e la libera dalla memoria.
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

// Data una Pedina_list*, rimuove la coda e la libera dalla memoria
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
// Data una Pedina_list* e l'elemento da inserire,
// effettua l'append di tale elemento alla lista.
// restituisce 1 se è andato a buon fine 0 se non va a buon fine.
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
