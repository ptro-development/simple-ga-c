#ifndef MUTATION_H
#define MUTATION_H

#include "mutation.h"
#include "member.h"
#include "fifo.h"
#include "line.h"
#include "gene_definition.h"

extern struct Mutation
{
  //minial mutation limit
  double min_limit;
  // max mutation limit
  double max_limit;
  // mutation step
  double step;
  // actual value of mutation
  double actual;
};

void init_mutation(struct Mutation *mt, double min_limit, double max_limit, double step);
void copy_mutation(struct Mutation *in, struct Mutation *out);
void limit_gene_mutation(const struct GeneDefinition *gene_definition, double *gene);

// different type of mutations
void mutate_gene_group_random_index(const struct Mutation *mutation,  const struct Line *line, struct Member *member, int gene_group_size);
void mutate_gene_group(const struct Mutation *mutation, const struct Line *line, struct Member *member, int gene_group_size, int gene_index);
void mutate(const struct Mutation *mutation, const struct Line *line, struct Member *member);
void adjust_mutation(struct Mutation *mutation);
void mutate_if_taboo(const struct Mutation *mutation, const struct Line *line, struct Member *member, struct Fifo *taboo_fifo, int retry_limit);

#endif
