#include "neuro.h"

struct Neuron *neuron_create(int relations) {
    struct Neuron *neuron = NULL;
    neuron = malloc(sizeof(*neuron));
    if (neuron != NULL) {
        neuron->input = malloc (relations * sizeof(int));
        neuron->output = 12;//del str
    }
    return neuron;
}

struct Neuron **layer_create(int number_nodes) {
    struct Neuron **layer = NULL;
    layer = malloc(number_nodes * sizeof(*layer));
    if (layer != NULL) {
        for (size_t i = 0; i < number_nodes; i++) {
            layer[i] = neuron_create(number_nodes);
            if (layer[i] == NULL)
                return NULL;
        }
    }
    return layer;
}
