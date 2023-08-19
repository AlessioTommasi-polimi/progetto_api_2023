#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

typedef struct veicolo
{
    int index; // indice del veicolo nella lista di veicoli della stazione
    int autonomia; // peso BST // espressa in km
} veicolo;

typedef struct parco_veicoli
{
    int actual_size;
    int actual_capacity;

    veicolo curr_max;
    veicolo *veicoli; // max 512 veicoli in una singola stazione
} parco_veicoli;

typedef struct stazione
{
    //se eliminata distanza da inizio autostrada diventa -1  && index = -1
    int distanza_da_inizio_autostrada; // i veicolo possono raggiungere la stazione solo se sono autonomia > distanza_da_inizio_autostrada
    int index;                        // indice della stazione indica indice in cui e posizionara stazione rispetto tutte le altre dell autostrada!
    // serve per ottimizzare al massimo il fatto che non si possa tornare indieto!
    parco_veicoli parco;
} stazione;

typedef struct autostrada
{
    stazione *stazioni; // lista inizialmente con 100 stazioni poi se sforo ne aggiungo altre 100 dinamicamente ogni volta
    int actual_size;
    int actual_capacity;
} autostrada;//si suppone che per file si descriva una sola autosrada

typedef struct viaggio
{
    int num_tappe;//actual size;
    //actualcapacity viene automaticamente inizializzata come sem i fermassi in tutte le stazioni da partenza ad arrivo!
    // si assume che stazione[0] sia la stazione di partenza e stazione[max_dim] la stazione di arrivo
    stazione *tappa;
} viaggio;

/*main*/
void setVar();

void controller(char *command);

void addStation();

void demolisciStazione();

void addAuto();

void rottamaAuto();

void plan();

void init();

void free_var();


/*view*/
void printparcoVeicoli(parco_veicoli *parco);

void printVeicolo(veicolo *v);

void printStazione(int distance);

void print_vector(int *v, int size, char *name);

void ErrorAdd();

void SuccessAdd();

void ErrorRottama();

void SuccessRottama();

void ErrorDemolisci();

void SuccessDemolisci();

void ErrorPianifica();

void SuccessPianifica(viaggio *v, int index_partenza, int index_arrivo);

void SuccessPianificaReverse(viaggio *v, int index_partenza, int index_arrivo);


/*end stampe*/

/*list_manager*/
void init_autostrada();

void init_viaggio(viaggio *v, int max_dim, veicolo macchina);

void init_parco(struct parco_veicoli *p, int initial_auto);

int add_auto_order(parco_veicoli *p, veicolo v);

int add_auto(parco_veicoli *p, veicolo v);

int rottama_auto(parco_veicoli *p, veicolo v);

int add_station(stazione s, int initial_auto);

int demolisci_stazione(int distanza);

void free_autostrada();

void free_parck(parco_veicoli *p);

void free_viaggio(viaggio *v);

void calculate_plan(viaggio *v, int index_partenza, int index_arrivo);

void calculate_plan_reverse_dijkstra_from_start(viaggio *v, int index_partenza, int index_arrivo);

void calculate_plan_reverse_dijkstra_from_end(viaggio *v, int index_partenza, int index_arrivo);

void fix_index();

/*getter of list*/
parco_veicoli *get_parco(int distanza);

veicolo  get_max_auto(parco_veicoli *p);

int get_index_station(int distanza);

int get_index_max_raggiungible_station_cres(int index_partenza, int curr_autonomia /*distanza_partenza + autonomia macchina*/);

int get_index_max_raggiungible_station_desh(int index_partenza, int curr_autonomia /*distanza_partenza + autonomia macchina*/);

int get_best_station(int index_partenza, int index_arrivo);

/*UNUSED*/
int get_best_station_reverse(int index_partenza, int index_arrivo);

int get_next_best_station(int index_partenza, int index_arrivo, int min_index);

char buffer_in[50000];
char *argument;
autostrada highway;

char newline[] = "\n";

void printparcoVeicoli(parco_veicoli *parco)
{
    printf("           Parco Veicoli:{\n");
    printf("            \"Size\": %d,\n", parco->actual_size);
    printf("            \"Capacity\": %d,\n", parco->actual_capacity);
    printf("             Veicoli:[\n");
    printf("              curr_max: {\"Autonomia\": %d \"index\": %d},\n", parco->curr_max.autonomia, parco->curr_max.index);
    /*
    for (int i = 0; i < parco->actual_size; i++)
    {
        printf("                 {\n");
        printf("                   \"numero veicolo\": %d,\n", i);//se tutto giusto corrisponde con index!
        printVeicolo(&parco->veicoli[i]);
        printf("                 }%s\n", (i < parco->actual_size - 1) ? "," : "");
    }
     */
    printf("              ]\n");
    printf("             }\n");
}


