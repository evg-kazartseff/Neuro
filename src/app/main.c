#include <stdbool.h>
#include "neuro.h"
#include "./lib/qdbmp.h"


#define N_nodes_h_layer 20
#define N_nodes_o_layer 10

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Error\n");
        return EXIT_FAILURE;
    }

    const char *input_path = argv[1];

    BMP *image = NULL;
    image = BMP_ReadFile(input_path);

    const char *err_str = NULL;

    err_str = BMP_GetErrorDescription();
    if (err_str != NULL) {
        printf("%s\n", err_str);
        return EXIT_FAILURE;
    }

    UINT width = BMP_GetWidth(image);
    UINT height = BMP_GetHeight(image);

    double **mass = calloc(height, sizeof(double *));
    for (int i = 0; i < height; i++) {
        mass[i] = calloc(width, sizeof(double));
    }


    UCHAR r, g, b;
    for (UINT y = 0; y < height; y++) {
        for (UINT x = 0; x < width; x++) {
            BMP_GetPixelRGB(image, x, y, &r, &g, &b);
            if ((r <= 30) && (g <= 30) && (b <= 30)) {
                mass[y][x] = 1;
            }
            else {
                mass[y][x] = 0;
            }
        }
    }

    err_str = BMP_GetErrorDescription();
    if (err_str != NULL) {
        printf("%s\n", err_str);
        return EXIT_FAILURE;
    }
/*
    //noise
    mass[1][1] = 1;
    mass[3][1] = 1;
*/
    int N_nodes_i_layer = (int) ((int) width * height);

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("%d ", (int) mass[i][j]);
        }
        printf("\n");
    }

    Neuron **input_layer = layer_create(N_nodes_i_layer, 1);
    Neuron **hidden_layer = layer_create(N_nodes_h_layer, N_nodes_i_layer);
    Neuron **output_layer = layer_create(N_nodes_o_layer, N_nodes_h_layer);

    tie_layers(input_layer, hidden_layer, N_nodes_i_layer, N_nodes_h_layer);
    tie_layers(hidden_layer, output_layer, N_nodes_h_layer, N_nodes_o_layer);

    srand((unsigned int) time(NULL));
    init_input_layer(input_layer, mass, (int) width, (int) height);
    init_next_layer(hidden_layer, N_nodes_h_layer, N_nodes_i_layer, "../src/memory/(w)i-h.txt");
    init_next_layer(output_layer, N_nodes_o_layer, N_nodes_h_layer, "../src/memory/(w)h-o.txt");

    input_layer_calc(input_layer, N_nodes_i_layer);
    next_layer_calc(hidden_layer, N_nodes_h_layer, N_nodes_i_layer);
    next_layer_calc(output_layer, N_nodes_o_layer, N_nodes_h_layer);

    double most;
    int digit;
    most = -2;
    digit = -1;
    for (int i = 0; i < N_nodes_o_layer; i++) {
        double tmp = output_layer[i]->output;
        if (tmp > most) {
            most = tmp;
            digit = i;
        }
    }

    for (int i = 0; i < N_nodes_o_layer; i++ ) {
        printf("output: %d = %f\n", i, output_layer[i]->output);
        printf("%f\n", output_layer[i]->sum_imp);
    }

    printf("Ответ сети цифра: %d точность: %f \n", digit, most);

    return EXIT_SUCCESS;
}
