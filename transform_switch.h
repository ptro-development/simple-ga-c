#ifndef TRANSFORM_SWITCH_H
#define TRANSFORM_SWITCH_H

#include "context.h"

extern struct TransformSwitch
{
    struct Context *actual_context; struct Context *prevoius_context;
    struct Population *actual_population; struct Population *previous_population;
    struct Population *actual_new_population; struct Population *previous_new_population;
};

void init_transform_switch(
    struct TransformSwitch *this,
    struct Context *actual_context, struct Context *prevoius_context,
    struct Population *actual_population, struct Population *previous_population,
    struct Population *actual_new_population, struct Population *previous_new_population
);

void transform_switch_swap_data(struct TransformSwitch *tr_switch);

#endif