void printVeicolo (veicolo *v){
    printf("                   \"Autonomia\": %d,\n", v->autonomia);
    printf("\n");
}

void ErrorAdd(){
    printf("non aggiunta\n");
}

void SuccessAdd(){
    printf("aggiunta\n");
}

void ErrorRottama()
{
    printf("non rottamata\n");
}

void SuccessRottama()
{
    printf("rottamata\n");
}

void ErrorDemolisci()
{
    printf("non demolita\n");
}

void SuccessDemolisci()
{
    printf("demolita\n");
}

void ErrorPianifica(){
    printf("nessun percorso\n");
}

void SuccessPianifica(viaggio *v, int index_partenza, int index_arrivo)
{
    //stampa le tappe in ordine di percorrenza rappresentate con la distanza di ogni stazione
    //printf("\n");
    printf("%d ", highway.stazioni[index_partenza].distanza_da_inizio_autostrada);
    for (size_t i = 0; i < v->num_tappe; i++)
    {
        printf("%d ", v->tappa[v->num_tappe -i -1].distanza_da_inizio_autostrada);
    }
    printf("%d", highway.stazioni[index_arrivo].distanza_da_inizio_autostrada);
    printf("\n");
}

void SuccessPianificaReverse(viaggio *v, int index_partenza, int index_arrivo)
{
    // stampa le tappe in ordine di percorrenza rappresentate con la distanza di ogni stazione
    //printf("\n");
    printf("%d ", highway.stazioni[index_partenza].distanza_da_inizio_autostrada);
    for (size_t i = 0; i < v->num_tappe; i++)
    {
        printf("%d ", v->tappa[i].distanza_da_inizio_autostrada);
    }
    printf("%d", highway.stazioni[index_arrivo].distanza_da_inizio_autostrada);
    printf("\n");
}

void printHighway(){
    printf("\nHighway:{\n");
    printf("  \"Size\": %d,\n", highway.actual_size);
    printf("  \"Capacity\": %d,\n", highway.actual_capacity);

    printf("    Stazioni:[\n");
    for (int i = 0; i < highway.actual_size; i++)
    {
        printf("        {\n");
        printf("          \"Stazione\": %d,\n", i);
        printf("          \"Distanza\": %d,\n", highway.stazioni[i].distanza_da_inizio_autostrada);
        printf("          \"Index\": %d,\n", highway.stazioni[i].index);
        printparcoVeicoli(&highway.stazioni[i].parco);
        printf("   }%s\n", (i < highway.actual_size - 1) ? "," : "");
    }
    printf("  ]\n");
    printf("}\n");
}

void printStazione(int distance){
    printf("\n\n stampo stazione %d\n", distance);
    int index = get_index_station(distance);
    if(index == -1){
        printf("stazione non trovata\n");
        return;
    }
    printf("\nStazione:{\n");
    printf("  \"Stazione\": %d,\n", index);
    printf("  \"Distanza\": %d,\n", highway.stazioni[index].distanza_da_inizio_autostrada);
    printf("  \"Index\": %d,\n", highway.stazioni[index].index);
    printparcoVeicoli(&highway.stazioni[index].parco);
    printf("}\n");
}

