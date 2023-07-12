void setVar();

void controller(char *command);

void addStation();

void addAuto();

void plan();

void init();

void free_var();

void free_autostrada();

void free_parck(parco_veicoli *p);

/*stampe*/
void printparcoVeicoli(parco_veicoli *parco);

void printVeicolo(veicolo *v);

void ErrorAdd();

void SuccessAdd();

/*end stampe*/
void init_autostrada();

void init_parco(struct parco_veicoli *p, int initial_auto);

void add_auto(parco_veicoli *p, veicolo v);

void add_station(stazione s, int initial_auto);

/*getter of list*/
parco_veicoli *get_parco(int distanza);