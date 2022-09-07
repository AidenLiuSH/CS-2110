/**
 * @file main.c
 * @author YOUR TAs!
 * @date 2021
 *
 * DO NOT MODIFY THIS FILE.
 */
#include "tl04.h"
#include <stdio.h>

/**
 * @brief A main function that you can use for testing.
 * 
 * Implementing this function is NOT necessary for the timed lab.
 * You can use this if you want to create custom tests.
 * We have included a basic test as an example.
 *
 * @param argc argument count
 * @param argv argument vector (it's an array of strings)
 * @return int status code of the program termination
 */
int main(int argc, char const *argv[])
{
    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    // Creates an image with height 5 and width 7 called "art.png".
    struct ascii_image *image = create_image(5, 7, "art.png");

    // Clears the image with periods.
    for (int i = 0; i < 5 * 7; i++)
        image->data[i] = '.';

    // Sets a character in the top-right corner with 'X'.
    set_character(image, 0, 4, 'X');

    // Draws a few diagonal lines within the image.
    printf("%d\n", draw_diagonal_line(image, 0, 3, 2, 5, 'X'));
    draw_diagonal_line(image, 0, 0, 5, 5, 'B');
    draw_diagonal_line(image, 3, 0, 1, 2, 'A');

    // Replaces the .png extension with a .jpeg extension.
    replace_extension(image, ".jpeg");

    // Prints the image's name and data.
    printf("Image name: %s\n", image->name);
    for (int i = 0; i < 5 * 7; i++)
    {
        printf("%c", image->data[i]);
        if (i % 7 == 6)
            printf("\n");
    }
    // Destroy the image, avoiding memory leaks.
    destroy_image(image);
}
