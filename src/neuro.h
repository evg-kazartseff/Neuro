#ifndef NEURO_H
#define NEURO_H

#include <stdlib.h>

#define input_layer 16
#define hidden_layer 6
#define output_layer 10

struct Neuron {
    int *input;
    int output;
};

struct Neuron *neuron_create(int relations);
struct Neuron **layer_create(int number_nodes);

#endif
