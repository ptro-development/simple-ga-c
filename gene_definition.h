#ifndef GENE_DEFINITION_H
#define GENE_DEFINITION_H

struct GeneDefinition
{
    // min allowed value of gene
    double min_value;
    // max allowed value of gene
    double max_value;
    // length of gene
    int size;
};

void init_gene_definition(struct GeneDefinition *gene_definition, const int size, const double min_value, const double max_value);
void copy_gene_definition(struct GeneDefinition *in, struct GeneDefinition *out);
void gene_definition_linear_transform(struct GeneDefinition *gene_definition, double factor, int down_or_up);

#endif
