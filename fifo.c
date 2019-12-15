#include "fifo.h"
#include <stdio.h>

void init_fifo(struct Fifo *fifo, const int element_size, const int length)
{
    fifo->element_size = element_size;
    fifo->lenght = length;
    fifo->data_size = length * element_size;
    fifo->data = (double *) malloc(sizeof(double) * element_size * length);
    if (fifo->data != NULL)
    {
        fifo->data_pointer = fifo->data;
        fifo->last_pointer = fifo->data + fifo->data_size;
    }
}

void copy_fifo(struct Fifo *in, struct Fifo *out)
{
    out->data_size = in->data_size;
    out->element_size = in->element_size;
    out->lenght = in->lenght;
    out->data = (double *) malloc(sizeof(double) * in->element_size * in->lenght);
    if (out->data != NULL)
    {
        int i, j;
        // copy data
        for(j = 0; j < in->lenght; j += in->element_size)
        {
            for(i = 0; i < in->element_size; i++)
                out->data[j+i] = in->data[j+i];
        }
        out->data_pointer = out->data + (in->data_pointer - in->data);
        out->last_pointer = out->data + out->data_size;
    }
}

void fifo_linear_transform(struct Fifo *fifo, double factor, int down_or_up)
{
    int i, j;
    for(j = 0; j < fifo->lenght; j += fifo->element_size)
    {
        for(i = 0; i < fifo->element_size; i++)
            if (down_or_up == 0)
                fifo->data[j+i] /= factor;
            else
                fifo->data[j+i] *= factor;
    }
}

void put_in_fifo(struct Fifo *fifo, const double *values)
{
    int i;
    for(i = 0; i < fifo->element_size; i++)
        fifo->data_pointer[i] = values[i];

    fifo->data_pointer += fifo->element_size;
    if(fifo->data_pointer == fifo->last_pointer)
        fifo->data_pointer = fifo->data;
}

double * is_in_fifo(struct Fifo *fifo, const double *values)
{
    int j, i, match;
    fifo->data_pointer = fifo->data;
    for(j = 0; j < fifo->lenght; j+= fifo->element_size)
    {
        match = 1;
        for(i = 0; i < fifo->element_size; i++)
        {
            if(fifo->data_pointer[j+i] != values[i])
            {
                match = 0;
                break;
            }
        }
        if(match == 1)
            return &fifo->data_pointer[j];
    }
    return NULL;
}
