typedef struct 
{
    int autonomia;//peso BST
    int carica_batteria; // espressa in km
} veicolo; //bst || array[512]

typedef struct 
{
    struct veicolo *veicoli; // max 512 veicoli
} parco_veicoli;

typedef struct 
{
    int distanza_da_inizio_autostada; // i veicolo possono raggiungere la stazione solo se sono autonomia > distanza_da_inizio_autostada
    int index;                        // indice della stazione indica indice in cui e posizionara stazione rispetto tutte le altre dell autostrada!
    // serve per ottimizzare al massimo il fatto che non si possa tornare indieto!
    struct parco_veicoli *parchi;
} stazione;

typedef struct 
{
    struct stazione *stazioni; // lista inizialmente con 300 stazioni poi se sforo ne aggiungo altre 300 dinamicamente ogni volta
} autostrada;

typedef struct
{
    // si assume che stazione[0] sia la stazione di partenza e stazione[max_dim] la stazione di arrivo
    struct stazione *tappa;
} viaggio;