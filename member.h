#ifndef MEMBER_H
#define MEMBER_H

#include "gnuplot_i.h"
#include "gene_definition.h"
#include "training_data.h"

struct Member
{
    double *gene;
    double fitness;
    struct GeneDefinition gene_definition;
};

double fitnessXY(struct Member *member, struct TrainingData *training_data);
double fitnessXYZ(struct Member *member, struct TrainingData *training_data);
void print_member(const struct Member *member);
void graph_member(gnuplot_ctrl *plot, const struct Member *member);
void generate_members_gene_copy_exchange(const struct Member *in_1, const struct Member *in_2, struct Member *out_1, struct Member *out_2);
void generate_members_gene_copy_exchange_random(const struct Member *in_1, const struct Member *in_2, struct Member *out_1, struct Member *out_2);
void generate_members_gene_copy(const struct Member *in_1, const struct Member *in_2, struct Member *out_1, struct Member *out_2);
void member_linear_gene_transform(struct Member *member, double factor, int down_or_up);

#endif
