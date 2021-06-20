#ifndef UNTITLED1_PEDINA_H
#define UNTITLED1_PEDINA_H
enum stato {PEDINA,GENERALE}; /** stato della pedina, PEDINA può muovere solo in avanti, GENERALE anche indietro e diventa tale dopo che ha raggiunto la fine del campo*/
enum giocatore{BLUE,RED}; /** colore della pedina */

/**
 * Pedina_list è la struttura di base che contiene una pedina con stato, colore,
 * coordinate e un riferimento alla successiva
 */
typedef struct pedina
{
    int coordx;
    int coordy;
    enum giocatore colore;  /** colore della pedina in testa */
    enum stato stato;       /** stato della pedina, GENERALE/NORMALE */
    struct pedina *next;
} *Pedina_list;

/**
 * Inizializza in memoria una pedina con determinate coordinate, un colore e uno stato,
 * dopodichè restituisce il suo riferimento.
 * Se la malloc non va a buon fine, restituisce NULL;
 * @param x la coordinata x della pedina
 * @param y la coordinata y
 * @param colore il colore della pedina
 * @param s lo stato della pedina
 * @return Pedina_list inizializzata in memoria
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

/**
 * Questa funzione viene chiamata specularmente alla allocazione in memoria della pedina,
 * libera la pedina e le sue successive dalla memoria.
 * @param p il puntatore alla pedina da eliminare
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

/**
 * Utility, data una Pedina_list restituisce quante pedine si susseguono in quel determinato stack.
 * @param pedina la pedina in testa di cui contare lo "stack"
 * @return un int corrispondente al numero di pedine stackate una sull'altra
 * */
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
/**
 * Data una Pedina_list*, rimuove la testa e la libera dalla memoria.
 * @param p il puntatore alla pedina da cui voglio rimuovere la testa
 * */
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

/**
 * Data una Pedina_list*, rimuove la coda e la libera dalla memoria*
 * @param p il puntatore alla pedina da cui rimuovere la coda
 */
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
/**
 * Data una Pedina_list* e l'elemento da inserire,
 * effettua l'append di tale elemento alla lista.
 * restituisce 1 se è andato a buon fine 0 se non va a buon fine.
 * @param p il puntatore alla pedina in cui fare l'append
 * @param p1 la struct pedina da aggiungere alla lista
 * @return 1 se la malloc e append vanno a buon fine 0 altrimenti
 */
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
