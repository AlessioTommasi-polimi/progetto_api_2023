#define INITIAL_STATION 50
#define INITIAL_AUTO 200 //COMUNQUE MAX SONO 512
//git pls
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
    v->current_machine = macchina;//perfetto cosi assegno una copia e non rischio di modificare!
    v->current_machine.index = -1;//non serve a nulla sapere index del parco di questa macchina cosi li inizializzo a -1 per non fare errori concettuali!
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
void calculate_plan(viaggio *v, int index_partenza, int index_arrivo)
{
    int curr_index = index_partenza; // Inizialmente si parte dalla stazione di partenza
    int num_tappe = 0;
    int max_ragg_index = -1;

    // Calcolo del numero minimo di tappe
    while ((curr_index < index_arrivo) && (max_ragg_index < index_arrivo))
    {
        max_ragg_index = get_index_max_raggiungible_station_cres(curr_index, v->current_machine.autonomia);
        
        // Controllo se conviene fermarsi in una stazione precedente per cambiare veicolo
        int best_index = -1;
        int best_autonomia = v->current_machine.autonomia;

        for (int j = curr_index; j > index_partenza; j--)
        {
            if (highway.stazioni[j].parco.curr_max.autonomia >= best_autonomia)
            {
                best_index = j;
                best_autonomia = highway.stazioni[j].parco.curr_max.autonomia;
            }
        }

        if (best_index != -1 && (max_ragg_index == curr_index   || 
            /*autonomia che ho col veicolo in posizione best_index basta ad arrivare a destinazione*/
            best_autonomia > (highway.stazioni[max_ragg_index].distanza_da_inizio_autostrada - highway.stazioni[curr_index].distanza_da_inizio_autostrada)))
        {
            // Fermarsi in una stazione precedente per cambiare veicolo
            curr_index = best_index;
            v->current_machine = highway.stazioni[best_index].parco.curr_max;
            v->tappa[num_tappe] = highway.stazioni[best_index];
            num_tappe++;
            printf("Tappa %d: Dist stazione:%d  best index: %d \n", num_tappe, highway.stazioni[curr_index].distanza_da_inizio_autostrada, best_index);
        }
        else
        {
            // Continuare al prossimo index raggiungibile
            if (max_ragg_index == curr_index) // Non è possibile raggiungere la stazione successiva!
            {
                //.DEBUG
                printf("Non è possibile raggiungere la stazione di arrivo.\n");
                return;
            }
            curr_index = max_ragg_index;
            //.DEBUG
        }
        //.DEBUG
        printf("\n max_ragg_index: %d\n curr_index: %d \n curr_autonomia: %d\n", max_ragg_index, curr_index, v->current_machine.autonomia);
    }
    v->num_tappe = num_tappe;
    //.DEBUG
    printf("Numero di tappe: %d\n", num_tappe);
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