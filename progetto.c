#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // for INT_MAX
#include <stdbool.h> // for bool type

struct veicolo {   
  int autonomia;           
  int carica_batteria;      // espressa in km
}; 

struct parco_veicoli
{
    struct veicolo *veicoli;//max 512 veicoli
};

struct stazione {
    int distanza_da_inizio_autostada; //i veicolo possono raggiungere la stazione solo se sono autonomia > distanza_da_inizio_autostada
    int index; //indice della stazione indica indice in cui e posizionara stazione rispetto tutte le altre dell autostrada!
    //serve per ottimizzare al massimo il fatto che non si possa tornare indieto!
    struct parco_veicoli *parchi;
};

struct autostrada {
    struct stazione *stazioni;
};

struct viaggio {
    //si assume che stazione[0] sia la stazione di partenza e stazione[max_dim] la stazione di arrivo 
    struct stazione *tappa;
};

/*
* Dichiarazione di variabili globali
*/
char buffer_in[100];
FILE *file;

//non funzionante
/*
struct stazione *pianifica_percorso(struct stazione s1, struct stazione s2, struct autostrada a) {
    struct stazione *intermediate_stations = NULL;
    int num_intermediate_stations = 0;

    // Calcolo del numero di stazioni tra s1 e s2
    int num_stazioni = abs(s2.index - s1.index);

    // Initialize a visited array to track visited stations
    bool *visited = (bool*) malloc(num_stazioni * sizeof(bool));
    for (int i = 0; i < num_stazioni; i++)
        visited[i] = false;

    // Initialize a queue for BFS
    struct stazione **queue = (struct stazione**) malloc(num_stazioni * sizeof(struct stazione*));
    int front = 0; // Front of the queue
    int rear = 0; // Rear of the queue

    // Enqueue the starting station and mark it as visited
    queue[rear++] = &s1;
    visited[s1.index] = true;

    // Perform BFS
    while (front < rear) {
        struct stazione *current_station = queue[front++];

        // Check if the current station is the destination
        if (current_station == &s2) {
            intermediate_stations = (struct stazione*) malloc(num_intermediate_stations * sizeof(struct stazione));
            int index = num_intermediate_stations - 1;

            // Backtrack from the destination to the starting station
            while (current_station != &s1) {
                intermediate_stations[index--] = *current_station;
                current_station = current_station->tappa->parchi->veicoli[0];
            }

            // Add the starting station to the intermediate stations array
            intermediate_stations[index] = s1;
            break;
        }

        // Iterate through the adjacent stations
        for (int i = 0; i < num_stazioni; i++) {
            struct stazione adjacent_station = a.stazioni[i];

            // Check if the adjacent station is reachable and not visited
            for (int j = 0; j < adjacent_station.parchi->num_veicoli; j++) {
                if (adjacent_station.distanza_da_inizio_autostada - current_station->distanza_da_inizio_autostada <= current_station->parchi[j].veicoli[0].autonomia &&
                    !visited[i]) {
                    visited[i] = true;
                    queue[rear++] = &adjacent_station;
                    break; // Found a reachable vehicle, no need to check other vehicles
                }
            }
        }
    }

    free(visited);
    free(queue);

    return intermediate_stations;
}
*/

int main() {

    //lettura nomefile contenente gli input da stdin
    fgets(buffer_in,sizeof(buffer_in),stdin);

    //lettura riga per riga del file
    

    file = fopen(buffer_in, "r");
    if (file == NULL)
    {
        printf("Impossibile aprire il file %s\n", buffer_in);
        return 1;
    }

    // Leggi il contenuto del file riga per riga
    while (fgets(buffer_in, sizeof(buffer_in), file) != NULL)
    {
        // Fai qualcosa con la riga letta, ad esempio stampala
        printf("%s", buffer_in);
    }

    // Chiudi il file
    fclose(file);


    // Dichiarazioni di variabili e istruzioni

    // Inserisci qui il codice del programma

    return 0;
}