


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