#ifndef SELECTION_H
#define SELECTION_H

extern struct Selection
{
    double cross_rate;
};

void init_selection(struct Selection *selection, double cross_rate);
void copy_selection(struct Selection *in, struct Selection *out);

#endif
