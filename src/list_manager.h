#define INITIAL_STATION 50
#define INITIAL_AUTO 200 //COMUNQUE MAX SONO 512

void free_autostrada()
{
    //libero i parchi
    //free_parck();
    for (size_t i = 0; i < highway.actual_size; i++)
    {
        free_parck(&highway.stazioni[i].parco);
    }
    free(highway.stazioni);
    highway.stazioni = NULL;
    highway.actual_size = 0;
    highway.actual_capacity = 0;
}

void free_viaggio(viaggio *v)
{
    free(v->tappa);
    v->num_tappe = -2;
}

void free_parck(struct parco_veicoli* p)
{
    free(p->veicoli);
    p->veicoli = NULL;
    p->actual_size = 0;
    p->actual_capacity = 0;
}



void init_autostrada()
{
    highway.stazioni = (struct stazione*)malloc(INITIAL_STATION * sizeof(struct stazione));
    highway.actual_size = 0;
    highway.actual_capacity = INITIAL_STATION;
}

// per ogni stazione inizializzo un parco con al max initial_auto+INITIAL_AUTO per evitare di far subito realloc
// con tot 25 auto per stazione occupazione ~ 50MB
void init_parco(parco_veicoli *p,int initial_auto)
{
    p->veicoli = (struct veicolo *)malloc((initial_auto+INITIAL_AUTO) * sizeof(struct veicolo));
    p->actual_size = 0;// aggiorno a inizial_auto quando effettivamente faccio la add!
    p->actual_capacity = initial_auto + INITIAL_AUTO;
    p->curr_max = get_max_auto(p);

}

void init_viaggio(viaggio *v, int max_dim,veicolo macchina){
    v->tappa = (struct stazione *)malloc(max_dim * sizeof(struct stazione));
    v->num_tappe = -1; // inizializzo a -1 caso in cui non trovo nessun percorso
}

//SETTER & MODIFICATORI

//non conviene ternerla ordinata, basta che mi salvo il massimo
//ORDINAMENTO PER AUTONOMIA: autonomia crescente
int add_auto_order( parco_veicoli *p, veicolo v)
{
    if (p->actual_size == 512)
    {
        return -1;
    }
    if (p->actual_size == p->actual_capacity)
    {
        p->actual_capacity += INITIAL_AUTO;
        p->actual_capacity > 512 ? p->actual_capacity = 512 : p->actual_capacity;
        p->veicoli = (struct veicolo *)realloc(p->veicoli, p->actual_capacity * sizeof(struct veicolo));
    }

    int index = 0;

    while (index < p->actual_size && p->veicoli[index].autonomia < v.autonomia)
    {
        index++;
    }
    v.index = index;
    // shift
    for (int i = p->actual_size; i > index; i--)
    {
        p->veicoli[i] = p->veicoli[i - 1];
    }

    p->veicoli[index] = v;
    p->curr_max = p->veicoli[p->actual_size];
    p->actual_size++;

    return 0;
}

int add_auto(parco_veicoli *p, veicolo v){
    if (p->actual_size == 512)
    {
        return -1;
    }
    if (p->actual_size == p->actual_capacity)
    {
        p->actual_capacity += INITIAL_AUTO;
        p->actual_capacity > 512 ? p->actual_capacity = 512 : p->actual_capacity;
        p->veicoli = (struct veicolo *)realloc(p->veicoli, p->actual_capacity * sizeof(struct veicolo));
    }
    v.index = p->actual_size;
    p->veicoli[p->actual_size] = v;
    p->curr_max = p->actual_size == 0 ? v : v.autonomia > p->curr_max.autonomia ? v : p->curr_max;

    p->actual_size++;

    return 0;
}

int rottama_auto(parco_veicoli *p, veicolo v){
    int index=-1;

    for (index = 0; index < p->actual_size; index++)
    {
        if (p->veicoli[index].autonomia == v.autonomia)
        {
            break;
        }
    }

    if (index == -1 || index == p->actual_size)
    {
        return -1;
    }
    
    for (int i = index; i < p->actual_size - 1; i++)
    {
        p->veicoli[i] = p->veicoli[i + 1];
        p->veicoli[i].index--;
    }
    p->actual_size--;

    //aggiorno il curr_max
    p->curr_max = get_max_auto(p);

    return 0;
}


//IN REALTA POTREI OTTIMIZZARE ORDINANDOLA SOLO ALLA FINE!
//ordina in base alla distanza crescente!
//ritorna indice di dove ho aggiunto la NUOVA stazione o -1 se non ho aggiunto
int add_station(stazione s, int initial_auto)
{
    if (highway.actual_size == highway.actual_capacity)
    {
        highway.actual_capacity += INITIAL_STATION;
        highway.stazioni = (struct stazione *)realloc(highway.stazioni, highway.actual_capacity * sizeof(struct stazione));
    }

    
    int index = 0;

    while (index < highway.actual_size && highway.stazioni[index].distanza_da_inizio_autostrada <= s.distanza_da_inizio_autostrada)
    {
        if (highway.stazioni[index].distanza_da_inizio_autostrada == s.distanza_da_inizio_autostrada)
        {
            return -1;
        }
        
        index++;
    }
    
    // shift
    for (int i = highway.actual_size; i > index; i--)
    {
        highway.stazioni[i] = highway.stazioni[i - 1];
        highway.stazioni[i].index =i;
    }

    s.index = index;
    highway.actual_size++;
    highway.stazioni[index] = s;

    // siccome ho numero di veicoli quando faccio un add station lo faccio li!
    init_parco(&highway.stazioni[index].parco, initial_auto);

    return index;
}

