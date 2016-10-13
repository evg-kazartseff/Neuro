#ifndef NEURO_H
#define NEURO_H

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define alf 0.5 //коэфициент наклона сигмоидной функции
#define speed 1 //скорость обучения сети

typedef struct {
    double **input; //массив входных импульсов
    double sum_imp; //суммарное значемни импульсов на входе
    double act_weight; //смещение нейрона на выходе(вес активаци)
    double output; //выходной импульс нейрона

    double weight; //вес связи между нейроном данного слоя и нейроном предыдущего слоя

    double err; //велечина ошибки
    double sum_err; //суммарная величина ошибки
    double del_weight; //величина на которую изменяется вес
}Neuron;

Neuron *neuron_create(int relations);
Neuron **layer_create(int number_nodes, int relations);
void tie_layers(Neuron **before, Neuron **after, int n_nodes_before, int n_nodes_after);
void init_input_layer(Neuron **neuron, double **mass, int width, int height);
void init_next_layer(Neuron **neuron, int number_nodes);
double f_rand(double fMax, double fMin);
void input_layer_calc(Neuron **neuron, int number_nodes);
void next_layer_calc(Neuron **neuron, int number_nodes_l, int number_nodes_begin_l);
void calc_err_and_delweight_out_layer(Neuron **after, Neuron **before, int n_nodes_after, int n_nodes_before, double *answer);
void calc_err_and_delweight_prew_layer(Neuron **out, Neuron **hid, Neuron **inp, int n_nodes_out, int n_nodes_hid, int n_nodes_inp);
void change_weight(Neuron **neuron, int n_nodes);
double activation_func(double x);
double derivative_activation_func(double x);

#endif
