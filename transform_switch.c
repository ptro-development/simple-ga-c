#include "transform_switch.h"

void init_transform_switch(
    struct TransformSwitch *this,
    struct Context *actual_context, struct Context *prevoius_context,
    struct Population *actual_population, struct Population *previous_population,
    struct Population *actual_new_population, struct Population *previous_new_population
)
{
    this->actual_context = actual_context;
    this->prevoius_context = prevoius_context;
    this->actual_population = actual_population;
    this->previous_population = previous_population;
    this->actual_new_population = actual_new_population;
    this->previous_new_population = previous_new_population;
}

#define swap(x, y) do \
{ unsigned char swap_tmp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
  memcpy(swap_tmp, &y, sizeof(x)); \
  memcpy(&x, &y, sizeof(x)); \
  memcpy(&x, swap_tmp, sizeof(x)); \
} while(0)

void transform_switch_swap_data(struct TransformSwitch *tr_switch)
{
    struct Context *ctx_tmp;
    struct Population *pop_tmp;

    swap(tr_switch->actual_context, tr_switch->prevoius_context);
    swap(tr_switch->actual_population, tr_switch->previous_population);
    swap(tr_switch->actual_new_population, tr_switch->previous_new_population);
}
