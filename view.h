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

void SuccessPianifica(){
    //stampa le tappe in ordine di percorrenza 
}

void printHighway(){
    printf("\nHighway:{\n");
    printf("  \"Size\": %d,\n", highway.actual_size);
    printf("  \"Capacity\": %d,\n", highway.actual_capacity);

    printf("    Stazioni:[\n");
    for (int i = 0; i < highway.actual_size; i++)
    {
        printf("        {\n");
        printf("          \"Stazione\": %d,\n", i + 1);
        printf("          \"Distanza\": %d,\n", highway.stazioni[i].distanza_da_inizio_autostrada);
        printf("          \"Index\": %d,\n", highway.stazioni[i].index);
        printparcoVeicoli(&highway.stazioni[i].parco);
        printf("   }%s\n", (i < highway.actual_size - 1) ? "," : "");   
    }
    printf("  ]\n");
    printf("}\n");
}