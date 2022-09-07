/**
 * @brief Header file for global macros and function prototypes
 * for the tl04 program.
 *
 * DO NOT MODIFY ANYTHING HERE
 */

#ifndef TL4_H
#define TL4_H

#include <stdlib.h>

#define UNUSED_PARAM(x) ((void) x) // This macro is only used for turning off compiler errors initially
#define UNUSED_FUNC(x) ((void) x)  // This macro is only used for turning off compiler errors initially

#define FAILURE 1
#define SUCCESS 0

struct ascii_image {
    int width;      // The number of columns in the ASCII image. Must be at least 1.
    int height;     // The number of rows in the ASCII image. Must be at least 1.
    char *name;     // The name of the ASCII image. This must be non-null, and must point to a deep copy that was allocated specifically for this struct.
    char *data;     // The image's data, which is a non-null, 1-dimensional char array of size width*height. Note: despite being a char*, this is not a string.
};

int is_printable(char c);
int set_character(struct ascii_image *image, int row, int col, char c);
int draw_diagonal_line(struct ascii_image *image, int row1, int col1, int row2, int col2, char c);
struct ascii_image *create_image(int height, int width, char *name);
void destroy_image(struct ascii_image *image);
int replace_extension(struct ascii_image *image, char *extension);
void print_image(struct ascii_image *image);

#endif
