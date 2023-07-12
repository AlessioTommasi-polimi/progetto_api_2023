void printparcoVeicoli(parco_veicoli *parco)
{
    printf("           Parco Veicoli:{\n");
    printf("            \"Size\": %d,\n", parco->actual_size);
    printf("            \"Capacity\": %d,\n", parco->actual_capacity);
    printf("             Veicoli:[\n");

    
    for (int i = 0; i < parco->actual_size; i++)
    {
        printf("                 {\n");
        printf("                   \"numero veicolo\": %d,\n", i + 1);
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

void printHighway(){
    printf("\nHighway:{\n");
    printf("  \"Size\": %d,\n", highway.actual_size);
    printf("  \"Capacity\": %d,\n", highway.actual_capacity);

    printf("    Stazioni:[\n");
    for (int i = 0; i < highway.actual_size; i++)
    {
        printf("        {\n");
        printf("          \"Stazione\": %d,\n", i + 1);
        printf("          \"Distanza\": %d,\n", highway.stazioni[i].distanza_da_inizio_autostada);
        printf("          \"Index\": %d,\n", highway.stazioni[i].index);
        printparcoVeicoli(&highway.stazioni[i].parco);
        printf("   }%s\n", (i < highway.actual_size - 1) ? "," : "");   
    }
    printf("  ]\n");
    printf("}\n");
}