#include "gene.h"

void init_gene(struct Gene *gene, const int min_value, const int max_value)
{
    gene->min_value = min_value;
    gene->max_value = max_value;
}
