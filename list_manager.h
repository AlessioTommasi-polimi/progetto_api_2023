#define INITIAL_STATION 100
#define INITIAL_AUTO 100 //COMUNQUE MAX SONO 512

void free_autostrada()
{
    //libero i parchi
    //free_parck();
    for (size_t i = 0; i < highway.actual_size; i++)
    {
        free_parck(&highway.stazioni[i].parco);
    }
    free(highway.stazioni);
    highway.stazioni = NULL;
    highway.actual_size = 0;
    highway.actual_capacity = 0;
}

void free_parck(struct parco_veicoli* p)
{
    free(p->veicoli);
    p->veicoli = NULL;
    p->actual_size = 0;
    p->actual_capacity = 0;
}