#include "../app/neuro.h"
#include "../app/lib/qdbmp.h"
#include <string.h>

#define N_nodes_h_layer 20
#define N_nodes_o_layer 10

void reverse(char *s)
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char *s)
{
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

char *return_path(int i)
{
    const char *input_path = "../digits/";
    const char *end_path = ".bmp";
    char num[1];
    char *path;
    path = calloc(20, sizeof(char));
    itoa((int) i, num);
    strcat(path, input_path);
    strcat(path, num);
    strcat(path, end_path);
    return path;
}

int main()
{
    char *input_path;
    for (size_t j = 0; j < 1000; j++) {
        for (size_t i = 0; i <= 9; i++) {
            input_path = return_path(i);
            //printf("%s\n", input_path);
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

            int N_nodes_i_layer = (int) ((int) width * height);

            /*for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                    printf("%d ", (int) mass[i][j]);
                }
                printf("\n");
            }*/

            Neuron **input_layer = layer_create(N_nodes_i_layer, 1);
            Neuron **hidden_layer = layer_create(N_nodes_h_layer, N_nodes_i_layer);
            Neuron **output_layer = layer_create(N_nodes_o_layer, N_nodes_h_layer);

            tie_layers(input_layer, hidden_layer, N_nodes_i_layer, N_nodes_h_layer);
            tie_layers(hidden_layer, output_layer, N_nodes_h_layer, N_nodes_o_layer);

            srand((unsigned int) time(NULL));
            init_input_layer(input_layer, mass, (int) width, (int) height);
            init_next_layer(hidden_layer, N_nodes_h_layer, N_nodes_i_layer, "../src/memory/(w)i-h.txt");
            init_next_layer(output_layer, N_nodes_o_layer, N_nodes_h_layer, "../src/memory/(w)h-o.txt");

            double **exp_response = calloc(10, sizeof(double *));
            for (int i = 0; i < 10; i++) {
                exp_response[i] = calloc(10, sizeof(double));
            }
            for (int i = 0; i < 10; i++) {
                exp_response[i][i] = 1;
            }
            double most;
            int digit;

            input_layer_calc(input_layer, N_nodes_i_layer);
            next_layer_calc(hidden_layer, N_nodes_h_layer, N_nodes_i_layer);
            next_layer_calc(output_layer, N_nodes_o_layer, N_nodes_h_layer);

            most = -2;
            digit = -1;
            for (int i = 0; i < N_nodes_o_layer; i++) {
                double tmp = output_layer[i]->output;
                if (tmp > most) {
                    most = tmp;
                    digit = i;
                }
            }
            //printf("Ответ сети цифра: %d точность: %f \n", digit, most);
            calc_err_and_delweight_out_layer(output_layer, hidden_layer, N_nodes_o_layer, N_nodes_h_layer, exp_response[i]);
            double rms;
            rms = rms_err(output_layer, N_nodes_o_layer);
            printf("epoch %ld\n", j);
            printf("rms_err %f\n", rms);
            FILE *tb;
            char *out = calloc(30, sizeof(char));
            char *name = "../testData/rms_err";
            char *type = ".dat";
            strcat(out, name);
            char n[1];
            itoa(i, n);
            strcat(out, n);
            strcat(out, type);
            tb = fopen(out, "a");
            fprintf(tb, "%ld %.6f\n", j, rms);
            fclose(tb);
            if (digit != i || rms > 0.0003) {
                printf("неправильно\n");
                calc_err_and_delweight_prew_layer(output_layer, hidden_layer, input_layer, N_nodes_o_layer, N_nodes_h_layer, N_nodes_i_layer);
                change_weight(output_layer, N_nodes_o_layer, N_nodes_h_layer);
                change_weight(hidden_layer, N_nodes_h_layer, N_nodes_i_layer);
                write_weigh("../src/memory/(w)i-h.txt", hidden_layer, N_nodes_h_layer, N_nodes_i_layer);
                write_weigh("../src/memory/(w)h-o.txt", output_layer, N_nodes_o_layer, N_nodes_h_layer);
            }
            else {
                printf("правильно\n");
            }


            free(input_path);
        }
    }
    return 0;
}
