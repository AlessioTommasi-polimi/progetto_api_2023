void printparcoVeicoli(parco_veicoli *parco)
{
    printf("{\n");
    printf("  \"Size\": %d,\n", parco->actual_size);
    printf("  \"Capacity\": %d,\n", parco->actual_capacity);

    printf("Veicoli:\n");
    for (int i = 0; i < parco->actual_size; i++)
    {
        printf("    {\n");
        printf("      \"Veicolo\": %d,\n", i + 1);
        printVeicolo(&parco->veicoli[i]);
        printf("    }%s\n", (i < parco->actual_size - 1) ? "," : "");   
    }
    printf("  ]\n");
    printf("}\n");
}


void printVeicolo (veicolo *v){
    printf("      \"Autonomia\": %d,\n", v->autonomia);
    printf("\n");
}

void ErrorAddAuto(){
    printf("non aggiunta\n");
}