#include "library.h"


int main()
{
    //leggo stringa per stringa

    //init variabili
    setVar();
    veicolo v;
    v.autonomia = 10;
    

    return 0;
}

void setVar() {
    int i = 0;
    char *command;
    // Leggi il contenuto del file riga per riga
    while (fgets(buffer_in, sizeof(buffer_in), stdin) != NULL)
    {
        // riga completa
        printf("\n %s", buffer_in);
        i=0;
        //split string
        command = strtok(buffer_in, " ");
        controller(command);

        
        while (argument != NULL)
        {
            if(i==0){

                i++;
            }
            //argument
            printf("%s\n", argument);
            argument = strtok(NULL, " ");
        }

    }

}

void controller(char *command){

    if (strcmp(command, "aggiungi-stazione") == 0)
    {
        addStation();
    }
    else if (strcmp(command, "demolisci-stazione") == 0)
    {
        //addstazione();
    }
    else if (strcmp(command, "aggiungi-auto") == 0)
    {
        //addautostrada();
    }
    else if (strcmp(command, "rottama-auto") == 0)
    {
        //addviaggio();
    }
    else if (strcmp(command, "pianifica-percorso") == 0)
    {
        plan();
    }
    
}

void addStation(){
    printf("aggiunta");
}

void plan(){
    printf("nessun percorso");

}