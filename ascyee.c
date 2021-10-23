#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_lib/stb_image.h"

// The maximum number of character 
// that the user could input
static const int MAX_CHAR = 1024;

// Ascii greyscale map
static const char *ASCII_MAP = 
    "$@B%%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

bool is_valid_input(char *input);
void process_image(char *input, int downscale_ratio);

double *get_greyscaled_data(unsigned char *data, int height, int width);

double **get_downscale_map(double *data, int actual_width, int actual_height, 
        int n_cols, int height);

void print_ascii(double **map, int width, int height);
void free_downscale_map(double **downscale_map, int height);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("error: too many / few arguments: \n");
        printf("./ascyee [image_path] [downscale_ratio]\n\n");
        return 1;
    }

    // Get input from user
    char *input = argv[1];
    int downscale_ratio = atoi(argv[2]);

    // only process image if input is valid
    if (is_valid_input(argv[2])) {

        process_image(input, downscale_ratio);
    }

    return 0;
}

// Determine if input is valid 
// i.e input is valid when it is a character 
// excluding `spaces` and `\n` 
//
// Outputs error if false
bool is_valid_input(char *input) {
    for (int i = 0; input[i]; i++) {
        if (input[i] < '0' || input[i] > '9') {
            printf("Invalid input: downscale ratio is not a positive intiger: program terminates\n");
            return false;
        }
    }

    return true;
}

// Removes the `\n` in the input
char *strtrunc(char *input) {
    int len = strlen(input);
    char *result = calloc(sizeof(*input), strlen(input));
    for (int i = 0; i < len - 1; i++) {
        result[i] = input[i];
    }

    result[len - 1] = '\0';
    return result;
}

// Processes the image
// Output error message if any
void process_image(char *input, int downscale_ratio) {
    int width, height, n_channels;

    // loading greyscale images
    unsigned char *image_data = stbi_load(input, &width, &height, &n_channels, 3);

    // Make sure the file isnt corrupt or unable to open
    if (image_data == NULL) {
        printf("Image is corrupted or not found: program terminates\n");
        return;
    }

    double *greyscaled_data = get_greyscaled_data(image_data, height, width);

    // The actual width and height of the generated ASCII art
    int actual_width = width / downscale_ratio;
    int actual_height = height / (downscale_ratio * 2);

    // Downscale the data and return a 2d unsigned char array
    double **downscaled_map = 
        get_downscale_map(greyscaled_data, actual_width, actual_height, width, height);

    print_ascii(downscaled_map, actual_width, actual_height);
    
    free_downscale_map(downscaled_map, actual_height);
    stbi_image_free(image_data);
    free(greyscaled_data);
}

double *get_greyscaled_data(unsigned char *data, int height, int width) {
    int map_len = height * width;
    double *map = calloc(map_len, sizeof(*map));

    for (int i = 0; i < map_len; i++) {
        int index = i * 3;
        map[i] = 0.21 * (double)data[index] + 0.72 * (double)data[index + 1]
                    + 0.07 * (double)data[index + 2];
    }

    return map;
}

// Process the original data
// Return a 2d array of downscaled data
double **get_downscale_map(double *data, 
    int actual_width, int actual_height, int n_cols, int height) {

    // Initialize map
    double **map = calloc(actual_height, sizeof(*map));
    for (int i = 0; i < actual_height; i++) {

        int target_height = round((i / (double)actual_height) * height);
        
        // Initialize row
        double *row = calloc(actual_width, sizeof(*row));
        for (int j = 0; j < actual_width; j++) {
            int target_width = round((j / (double)actual_width) * n_cols);
            
            row[j] = data[(target_height * n_cols) + target_width];
        }
        map[i] = row;
    }

    return map;
}

void print_ascii(double **map, int width, int height) {
    // Number of elements in ASCII_MAP
    int map_len = strlen(ASCII_MAP);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int mapped_value = round((map[i][j]/ (double)255) * map_len);

            printf("%c", ASCII_MAP[mapped_value]);
        }
        printf("\n");
    }
}

void free_downscale_map(double **downscale_map, int height) {
    for (int i = 0; i < height; i++) {
        free(downscale_map[i]);
    }
    free(downscale_map);
}