#include "neuro.h"

Neuron *neuron_create(int relations)
{
    Neuron *neuron = NULL;
    neuron = malloc(sizeof(*neuron));
    if (neuron != NULL) {
        neuron->input = malloc(relations * sizeof(double *));
        neuron->output = 0;
    }
    return neuron;
}

Neuron **layer_create(int number_nodes, int relations)
{
    Neuron **layer = NULL;
    layer = malloc(number_nodes * sizeof(*layer));
    if (layer != NULL) {
        for (size_t i = 0; i < number_nodes; i++) {
            layer[i] = neuron_create(relations);
            if (layer[i] == NULL)
                return NULL;
        }
    }
    return layer;
}

void tie_layers(Neuron **before, Neuron **after, int n_nodes_before, int n_nodes_after)
{
    for (int i = 0; i < n_nodes_after; i++) {
        for (int j = 0; j < n_nodes_before; j++) {
            after[i]->input[j] = &before[j]->output;
        }
    }
}

void init_input_layer(Neuron **neuron, double **mass, int width, int height)
{
    int place = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            neuron[place]->input[0] = &mass[i][j];
            place++;
        }
    }
}

void init_next_layer(Neuron **neuron, int number_nodes)
{
    for (int i = 0; i < number_nodes; i++) {
        neuron[i]->weight = f_rand(0.05, -0.05);
        neuron[i]->act_weight = f_rand(0.5, -0.5);
    }

}

double f_rand(double fMax, double fMin)
{
    srand(time(NULL) * rand());
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void input_layer_calc(Neuron **neuron, int number_nodes)
{
    for (int i = 0; i < number_nodes; i++) {
        neuron[i]->output = *neuron[i]->input[0];
    }
}

void next_layer_calc(Neuron **neuron, int number_nodes_l, int number_nodes_begin_l)
{
    for (int i = 0; i < number_nodes_l; i++) {
        for (int j = 0; j < number_nodes_begin_l; j++) {
            neuron[i]->sum_imp += ((*neuron[i]->input[j]) * (neuron[i]->weight));
        }
        neuron[i]->sum_imp += neuron[i]->act_weight;
        neuron[i]->output = activation_func(neuron[i]->sum_imp);
    }
}

void calc_err_and_delweight_out_layer(Neuron **after, Neuron **before, int n_nodes_after, int n_nodes_before, double *answer)
{
    for (int i = 0; i < n_nodes_after; i++) {
        after[i]->err = (answer[i] - after[i]->output) * derivative_activation_func(after[i]->sum_imp);
        after[i]->act_weight = speed * after[i]->err;
        for (int j = 0; j < n_nodes_before; j++) {
            after[i]->del_weight = speed * after[i]->err * before[j]->output;
        }
    }
}

void calc_err_and_delweight_prew_layer(Neuron **out, Neuron **hid, Neuron **inp, int n_nodes_out, int n_nodes_hid, int n_nodes_inp)
{
    for (int i = 0; i < n_nodes_hid; i++) {
        for (int j = 0; j < n_nodes_out; j++) {
            hid[i]->sum_err += out[j]->err * out[j]->weight;
        }
        hid[i]->err = derivative_activation_func(hid[i]->sum_err);
    }
    for (int i = 0; i < n_nodes_inp; i++) {
        for (int j = 0; j < n_nodes_hid; j++) {
            hid[j]->del_weight = speed * hid[j]->err * inp[i]->output;
            hid[j]->act_weight = speed * hid[j]->err;
        }
    }
}

void change_weight(Neuron **neuron, int n_nodes)
{
    for (int i = 0; i < n_nodes; i++) {
        neuron[i]->weight += neuron[i]->del_weight;
    }
}

double activation_func(double x)
{
    double y = 1 / (1 + exp((-1) * x * alf));
    return y;
}

double derivative_activation_func(double x)
{
    double z = activation_func(x);
    double y = alf * z * (1 - z);
    return y;
}
