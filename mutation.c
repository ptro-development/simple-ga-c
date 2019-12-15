#include <stdlib.h>

#include "mutation.h"

void init_mutation(struct Mutation *mt, double min_limit, double max_limit, double step)
{
    mt->min_limit = min_limit;
    mt->max_limit = max_limit;
    mt->step = step;
    mt->actual = min_limit;
}

void copy_mutation(struct Mutation *in, struct Mutation *out)
{
    *out = *in;
}

void limit_gene_mutation(const struct GeneDefinition *gene_definition, double *gene)
{
    if(*gene < gene_definition->min_value)
        *gene = gene_definition->min_value;
    else
    {
        if(*gene > gene_definition->max_value)
            *gene = gene_definition->max_value;
    }
}

/*
To write function which mutates only the part of gene (segment)

 Details:

 - this should happen only if there is stagnation for longer time period and
 a whole fitness is very high, this is somehow opposite to random mutation
 across a whole gene - let call this selective, focused or group mutation

 - the main problem is to find how big the group for mutation is.
*/

void mutate_gene_group_random_index(const struct Mutation *mutation, const struct Line *line, struct Member *member, int gene_group_size)
{
    int i, gene_index;
    double delta;

    gene_index = (int)((double)member->gene_definition.size * rand() / RAND_MAX);
    if(member->gene_definition.size - gene_index < gene_group_size) // shift gene_index if necessary
        gene_index = member->gene_definition.size - gene_group_size;

    for (i=0; i<gene_group_size ; i++)
    {
        get_delta_line_normal_distribution(mutation, line, member->gene_definition, &delta);
        member->gene[gene_index + i] += delta;
        limit_gene_mutation(&member->gene_definition, &member->gene[gene_index + i]);
    }
}

void mutate_gene_group(const struct Mutation *mutation, const struct Line *line, struct Member *member, int gene_group_size, int gene_index)
{
    int i;
    double delta;

    //print_member(member);
    for (i=0; i < gene_group_size ; i++)
    {
        get_delta_line_normal_distribution(mutation, line, member->gene_definition, &delta);
        member->gene[gene_index + i] += delta;
        limit_gene_mutation(&member->gene_definition, &member->gene[gene_index + i]);
        //printf("In %d delta %f gene_group_size %d gene_index %d gene %f\n", i, delta, gene_group_size, gene_index, member->gene[gene_index + i]);
    }
    //print_member(member);
}

void mutate(const struct Mutation *mutation, const struct Line *line, struct Member *member)
{
    double delta;
    int j;
    for(j = 0; j < member->gene_definition.size; j++)
    {
        if((double)rand()/RAND_MAX < mutation->actual)
        {
            // normal distribution
            get_delta_line_normal_distribution(mutation, line, member->gene_definition, &delta);
            //printf("mutation num:%f gene:%f\n", random_num, member->gene[j]);
            member->gene[j] += delta;
            //printf("mutation num:%f gene:%f\n", random_num, member->gene[j]);
            limit_gene_mutation(&member->gene_definition, &member->gene[j]);
        }
    }
}

void adjust_mutation(struct Mutation *mutation)
{
    mutation->actual += mutation->step;
    if(mutation->actual > mutation->max_limit)
    {
        mutation->actual = mutation->min_limit;
    }
}

void mutate_if_taboo(const struct Mutation *mutation, const struct Line *line, struct Member *member, struct Fifo *taboo_fifo, int retry_limit)
{
    int i;
    for (i = 0; i < retry_limit; i++)
    {
        if(is_in_fifo(taboo_fifo, member->gene) != NULL)
        {
            printf("1 i is %d\n", i);
            mutate(mutation, line, member);
        }
        else
            break;
    }
}
