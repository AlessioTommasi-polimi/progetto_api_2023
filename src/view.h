void printparcoVeicoli(parco_veicoli *parco)
{
    printf("           Parco Veicoli:{\n");
    printf("            \"Size\": %d,\n", parco->actual_size);
    printf("            \"Capacity\": %d,\n", parco->actual_capacity);
    printf("             Veicoli:[\n");
    printf("              curr_max: {\"Autonomia\": %d \"index\": %d},\n", parco->curr_max.autonomia, parco->curr_max.index);

    for (int i = 0; i < parco->actual_size; i++)
    {
        printf("                 {\n");
        printf("                   \"numero veicolo\": %d,\n", i);//se tutto giusto corrisponde con index!
        printVeicolo(&parco->veicoli[i]);
        printf("                 }%s\n", (i < parco->actual_size - 1) ? "," : "");
    }
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
    printf("nessun percorso");
}

void SuccessPianifica(viaggio *v)
{
    //stampa le tappe in ordine di percorrenza rappresentate con la distanza di ogni stazione
    printf("\n");
    for (size_t i = 0; i < v->num_tappe; i++)
    {
        printf("%d ", v->tappa[v->num_tappe -i -1].distanza_da_inizio_autostrada);
    }
    
}

void SuccessPianificaReverse(viaggio *v)
{
    // stampa le tappe in ordine di percorrenza rappresentate con la distanza di ogni stazione
    printf("\n");
    for (size_t i = 0; i < v->num_tappe; i++)
    {
        printf("%d ", v->tappa[i].distanza_da_inizio_autostrada);
    }
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