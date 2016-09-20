#include "neuro.h"
#include <stdio.h>

int main() {
    struct Neuron **i_layer = layer_create(input_layer);
    printf("%d\n", i_layer[1]->output);
    return 0;
}
