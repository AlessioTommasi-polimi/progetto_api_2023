#define INITIAL_STATION 25
#define INITIAL_AUTO 10 //COMUNQUE MAX SONO 512

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
}

void add_auto( parco_veicoli *p, veicolo v)
{
    if (p->actual_size == 512)
    {
        ErrorAddAuto();
        return;
    }
    if (p->actual_size == p->actual_capacity)
    {
        p->actual_capacity += INITIAL_AUTO;
        p->actual_capacity > 512 ? p->actual_capacity = 512 : p->actual_capacity;
        p->veicoli = (struct veicolo *)realloc(p->veicoli, p->actual_capacity * sizeof(struct veicolo));
    }
    p->veicoli[p->actual_size] = v;
    p->actual_size++;
}

void add_station(stazione s, int initial_auto)
{
    if (highway.actual_size == highway.actual_capacity)
    {
        highway.actual_capacity += INITIAL_STATION;
        highway.stazioni = (struct stazione *)realloc(highway.stazioni, highway.actual_capacity * sizeof(struct stazione));
    }
    highway.stazioni[highway.actual_size] = s;
    highway.actual_size++;

    // siccome ho numero di veicoli quando faccio un add station lo faccio li!
    init_parco(&highway.stazioni[highway.actual_size-1].parco, initial_auto);
}