void print_vector(int *v, int size, char *name){
    printf("\n %s: \n", name);
    for (int i = 0; i < size; i++)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

#define INITIAL_STATION 100
#define INITIAL_AUTO 320 //COMUNQUE MAX SONO 512

void free_autostrada() {
    //libero i parchi
    //free_parck();
    for (size_t i = 0; i < highway.actual_size; i++) {
        free_parck(&highway.stazioni[i].parco);
    }
    free(highway.stazioni);
    highway.stazioni = NULL;
    highway.actual_size = 0;
    highway.actual_capacity = 0;
}

void free_viaggio(viaggio *v) {
    free(v->tappa);
    v->num_tappe = -2;
}

void free_parck(struct parco_veicoli *p) {
    free(p->veicoli);
    p->veicoli = NULL;
    p->actual_size = 0;
    p->actual_capacity = 0;
}


void init_autostrada() {
    highway.stazioni = (struct stazione *) malloc(INITIAL_STATION * sizeof(struct stazione));
    highway.actual_size = 0;
    highway.actual_capacity = INITIAL_STATION;
}

// per ogni stazione inizializzo un parco con al max initial_auto+INITIAL_AUTO per evitare di far subito realloc
// con tot 25 auto per stazione occupazione ~ 50MB
void init_parco(parco_veicoli *p, int initial_auto) {
    p->veicoli = (struct veicolo *) malloc((initial_auto + INITIAL_AUTO) * sizeof(struct veicolo));
    p->actual_size = 0;// aggiorno a inizial_auto quando effettivamente faccio la add!
    p->actual_capacity = initial_auto + INITIAL_AUTO;
    p->curr_max = get_max_auto(p);

}

void init_viaggio(viaggio *v, int max_dim, veicolo macchina) {
    //.DEBUG
    //printf("\ninit_viaggio: max_dim: %d\n", max_dim);
    v->tappa = (struct stazione *) malloc(max_dim * sizeof(struct stazione));
    v->num_tappe = -1; // inizializzo a -1 caso in cui non trovo nessun percorso
}

//SETTER & MODIFICATORI

//non conviene ternerla ordinata, basta che mi salvo il massimo
//ORDINAMENTO PER AUTONOMIA: autonomia crescente
int add_auto_order(parco_veicoli *p, veicolo v) {
    if (p->actual_size == 512) {
        return -1;
    }
    if (p->actual_size == p->actual_capacity) {
        p->actual_capacity += INITIAL_AUTO;
        p->actual_capacity > 512 ? p->actual_capacity = 512 : p->actual_capacity;
        p->veicoli = (struct veicolo *) realloc(p->veicoli, p->actual_capacity * sizeof(struct veicolo));
    }

    int index = 0;

    while (index < p->actual_size && p->veicoli[index].autonomia < v.autonomia) {
        index++;
    }
    v.index = index;
    // shift
    for (int i = p->actual_size; i > index; i--) {
        p->veicoli[i] = p->veicoli[i - 1];
    }

    p->veicoli[index] = v;
    p->curr_max = p->veicoli[p->actual_size];
    p->actual_size++;

    return 0;
}

int add_auto(parco_veicoli *p, veicolo v) {
    if (p->actual_size == 512) {
        return -1;
    }
    if (p->actual_size == p->actual_capacity) {
        p->actual_capacity += INITIAL_AUTO;
        p->actual_capacity > 512 ? p->actual_capacity = 512 : p->actual_capacity;
        p->veicoli = (struct veicolo *) realloc(p->veicoli, p->actual_capacity * sizeof(struct veicolo));
    }
    v.index = p->actual_size;
    p->veicoli[p->actual_size] = v;
    p->curr_max = p->actual_size == 0 ? v : v.autonomia > p->curr_max.autonomia ? v : p->curr_max;

    p->actual_size++;


    return 0;
}

int rottama_auto(parco_veicoli *p, veicolo v) {
    int index = -1;

    for (index = 0; index < p->actual_size; index++) {
        if (p->veicoli[index].autonomia == v.autonomia) {
            break;
        }
    }

    if (index == -1 || index == p->actual_size) {
        return -1;
    }

    for (int i = index; i < p->actual_size - 1; i++) {
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
int add_station(stazione s, int initial_auto) {
    if (highway.actual_size == highway.actual_capacity) {
        highway.actual_capacity += INITIAL_STATION;
        highway.stazioni = (struct stazione *) realloc(highway.stazioni,
                                                       highway.actual_capacity * sizeof(struct stazione));
    }


    int index = 0;

    while (index < highway.actual_size &&
           highway.stazioni[index].distanza_da_inizio_autostrada <= s.distanza_da_inizio_autostrada) {
        if (highway.stazioni[index].distanza_da_inizio_autostrada == s.distanza_da_inizio_autostrada) {
            return -1;
        }

        index++;
    }

    // shift
    for (int i = highway.actual_size; i > index; i--) {
        highway.stazioni[i] = highway.stazioni[i - 1];
        highway.stazioni[i].index = i;
    }

    s.index = index;
    highway.actual_size++;
    highway.stazioni[index] = s;

    // siccome ho numero di veicoli quando faccio un add station lo faccio li!
    init_parco(&highway.stazioni[index].parco, initial_auto);

    return index;
}

int demolisci_stazione(int distanza) {
    int index = -1;
    for (size_t i = 0; i < highway.actual_size; i++) {
        if (highway.stazioni[i].distanza_da_inizio_autostrada == distanza) {
            index = i;
            //.DEBUG
            //printf("demolisco stazione %d   con indice: %d\n", distanza, index);
            break;
        }
    }

    if (index == -1) {
        return -1;
    }
    //.DEBUG
    //printparcoVeicoli(&highway.stazioni[index].parco);
    //libero memoria

    highway.actual_size--;
    free_parck(&highway.stazioni[index].parco);
    for (int i = index; i < highway.actual_size; i++) {
        highway.stazioni[i] = highway.stazioni[i + 1];
        highway.stazioni[i].index = i;
    }
    //printf("post for\n");


    return 0;
}

// Funzione per calcolare il piano di viaggio
// ATTENZIONE TAPPE E'RITORNATO IN ORDINE INVERSO!!
// la prima tappa da stampare e'quella in posizione v->num_tappe-1
void calculate_plan(viaggio *v, int index_partenza, int index_arrivo) {
    int curr_index = index_arrivo; // Inizialmente si parte dalla stazione di partenza
    int num_tappe = 0;
    int next_index;

    curr_index = get_best_station(index_partenza, index_arrivo);
    while (curr_index > index_partenza) {
        /*caso iniziale*/
        if (curr_index == index_arrivo) {
            // Non è stato possibile trovare un percorso
            //printf("non e' stato possibile trovare un percorso 1\n");
            v->num_tappe = -1;
            return;
        }
        v->tappa[num_tappe] = highway.stazioni[curr_index];
        num_tappe++;
        next_index = get_best_station(index_partenza, curr_index);
        //.DEBUG
        //printf("curr_index: %d curr distance: %d next index: %d next distance: %d\n", curr_index, highway.stazioni[curr_index].distanza_da_inizio_autostrada, next_index, highway.stazioni[next_index].distanza_da_inizio_autostrada);
        //printf("curr autonomia: %d \n", highway.stazioni[curr_index].parco.curr_max.autonomia);
        /*altri casi*/
        if (next_index == curr_index) {
            // Non è stato possibile trovare un percorso
            //printf("non e' stato possibile trovare un percorso 2\n");
            v->num_tappe = -1;
            return;
            //break;
        }
        curr_index = next_index;
    }
    v->num_tappe = num_tappe;
    //.DEBUG
    //printf("Numero di tappe: %d\n", num_tappe);
}


void fix_index() {
    for (size_t i = 0; i < highway.actual_size; i++) {
        highway.stazioni[i].index = i;
    }
}

//GETTERS
parco_veicoli *get_parco(int distanza) {
    for (size_t i = 0; i < highway.actual_size; i++) {
        if (highway.stazioni[i].distanza_da_inizio_autostrada == distanza) {
            return &highway.stazioni[i].parco;
        }
    }
    return NULL;
}

veicolo get_max_auto(parco_veicoli *p) {
    //ogni volta che lo chiamo setto anche il curr max nel parco!
    //caso in cui non ci sono auto!
    veicolo v2;
    v2.autonomia = -1;
    v2.index = -1;
    p->curr_max = p->actual_size == 0 ? v2 : p->veicoli[0];

    if (p->curr_max.index == -1) {//ottimizzazione
        return p->curr_max;
    }

    // DEVO FARE COosi perche non ho il vettore ordinato!
    for (int i = 1; i < p->actual_size; i++) {
        if (p->veicoli[i].autonomia > p->curr_max.autonomia) {
            p->curr_max = p->veicoli[i];
        }
    }

    return p->curr_max;
}

//ritorna indice della stazione se c'è una stazione alla distanza passata altrimenti -1
int get_index_station(int distanza) {
    for (size_t i = 0; i < highway.actual_size; i++) {
        if (highway.stazioni[i].distanza_da_inizio_autostrada == distanza) {
            return i;
        }
    }
    return -1;
}

// da richiamare <=> index_partenza < index_arrivo
// ritorna indice della stazione raggiungibile altrimenti index_partenza se non ne posso raggiungere
int get_index_max_raggiungible_station_cres(int index_partenza,
                                            int curr_autonomia/*distanza_partenza + autonomia macchina*/) {
    for (size_t i = index_partenza; i < highway.actual_size; i++) {
        if (highway.stazioni[i].distanza_da_inizio_autostrada -
            highway.stazioni[index_partenza].distanza_da_inizio_autostrada > curr_autonomia) {
            return i - 1;
        }
    }
    return highway.actual_size - 1;
}

// da richiamare <=> index_partenza > index_arrivo
// ritorna indice della stazione raggiungibile altrimenti index_partenza se non ne posso raggiungere
// se ho autonomia negativa ritorna index_partenza +1!
int get_index_max_raggiungible_station_desh(int index_partenza,
                                            int curr_autonomia /*distanza_partenza + autonomia macchina*/) {
    for (int i = index_partenza; i >= 0; i--) {
        if (highway.stazioni[index_partenza].distanza_da_inizio_autostrada -
            highway.stazioni[i].distanza_da_inizio_autostrada > curr_autonomia) {
            return i + 1;
        }
    }
    //.DEBUG
    //printf(" e' possibile raggiungere tutto con l autonomia fornita\n");
    return 0;
}

int get_next_best_station(int index_partenza, int index_arrivo, int min_index) {
    int best_index = index_partenza, ap, best_max_index_ragg = index_partenza;
    int flag_is_changed_best_station = 1;
    for (int i = index_partenza; i >= index_arrivo; i--) {
        ap = get_index_max_raggiungible_station_desh(i, highway.stazioni[i].parco.curr_max.autonomia);
        //.DEBUG
        /*
        printf("stazione: %d autonomia: %d\n  max stazione ragg: %d  index: %d \n\n",
               highway.stazioni[i].distanza_da_inizio_autostrada,
               highway.stazioni[i].parco.curr_max.autonomia,
               highway.stazioni[ap].distanza_da_inizio_autostrada,
               ap);
        */
        if (flag_is_changed_best_station == 1) {
            best_max_index_ragg = get_index_max_raggiungible_station_desh(best_index,
                                                                          highway.stazioni[best_index].parco.curr_max.autonomia);
            flag_is_changed_best_station = 0;
        }

        if (ap <= best_max_index_ragg || ap <= min_index) {
            best_index = i;
            flag_is_changed_best_station = 1;
        }
        /*printf("  curr best station: %d\n", highway.stazioni[best_index].distanza_da_inizio_autostrada);*/
    }

    return best_index;
}

// ritrona indice della stazione con autonomia migliore
// da richiamare <=> index_partenza < index_arrivo
// ritorna index_arrivo se la stazione di arrivo non e' raggiungibile da nessuna stazione da index_partenza
int get_best_station(int index_partenza, int index_arrivo) {
    //.DEBUG
    //printf("index_partenza: %d, index_arrivo: %d\n", index_partenza, index_arrivo);

    stazione current_station = highway.stazioni[index_arrivo];
    for (int i = index_arrivo - 1; i >= index_partenza; i--) {
        //.DEBUG
        //printf("\n current_station: %d\n, i: %d\n", current_station.index, i);

        /*mi permette di arrivare a stazione finale ed e'la piu piccola possibile*/
        if (highway.stazioni[i].parco.curr_max.autonomia + highway.stazioni[i].distanza_da_inizio_autostrada >=
            highway.stazioni[index_arrivo].distanza_da_inizio_autostrada) {
            //.DEBUG
            //printf("\n \r curr_max.autonomia: %d\n, stazioni[i].distanza_da_inizio_autostrada: %d\n, stazioni[index_arrivo].distanza_da_inizio_autostrada: %d", highway.stazioni[i].parco.curr_max.autonomia, highway.stazioni[i].distanza_da_inizio_autostrada, highway.stazioni[index_arrivo].distanza_da_inizio_autostrada);
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
int get_best_station_reverse(int index_partenza, int index_arrivo) {

    stazione current_station = highway.stazioni[index_arrivo];

    //.DEBUG
    // printf("index_partenza: %d, index_arrivo: %d\n", index_partenza, index_arrivo);
    int ap = index_partenza;
    index_partenza = index_arrivo;
    index_arrivo = ap;

    for (int i = index_arrivo; i >= index_partenza; i--) {
        //.DEBUG
        // printf("\n current_station: %d\n, i: %d\n", current_station.index, i);

        /*mi permette di arrivare a stazione finale ed e'la piu piccola possibile*/
        if (highway.stazioni[i].distanza_da_inizio_autostrada - highway.stazioni[i].parco.curr_max.autonomia <=
            highway.stazioni[index_partenza].distanza_da_inizio_autostrada) {
            //.DEBUG
            // printf("\n urr_max.autonomia: %d\n, stazioni[i].distanza_da_inizio_autostrada: %d\n, stazioni[index_arrivo].distanza_da_inizio_autostrada: %d", highway.stazioni[i].parco.curr_max.autonomia, highway.stazioni[i].distanza_da_inizio_autostrada, highway.stazioni[index_arrivo].distanza_da_inizio_autostrada);
            current_station = highway.stazioni[i];
        }
    }
    //.DEBUG
    // printf("\n end currwnt: %d\n", current_station.index);
    return current_station.index;
}




// Algoritmo di Dijkstra modificato per trovare il percorso minimo con le condizioni specificate
void calculate_plan_reverse_dijkstra_from_start(viaggio *v, int index_partenza, int index_arrivo) {
    int num_stazioni = index_partenza - index_arrivo + 1;
    int *costo = malloc(num_stazioni * sizeof(int));
    int *precedente = malloc(num_stazioni * sizeof(int)); //mi serve per ricostruire il percorso da aggiungere alle tappe!

    int *reversed_perc = malloc(num_stazioni * sizeof(int));//debug

    int *viaggio_index = malloc(num_stazioni * sizeof(int)); //mi serve per ricostruire il percorso da aggiungere alle tappe!
    int costo_curr;
    // Inizializzazione
    for (int i = 0; i < num_stazioni; i++) {
        costo[i] = INT_MAX;
        precedente[i] = -1;
    }
    costo[0] = 0; //il costo della stazione da cui inizia il percorso non costa nulla

    for (int curr_node = index_arrivo; curr_node <= index_partenza; curr_node++) {

        for (int i = curr_node +1/*non ha senso contare anche se stessi!*/; i<=  index_partenza ; i++) {

            if (get_index_max_raggiungible_station_desh(i, highway.stazioni[i].parco.curr_max.autonomia) <= curr_node){
                //i - index_arrivo (siccome index_arrivo < index_partenza) e' so schift necessario per trovare stazione corrente di cui sto calcolando il costo!
                //costo_curr = /*costo per arrivare a stazione corrente*/ costo[curr_node-index_arrivo] + highway.stazioni[i].distanza_da_inizio_autostrada;
                costo_curr = /*costo per arrivare a stazione corrente*/ costo[curr_node-index_arrivo] +  /*indice stazione nell autostrada*/i;
                //costo_curr = i; //non funziona mostra tutte le stazioni!

                if (costo[i-index_arrivo] > costo_curr){

                    //.DEBUG
                    printf("\n BEST FOUND: "
                           "\n  nuovo precedente trovato per la stazione %d ed e' staz %d\n", highway.stazioni[i].distanza_da_inizio_autostrada, highway.stazioni[curr_node].distanza_da_inizio_autostrada);

                    costo[i-index_arrivo] = costo_curr;
                    precedente[i-index_arrivo] = curr_node-index_arrivo;//ottimizzazione: salvo  l'index in cui devo andare a prendere la stazionein prec!

                    reversed_perc[i-index_arrivo] = highway.stazioni[curr_node].distanza_da_inizio_autostrada;

                    //old_i = i;//serve nel caso in cui il costo_curr sia uguale ma io devo preferire quella con distanza_da_inizio_autostrada minore!
                }
            }

        }
        //old_max_index_raggiungibile = max_index_raggiungibile;
    }

    //devo immettere tutto nel viaggio!
    /*
     * se tutto ok:
     * prec[0] == -1 se tutto bene sembrew cosi!!
     * prec[i] == -1 non ce nessun percorso!
     * */
    //.DEBUG
    /*
    print_vector(costo, num_stazioni, "costo:");
    print_vector(precedente, num_stazioni, "precedente:");
    print_vector(reversed_perc, num_stazioni, "reversed_perc:");
    */
    v->num_tappe = 0;
    for (int i =num_stazioni -1/*indice stazione di partenza*/; i >=0 /*indice stazione di arrivo*/; ) {
        if (precedente[i] == -1){
            //v->num_tappe = -1;
            //return;
        }
        viaggio_index[v->num_tappe] = precedente[i] + index_arrivo;
        v->num_tappe++;
        /*esempio di come si riempie:
         * v->num_tappe = 4
         * [10 11 12 13 - -]
         * questi sono gli indici io mi devo salvare in v->tappa le stazioni!
         * */
        i = precedente[i];
    }
    v->num_tappe--;//tolgo tappa di arrivo

    //print_vector(viaggio_index, num_stazioni, "viaggio_index:");

    for (int i = 0; i < v->num_tappe; i++) {
        v->tappa[i] = highway.stazioni[viaggio_index[i]];
    }

    free(costo);
    free(precedente);
    free(viaggio_index);
    free(reversed_perc);
}

/*CRITERI DI COSTO:
 * numero di tappe e distanza stazione
 *
 * costo:
 *   a 0 -> b 1 f 3
 *   b 1 -> a 1+1 f 1+1 e 1+5 c 1+3
 *   c 4 -> b 4+3 d 4+2
 *
 *   capisco che non ce nessun percorso perche e'presente  INT_MAX in costo[i] oppure precedente[i] = -1 e nodo e' diverso da [index_partenza - index_arrivo]
 * */
void calculate_plan_reverse_dijkstra_from_end(viaggio *v, int index_partenza, int index_arrivo) {
    int num_stazioni = index_partenza - index_arrivo + 1;
    int *costo = malloc(num_stazioni * sizeof(int));
    int *precedente = malloc(num_stazioni * sizeof(int)); //mi serve per ricostruire il percorso da aggiungere alle tappe!

    int *reversed_perc = malloc(num_stazioni * sizeof(int));//debug

    int *viaggio_index = malloc(num_stazioni * sizeof(int)); //mi serve per ricostruire il percorso da aggiungere alle tappe!
    int max_index_raggiungibile = -1;
    int costo_curr;
    //int old_i = INT_MAX;
    // Inizializzazione
    for (int i = 0; i < num_stazioni; i++) {
        costo[i] = INT_MAX;
        precedente[i] = -1;
    }
    costo[index_partenza - index_arrivo] = 0; //il costo della stazione da cui inizia il percorso non costa nulla

    for (int curr_node = index_partenza; curr_node > index_arrivo; curr_node--) {
        max_index_raggiungibile = get_index_max_raggiungible_station_desh(curr_node, highway.stazioni[curr_node].parco.curr_max.autonomia);
        //.DEBUG
        //printf("\n  curr_node: %d, max_index_raggiungibile: %d", curr_node, max_index_raggiungibile);
        //da curr_node a max_index_raggiungibile sono tutti quelli che sono collegati!
        for (int i = curr_node -1/*non ha senso contare anche se stessi!*/; i >= max_index_raggiungibile && i >= index_arrivo; i--) {
            //i - index_arrivo (siccome index_arrivo < index_partenza) e' so schift necessario per trovare stazione corrente di cui sto calcolando il costo!
            //costo_curr = /*costo per arrivare a stazione corrente*/ costo[curr_node-index_arrivo] + highway.stazioni[i].distanza_da_inizio_autostrada;
            costo_curr = /*costo per arrivare a stazione corrente*/ costo[curr_node-index_arrivo] +  1;
            //costo_curr = i; //non funziona mostra tutte le stazioni!


            if (costo[i-index_arrivo] >= costo_curr /*&& (i <= old_i)*/){

                //.DEBUG
//                printf("\n BEST FOUND: "
//                       "\n  nuovo precedente trovato per la stazione %d ed e' staz %d\n", highway.stazioni[curr_node].distanza_da_inizio_autostrada, highway.stazioni[i].distanza_da_inizio_autostrada);

                costo[i-index_arrivo] = costo_curr;
                precedente[i-index_arrivo] = curr_node-index_arrivo;//ottimizzazione: salvo  l'index in cui devo andare a prendere la stazionein prec!

                reversed_perc[i-index_arrivo] = highway.stazioni[curr_node].distanza_da_inizio_autostrada;

                //old_i = i;//serve nel caso in cui il costo_curr sia uguale ma io devo preferire quella con distanza_da_inizio_autostrada minore!
            }
        }
        //old_max_index_raggiungibile = max_index_raggiungibile;
    }

    //devo immettere tutto nel viaggio!
    /*
     * se tutto ok:
     * prec[0] == -1 se tutto bene sembrew cosi!!
     * prec[i] == -1 non ce nessun percorso!
     * */
    //.DEBUG
    /*
    print_vector(costo, num_stazioni, "costo:");
    print_vector(precedente, num_stazioni, "precedente:");
    print_vector(reversed_perc, num_stazioni, "reversed_perc:");
    */
    v->num_tappe = 0;
    for (int i =0/*indice stazione di arrivo*/; i < num_stazioni -1/*indice stazione di partenza*/; ) {
        if (precedente[i] == -1){
            v->num_tappe = -1;
            return;
        }
        viaggio_index[v->num_tappe] = precedente[i] + index_arrivo;
        v->num_tappe++;
        /*esempio di come si riempie:
         * v->num_tappe = 4
         * [10 11 12 13 - -]
         * questi sono gli indici io mi devo salvare in v->tappa le stazioni!
         * */
        i = precedente[i];
    }
    v->num_tappe--;//tolgo tappa di arrivo

    //print_vector(viaggio_index, num_stazioni, "viaggio_index:");

    for (int i = 0; i < v->num_tappe; i++) {
        v->tappa[i] = highway.stazioni[viaggio_index[v->num_tappe-i-1]];
    }

    free(costo);
    free(precedente);
    free(viaggio_index);
    free(reversed_perc);

}


int main()
{
    //leggo stringa per stringa

    //init variabili
    init();
    setVar();

    free_var();

    return 0;
}

void init(){
    init_autostrada();
}

void free_var(){
    free_autostrada();
}



void setVar() {
    //int i = 0;
    char *command;
    // Leggi il contenuto del file riga per riga
    while (fgets(buffer_in, sizeof(buffer_in), stdin) != NULL)
    {
        // riga completa
        //printf("\n %s", buffer_in);
        //i=0;
        //split string
        command = strtok(buffer_in, " ");
        controller(command);
        //.DEBUG
        //printHighway();
    }

    //.DEBUG
    //printHighway();
}

void controller(char *command){

    if (strcmp(command, "aggiungi-stazione") == 0)
    {
        addStation();
    }
    else if (strcmp(command, "demolisci-stazione") == 0)
    {
        demolisciStazione();
    }
    else if (strcmp(command, "aggiungi-auto") == 0)
    {
        addAuto();
    }
    else if (strcmp(command, "rottama-auto") == 0)
    {
        rottamaAuto();
    }
    else if (strcmp(command, "pianifica-percorso") == 0)
    {
        plan();
        //.DEBUG
        //printf("\nPOST: ");
        //printHighway();
    }

}

void addStation(){
    int distance, num_auto;
    int index;
    veicolo v;
    argument = strtok(NULL, " ");
    distance = atoi(argument);

    argument = strtok(NULL, " ");
    num_auto = atoi(argument);

    stazione s;
    s.distanza_da_inizio_autostrada = distance;
    index = add_station(s, num_auto);
    if (index == -1)
    {
        ErrorAdd();//stazione gia presente
        return;
    }

    while (argument != NULL)//adesso qui sono tutte auto
    {
        argument = strtok(NULL, " ");
        //.DEBUG
        //printf("\nargument: %s", argument);
        if (argument == NULL || strcmp(argument, newline) == 0)
        {
            break;
        }
        v.autonomia = atoi(argument);
        if (add_auto(&highway.stazioni[index].parco, v)==-1){
            //errore aggiunta auto
            ErrorAdd();
            return;
        }
    }
    SuccessAdd();

    //.DEBUG
    /*
    if (distance == 3038)
    {
        printf("\nstazione appena creata! ");
        printStazione(3038);
    }*/

}

void demolisciStazione(){
    int distance;
    argument = strtok(NULL, " ");
    distance = atoi(argument);
    // in questo particolare caso la stampa la faccio all interno di demolisci_stazione perche piu ottimizzata
    if (demolisci_stazione(distance) == 0)
    {
        SuccessDemolisci();
    }else{
        ErrorDemolisci();
    }
}

void addAuto(){
    int distance, autonomia;
    veicolo v;
    parco_veicoli *p;
    argument = strtok(NULL, " ");
    distance = atoi(argument);

    argument = strtok(NULL, " ");
    autonomia = atoi(argument);

    v.autonomia = autonomia;
    p = get_parco(distance);

    if(p== NULL){
        ErrorAdd();
        //.DEBUG
        //printf("parco: %d non trovato\n", distance);
        return;
    }
    if(add_auto(p, v) == 0){
        //.DEBUG
        //printf("parco: %d aggiunta auto: %d curr_max.autonomia: %d\n", distance, v.autonomia, p->curr_max.autonomia);
        SuccessAdd();
    }
    else
        ErrorAdd();

}

void rottamaAuto(){
    int distance, autonomia;
    veicolo v;
    parco_veicoli *p;
    argument = strtok(NULL, " ");
    distance = atoi(argument);

    argument = strtok(NULL, " ");
    autonomia = atoi(argument);

    v.autonomia = autonomia;
    p = get_parco(distance);
    if(p== NULL){
        ErrorRottama();
        return;
    }

    if(rottama_auto(p, v) == 0)
        SuccessRottama();
    else
        ErrorRottama();

}

/*
* minor numero di tappe possibili
*CASO IN CUI PIU PERCORSI HANNO LO STESSO NUMERO DI TAPPE
* si predilige le tappe con la distanza piu breve dall inizio dell autostrada (ovveri dist_partenza)

*/

void plan(){//nota ogni autostrada e' percorribile in 2 sensi di marcia
    int dist_partenza, dist_arrivo, index_partenza, index_arrivo;
    viaggio v;

    argument = strtok(NULL, " ");
    dist_partenza = atoi(argument);

    argument = strtok(NULL, " ");
    dist_arrivo = atoi(argument);

    index_partenza = get_index_station(dist_partenza);
    index_arrivo = get_index_station(dist_arrivo);

    //gia fatto! non si puo implementare questa ottimizzazione qui!
    //fix_index();

    //.DEBUG
    //printf("\nplan: index_partenza: %d, index_arrivo: %d\n", index_partenza, index_arrivo);

    //ERROR HANDLER
    if (index_partenza == -1 || index_arrivo == -1)
    {
        //.DEBUG
        //printf("\nindex_partenza: %d, index_arrivo: %d", index_partenza, index_arrivo);
        ErrorPianifica();
        return;
    }
    if (index_partenza == index_arrivo)
    {
        printf("\n%d\n", dist_partenza);
        return;
    }


    init_viaggio(&v,abs(index_arrivo - index_partenza) +2,highway.stazioni[index_partenza].parco.curr_max);



    if (index_partenza < index_arrivo)
    {
        calculate_plan(&v, index_partenza, index_arrivo);
        if (v.num_tappe == -1)
        {
            //.DEBUG
            //printHighway();
            ErrorPianifica();
            return;
        }
        SuccessPianifica(&v, index_partenza, index_arrivo);
    }else{
        //calculate_plan_reverse(&v, index_partenza, index_arrivo);
        //calculate_plan_reverse_dijkstra_from_end(&v, index_partenza, index_arrivo);
        calculate_plan_reverse_dijkstra_from_end(&v, index_partenza, index_arrivo);
        //.DEBUG
        //printHighway();
        if (v.num_tappe == -1)
        {
            ErrorPianifica();
            return;
        }
        SuccessPianificaReverse(&v,index_partenza, index_arrivo);
    }



    free_viaggio(&v);
    //.DEBUG

    /*if (dist_partenza == 835049915 && dist_arrivo == 72452031){
        printHighway();
        return;
    }*/

    //printf("\npost operazioni: ");
    //printStazione(3038);

}