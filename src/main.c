#include "neuro.h"
#include "./lib/qdbmp.h"
#include <stdio.h>
#include <string.h>

//#define N_nodes_i_layer (16 * 16)
#define N_nodes_h_layer 36
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
    if (err_str != NULL)
        printf("%s\n", err_str);

    UINT width = BMP_GetWidth(image);
    UINT height = BMP_GetHeight(image);
    printf("width %ld\n", (UINT)width);
    printf("height %ld\n", (UINT)height);

    double **mass = calloc(height, sizeof(double *));
    for (int i = 0; i < height; i++) {
        mass[i] = calloc(width, sizeof(double));
    }


    UCHAR r, g, b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BMP_GetPixelRGB(image, x, y, &r, &g, &b);
            printf("r:%dg:%db:%d ", r, g, b);
            if ((r <= 50) && (g <= 50) && (b <= 50)) {
                mass[y][x] = 1;
            }
            else {
                mass[y][x] = 0;
            }
        }
        printf("\n");
    }

    err_str = BMP_GetErrorDescription();
    if (err_str != NULL)
        printf("%s\n", err_str);

    int N_nodes_i_layer = width * height;

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

    init_input_layer(input_layer, mass, width, height);
    init_next_layer(hidden_layer, N_nodes_h_layer);
    init_next_layer(output_layer, N_nodes_o_layer);

int flg = 1;
while (flg) {

    input_layer_calc(input_layer, N_nodes_i_layer);
    next_layer_calc(hidden_layer, N_nodes_h_layer, N_nodes_i_layer);
    next_layer_calc(output_layer, N_nodes_o_layer, N_nodes_h_layer);

    double most = -2;
    int digit = -1;
    for (int i = 0; i < N_nodes_o_layer; i++) {
        double tmp = output_layer[i]->output;
        if (tmp > most) {
            most = tmp;
            digit = i;
        }
    }

    printf("Ответ сети цифра: %d точность: %f \n", digit, most);
    char answer[100];

    while (1) {
        printf("Ответ верен? (y/n) или (yes/no): ");
        scanf("%s", answer);
        if ((strcmp(answer, "n") == 0) || (strcmp(answer, "no") == 0)) {
            printf("Продолжить обучение? (y/n) или (yes/no): ");
            scanf("%s", answer);
            if ((strcmp(answer, "n") == 0) || (strcmp(answer, "no") == 0)) {
                flg = 0; //обнулить флаг обучения
                //return EXIT_SUCCESS;//удалить
                break;
            }
            else if ((strcmp(answer, "y") == 0) || (strcmp(answer, "yes") == 0)) {
                printf("Введите правильный ответ: ");
                scanf("%d", &digit);
                if ((digit >= 0) && (digit <= 9)) {
                    printf("digit %d\n", digit);
                    break;
                }
                else {
                    printf("Неправильное значение\n");
                }
            }
            else {
                printf("Ведите только (y/n) или (yes/no)\n");
            }
        }
        else if ((strcmp(answer, "y") == 0) || (strcmp(answer, "yes") == 0)) {
            flg = 0; //обнулить флаг обучения
            //return EXIT_SUCCESS;//удалить
            break;
        }
        else {
            printf("Ведите только (y/n) или (yes/no)\n");
        }
        /*printf("Для выхода нажмите (y)\nДля продолжения нажмите любую кнопку: ");
        scanf("%s", answer);
        if (strcmp(answer, "y") == 0) {
            return EXIT_SUCCESS;
        }*/
    }

    double **exp_response = calloc(10, sizeof(double *));
    for (int i = 0; i < 10; i++) {
        exp_response[i] = calloc(10, sizeof(double));
    }
    for (int i = 0; i < 10; i++) {
        exp_response[i][i] = 1;
    }

    calc_err_and_delweight_out_layer(output_layer, hidden_layer, N_nodes_o_layer, N_nodes_h_layer, exp_response[digit]);
    calc_err_and_delweight_prew_layer(output_layer, hidden_layer, input_layer, N_nodes_o_layer, N_nodes_h_layer, N_nodes_i_layer);
    change_weight(output_layer, N_nodes_o_layer);
    change_weight(hidden_layer, N_nodes_h_layer);


    for (int i = 0; i < N_nodes_o_layer; i++ ) {
        printf("output: %d = %f\n", i, output_layer[i]->output);
        printf("%f\n", output_layer[i]->sum_imp);
    }
}
    return EXIT_SUCCESS;

}
