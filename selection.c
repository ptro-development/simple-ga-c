#include "selection.h"

void init_selection(struct Selection *selection, double cross_rate)
{
    selection->cross_rate = cross_rate;
}

void copy_selection(struct Selection *in, struct Selection *out)
{
    *out = *in;
}
