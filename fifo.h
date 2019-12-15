#ifndef FIFO_H
#define FIFO_H

extern struct Fifo
{
    double *data;
    double *data_pointer;
    double *last_pointer;
    int data_size;
    int element_size;
    int lenght;
};

void init_fifo(struct Fifo *fifo, const int element_size, const int length);
void copy_fifo(struct Fifo *in, struct Fifo *out);
void put_in_fifo(struct Fifo *fifo, const double *values);
double * is_in_fifo(struct Fifo *fifo, const double *values);
void fifo_linear_transform(struct Fifo *fifo, double factor, int down_or_up);

#endif
