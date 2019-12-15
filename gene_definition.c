#include "gene_definition.h"

void init_gene_definition(struct GeneDefinition *gene_definition, const int size, const double min_value, const double max_value)
{
    gene_definition->size = size;
    gene_definition->min_value = min_value;
    gene_definition->max_value = max_value;
}

void copy_gene_definition(struct GeneDefinition *in, struct GeneDefinition *out)
{
    *out = *in;
}

void gene_definition_linear_transform(struct GeneDefinition *gene_definition, double factor, int down_or_up)
{
    if(down_or_up == 0)
    {
        gene_definition->min_value /= factor;
        gene_definition->max_value /= factor;
    }
    else
    {
        gene_definition->min_value *= factor;
        gene_definition->max_value *= factor;
    }
}