int demolisci_stazione(int distanza)
{
    int index = -1;
    for (size_t i = 0; i < highway.actual_size; i++)
    {
        if (highway.stazioni[i].distanza_da_inizio_autostrada == distanza)
        {
            index = i;
            //.DEBUG
            printf("demolisco stazione %d   con indice: %d\n", distanza, index);
            break;
        }
    }

    if (index == -1)
    {
        return -1;
    }

    //libero memoria
    free_parck(&highway.stazioni[index].parco);

    for (int i = index; i < highway.actual_size; i++)
    {
        highway.stazioni[i] = highway.stazioni[i+1];
    }
    highway.actual_size--;

    return 0;
}

// Funzione per calcolare il piano di viaggio
// ATTENZIONE TAPPE E'RITORNATO IN ORDINE INVERSO!!
// la prima tappa da stampare e'quella in posizione v->num_tappe-1
void calculate_plan(viaggio *v, int index_partenza, int index_arrivo)
{
    int curr_index = index_arrivo; // Inizialmente si parte dalla stazione di partenza
    int num_tappe = 0;
    
    curr_index = get_best_station(index_partenza, curr_index);
    while (curr_index > index_partenza)
    {
        //.DEBUG
        //printf("curr_index: %d\n", curr_index);
        if (curr_index == index_arrivo)
        {
            // Non è stato possibile trovare un percorso
            v->num_tappe = -1;
            return;
        }
        v->tappa[num_tappe] = highway.stazioni[curr_index];
        num_tappe++;
        curr_index = get_best_station(index_partenza, curr_index);
    }
    v->num_tappe = num_tappe;
    //.DEBUG
    //printf("Numero di tappe: %d\n", num_tappe);
}

void calculate_plan_reverse(viaggio *v, int index_partenza, int index_arrivo)
{
    int ap = index_partenza;
    int next_max = -1;

    index_partenza = index_arrivo;
    index_arrivo = ap;

    int curr_index = index_partenza;
    int num_tappe = 0;

    stazione best_curr_station = highway.stazioni[index_arrivo];

    int max_index_curr = get_index_max_raggiungible_station_desh(best_curr_station.index, best_curr_station.parco.curr_max.autonomia);

    /*devo escludere stazione iniziale dall elenco delle tappe: */
    if (max_index_curr <= index_partenza)
    {
        v->num_tappe = 0;
        return;
    }
    
    while (curr_index > index_partenza)
    {
        next_max = max_index_curr;
        for (size_t i = curr_index; i >= max_index_curr; i--)
        {
            ap = get_index_max_raggiungible_station_desh(i, highway.stazioni[i].parco.curr_max.autonomia);
            /*con la stazione i riesco ad arrivare piu lontano*/
            if (ap >= get_index_max_raggiungible_station_desh(best_curr_station.index, best_curr_station.parco.curr_max.autonomia) || (ap <= index_partenza))
            {
                best_curr_station = highway.stazioni[i];
                next_max = ap;
            }
            
        }

        if ((best_curr_station.index == curr_index /*non ce nessuna stazione che riesce a portarmi piu lontano di quella corrente*/) && (/*non riesco a raggiungere arrivo*/get_index_max_raggiungible_station_desh(best_curr_station.index, best_curr_station.parco.curr_max.autonomia) > index_partenza ))
        {
            // Non è stato possibile trovare un percorso
            v->num_tappe = -1;
            return;
        }

        v->tappa[num_tappe] = best_curr_station;
        num_tappe++;

        max_index_curr = next_max;/*equivalente della riga sotto ma ottimizzato*/
        //max_index_curr = get_index_max_raggiungible_station_desh(best_curr_station.index, best_curr_station.parco.curr_max.autonomia);

        curr_index = best_curr_station.index;
    }
    v->num_tappe = num_tappe;
    //.DEBUG
    // printf("Numero di tappe: %d\n", num_tappe);
}
//GETTERS
parco_veicoli *get_parco(int distanza)
{
    for (size_t i = 0; i < highway.actual_size; i++)
    {
        if (highway.stazioni[i].distanza_da_inizio_autostrada == distanza)
        {
            return &highway.stazioni[i].parco;
        }
    }
    return NULL;
}

veicolo get_max_auto(parco_veicoli *p){
    //ogni volta che lo chiamo setto anche il curr max nel parco!
    //caso in cui non ci sono auto!
    veicolo v2;
    v2.autonomia = -1;
    v2.index = -1;
    p->curr_max = p->actual_size == 0 ? v2 : p->veicoli[0];

    if (p->curr_max.index == -1)
    {//ottimizzazione
        return p->curr_max;
    }

    // DEVO FARE COosi perche non ho il vettore ordinato!
    for (int i = 1; i < p->actual_size ; i++)
    {
        if (p->veicoli[i].autonomia > p->curr_max.autonomia)
        {
            p->curr_max = p->veicoli[i];
        }
    }

    return p->curr_max;
}

