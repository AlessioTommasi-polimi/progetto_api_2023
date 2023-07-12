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
    int distanza_da_inizio_autostada; // i veicolo possono raggiungere la stazione solo se sono autonomia > distanza_da_inizio_autostada
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
    // si assume che stazione[0] sia la stazione di partenza e stazione[max_dim] la stazione di arrivo
    stazione *tappa;
} viaggio;