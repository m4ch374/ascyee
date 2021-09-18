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

// Multiplyer for downscaling
static const int DOWNSCALE_MULTIPLYER_X = 5;
static const int DOWNSCALE_MULTIPLYER_Y = 10;

bool is_valid_input(char *input);
char *strtrunc(char *input);
void process_image(char *input);

unsigned char **get_downscale_map(unsigned char *data, 
    int actual_width, int actual_height, int n_cols);

unsigned char get_chunk_average(unsigned char *data, 
    int starting_width, int starting_height, int width);

void print_ascii(unsigned char **map, int width, int height);
void free_downscale_map(unsigned char **downscale_map, int height);

int main(void) {

    // Get input from user
    char input[MAX_CHAR];
    printf("Enter the path for image: ");
    fgets(input, MAX_CHAR, stdin);

    // only process image if input is valid
    if (is_valid_input(input)) {

        // Remove the `\n` in the original string
        char *processed_input = strtrunc(input);
        process_image(processed_input);
        free(processed_input);
    }

    return 0;
}

// Determine if input is valid
// i.e input is valid when it is a character
// excluding `spaces` and `\n`
//
// Outputs error if false
bool is_valid_input(char *input) {
    for (int i = 0; i < strlen(input) - 1; i++) {
        if (input[i] < '!' || input[i] > '~') {
            printf("Invalid input, program terminates\n");
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
void process_image(char *input) {
    int width, height, n_channels;

    // loading greyscale images
    unsigned char *image_data = stbi_load(input, &width, &height, &n_channels, 1);

    // Make sure the file isnt corrupt or unable to open
    assert(image_data != NULL);

    // The actual width and height of the generated ASCII art
    int actual_width = width / DOWNSCALE_MULTIPLYER_X;
    int actual_height = height / DOWNSCALE_MULTIPLYER_Y;

    // Downscale the data and return a 2d unsigned char array
    unsigned char **downscaled_map = 
        get_downscale_map(image_data, actual_width, actual_height, width);

    print_ascii(downscaled_map, actual_width, actual_height);
    
    free_downscale_map(downscaled_map, actual_height);
    stbi_image_free(image_data);
}

// Process the original data
// Return a 2d array of downscaled data
unsigned char **get_downscale_map(unsigned char *data, 
    int actual_width, int actual_height, int n_cols) {

    // Initialize map
    unsigned char **map = calloc(actual_height, sizeof(unsigned char *));
    for (int i = 0; i < actual_height; i++) {

        // Set start height of original data
        int start_height = i * DOWNSCALE_MULTIPLYER_Y - 1;
        start_height = (start_height < 0) ? 0 : start_height;
        
        // Initialize row
        unsigned char *row = calloc(actual_width, sizeof(unsigned char));
        for (int j = 0; j < actual_width; j++) {

            // Set start width of original data
            int start_width = j * DOWNSCALE_MULTIPLYER_X - 1;
            start_width = (start_width < 0) ? 0 : start_width;

            unsigned char average = 
                get_chunk_average(data, start_width, start_height, n_cols);

            row[j] = average;
        }
        map[i] = row;
    }

    return map;
}

unsigned char get_chunk_average(unsigned char *data, 
    int starting_width, int starting_height, int width) {

    int total = 0;
    for (int i = 0; i < DOWNSCALE_MULTIPLYER_Y; i++) {
        for (int j = 0; j < DOWNSCALE_MULTIPLYER_X; j++) {
            total += (int)data[(starting_height + i) * width + (starting_width + j)];
        }
    }

    unsigned char result = (unsigned char)
        round((double)total / (double)(DOWNSCALE_MULTIPLYER_Y * DOWNSCALE_MULTIPLYER_X));

    //printf("%d\n", result);
    return result;
}

void print_ascii(unsigned char **map, int width, int height) {
    // Number of elements in ASCII_MAP
    int map_len = strlen(ASCII_MAP);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int mapped_value = round(((double)map[i][j]/ (double)255) * map_len);

            printf("%c", ASCII_MAP[mapped_value]);
        }
        printf("\n");
    }
}

void free_downscale_map(unsigned char **downscale_map, int height) {
    for (int i = 0; i < height; i++) {
        free(downscale_map[i]);
    }
    free(downscale_map);
}