//ritorna indice della stazione se c'è una stazione alla distanza passata altrimenti -1
int get_index_station(int distanza)
{
    for (size_t i = 0; i < highway.actual_size; i++)
    {
        if (highway.stazioni[i].distanza_da_inizio_autostrada == distanza)
        {
            return i;
        }
    }
    return -1;
}

// da richiamare <=> index_partenza < index_arrivo
// ritorna indice della stazione raggiungibile altrimenti index_partenza se non ne posso raggiungere
int get_index_max_raggiungible_station_cres(int index_partenza,int curr_autonomia/*distanza_partenza + autonomia macchina*/)
{
    for (size_t i = index_partenza; i < highway.actual_size; i++)
    {
        if (highway.stazioni[i].distanza_da_inizio_autostrada - highway.stazioni[index_partenza].distanza_da_inizio_autostrada > curr_autonomia)
        {
            return i-1;
        }
    }
    return highway.actual_size-1;
}

// da richiamare <=> index_partenza > index_arrivo
// ritorna indice della stazione raggiungibile altrimenti index_partenza se non ne posso raggiungere
int get_index_max_raggiungible_station_desh(int index_partenza, int curr_autonomia /*distanza_partenza + autonomia macchina*/)
{
    for (size_t i = index_partenza; i >=0; i--)
    {
        if (highway.stazioni[index_partenza].distanza_da_inizio_autostrada - highway.stazioni[i].distanza_da_inizio_autostrada > curr_autonomia)
        {
            return i + 1;
        }
    }
    return 0;
}

// ritrona indice della stazione con autonomia migliore
// da richiamare <=> index_partenza < index_arrivo
// ritorna index_arrivo se la stazione di arrivo non e' raggiungibile da nessuna stazione da index_partenza
int get_best_station(int index_partenza, int index_arrivo){
    //.DEBUG
    //printf("index_partenza: %d, index_arrivo: %d\n", index_partenza, index_arrivo);

    stazione current_station = highway.stazioni[index_arrivo];
    for (int i = index_arrivo-1; i >= index_partenza; i--)
    {
        //.DEBUG
        //printf("\n current_station: %d\n, i: %d\n", current_station.index, i);

        /*mi permette di arrivare a stazione finale ed e'la piu piccola possibile*/
        if (highway.stazioni[i].parco.curr_max.autonomia + highway.stazioni[i].distanza_da_inizio_autostrada >= highway.stazioni[index_arrivo].distanza_da_inizio_autostrada)
        {
            //.DEBUG
            //printf("\n urr_max.autonomia: %d\n, stazioni[i].distanza_da_inizio_autostrada: %d\n, stazioni[index_arrivo].distanza_da_inizio_autostrada: %d", highway.stazioni[i].parco.curr_max.autonomia, highway.stazioni[i].distanza_da_inizio_autostrada, highway.stazioni[index_arrivo].distanza_da_inizio_autostrada);
            current_station = highway.stazioni[i];
        }
        
    }
    //.DEBUG
    //printf("\n end currwnt: %d\n", current_station.index);
    return current_station.index;
}

// ritrona indice della stazione con autonomia migliore
// da richiamare <=> index_partenza > index_arrivo
// ritorna index_arrivo se la stazione di arrivo non e' raggiungibile da nessuna stazione da index_partenza
/*NOPE*/
int get_best_station_reverse(int index_partenza, int index_arrivo)
{

    stazione current_station = highway.stazioni[index_arrivo];

    //.DEBUG
    // printf("index_partenza: %d, index_arrivo: %d\n", index_partenza, index_arrivo);
    int ap = index_partenza;
    index_partenza = index_arrivo;
    index_arrivo = ap;

    for (int i = index_arrivo; i >= index_partenza; i--)
    {
        //.DEBUG
        // printf("\n current_station: %d\n, i: %d\n", current_station.index, i);

        /*mi permette di arrivare a stazione finale ed e'la piu piccola possibile*/
        if (highway.stazioni[i].distanza_da_inizio_autostrada - highway.stazioni[i].parco.curr_max.autonomia <= highway.stazioni[index_partenza].distanza_da_inizio_autostrada)
        {
            //.DEBUG
            // printf("\n urr_max.autonomia: %d\n, stazioni[i].distanza_da_inizio_autostrada: %d\n, stazioni[index_arrivo].distanza_da_inizio_autostrada: %d", highway.stazioni[i].parco.curr_max.autonomia, highway.stazioni[i].distanza_da_inizio_autostrada, highway.stazioni[index_arrivo].distanza_da_inizio_autostrada);
            current_station = highway.stazioni[i];
        }
    }
    //.DEBUG
    // printf("\n end currwnt: %d\n", current_station.index);
    return current_station.index;
}
