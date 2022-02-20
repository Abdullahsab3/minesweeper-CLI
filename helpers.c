#include <math.h>
#include <stdlib.h>
#include "helpers.h"


// Dit zijn de hulpfuncties die geen deel uitmaken van de spellogica.


// een functie om een willekeurig getal van een gegeven interval terug te geven.
int myrandom(int min, int max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}