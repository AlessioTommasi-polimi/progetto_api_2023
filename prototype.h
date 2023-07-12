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

void ErrorAdd();

void SuccessAdd();

void ErrorRottama();

void SuccessRottama();

void ErrorDemolisci();

void SuccessDemolisci();

/*end stampe*/

/*list_manager*/
void init_autostrada();

void init_parco(struct parco_veicoli *p, int initial_auto);

int add_auto_order(parco_veicoli *p, veicolo v);

int add_auto(parco_veicoli *p, veicolo v);

int rottama_auto(parco_veicoli *p, veicolo v);

int add_station(stazione s, int initial_auto);

int demolisci_stazione(int distanza);

void free_autostrada();

void free_parck(parco_veicoli *p);

/*getter of list*/
parco_veicoli *get_parco(int distanza);

veicolo  get_max_auto(parco_veicoli *p);