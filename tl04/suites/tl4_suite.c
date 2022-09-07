/**
 * @file tl4_suite.c
 * @author Sameer
 * @brief Testing Suites to TL4 - Fall 2020
 * @date 2021-11-09
 */

// Check Docs
// Tutorial : https://libcheck.github.io/check/doc/check_html/check_3.html
// Check API: https://libcheck.github.io/check/doc/doxygen/html/check_8h.html

// System Headers
#include <string.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h> // For grabbing return value of system call

// TA Headers
#include "test_utils.h"

int mallocs_until_fail = -1;

// allow infinite mallocs by default
static void reset_mallocs_until_fail(void) {
    mallocs_until_fail = -1;
}

static char *render_image(int width, int height, const char *data) {
    size_t new_len = height * (width + 1);
    char *str = malloc(new_len + 1);
    for (int row = 0; row < height; row++) {
        memcpy(&str[row * (width + 1)], &data[row * width], width);
    }
    for (int row = 0; row < height; row++) {
        str[row * (width + 1) + width] = '\n';
    }
    str[new_len] = '\0';
    return str;
}

const char *_big_image =
    "..|.."
    "./_\\."
    ".|@|."
    ".|_|."
    "/___\\"
    ".VvV.";
static int _big_image_width = 5;
static int _big_image_height = 6;
static struct ascii_image *_create_big_image(void) {
    struct ascii_image *img = _create_image(_big_image_height, _big_image_width, "big_image");
    memcpy(img->data, _big_image, _big_image_width * _big_image_height);
    return img;
}

static struct ascii_image *_create_small_image(void) {
    struct ascii_image *img = _create_image(5, 7, "small_image");
    return img;
}

static void assert_image_data(struct ascii_image *image, struct ascii_image *expected) {
    if ((image->data == NULL) && (expected->data != NULL)) {
        ck_abort_msg("expected data to not be NULL");
    } else if ((image->data != NULL) && (expected->data == NULL)) {
        ck_abort_msg("expected data to be NULL");
    } else if ((image->data != NULL) && (expected->data != NULL)) {
        int same = memcmp(image->data, expected->data, expected->width * expected->height) == 0;
        // don't generate strings if comparison succeeds
        if (!same) {
            char *e = render_image(expected->width, expected->height, expected->data);
            char *i = render_image(image->width, image->height, image->data);
            ck_abort_msg("expected image \n%sbut got image \n%s", e, i);
            free(e);
            free(i);
        }
    }
}

static void assert_image(struct ascii_image *image, struct ascii_image *expected) {
    ck_assert_msg(image->width == expected->width, "expected width %d, but got width %d", expected->width, image->width);
    ck_assert_msg(image->height == expected->height, "expected height %d, but got height %d", expected->height, image->height);
    if ((image->name == NULL) && (expected->name != NULL)) {
        ck_abort_msg("expected name \"%s\", but was NULL", expected->name);
    } else if ((image->name != NULL) && (expected->name == NULL)) {
        ck_abort_msg("expected name NULL, but was \"%s\"", image->name);
    } else if ((image->name != NULL) && (expected->name != NULL)) {
        ck_assert_msg(strncmp(image->name, expected->name, strlen(expected->name) + 1) == 0, "expected name \"%s\", but was \"%s\"", expected->name, image->name);
    }
    assert_image_data(image, expected);
}

/* is_printable test (all or nothing) */
START_TEST(test_is_printable) {
    for (char c = -128; c < 32; c++) {
        int res = is_printable(c);
        ck_assert_msg(res == 0, "is_printable should return 0 (false) for c=%d, instead returned %d", c, res);
    }
    for (char c = 32; c < 127; c++) {
        int res = is_printable(c);
        ck_assert_msg(res != 0, "is_printable should return nonzero (true) for c=%d, instead returned %d", c, res);
    }
    int res = is_printable(127);
    ck_assert_msg(res == 0, "is_printable should return 0 (false) for c=%d, instead returned %d", 127, res);
}
END_TEST

/* set_character tests */
START_TEST(test_set_character_NULL) {
    struct ascii_image img = { .width = 5, .height = 5, .name = "test", .data = NULL };
    ck_assert_msg(set_character(NULL, 0, 0, '@') != SUCCESS, "set_character should return FAILURE on a NULL image");
    ck_assert_msg(set_character(&img, 0, 0, '@') != SUCCESS, "set_character should return FAILURE on an image with NULL data");
}
END_TEST

START_TEST(test_set_character_out_of_bounds) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(
            set_character(img, -1, 0, '@') != SUCCESS,
            "set_character should return FAILURE when setting a character in a row %d of a %dx%d image",
            -1, img_copy->width, img_copy->height
            );
    ck_assert_msg(
            set_character(img, 0, -1, '@') != SUCCESS,
            "set_character should return FAILURE when setting a character in a column %d of a %dx%d image",
            -1, img_copy->width, img_copy->height
            );
    ck_assert_msg(
            set_character(img, img_copy->height, 0, '@') != SUCCESS,
            "set_character should return FAILURE when setting a character in a row %d of a %dx%d image",
            img_copy->height, img_copy->width, img_copy->height
            );
    ck_assert_msg(
            set_character(img, 0, img_copy->width, '@') != SUCCESS,
            "set_character should return FAILURE when setting a character in a column %d of a %dx%d image",
            img_copy->width, img_copy->width, img_copy->height
            );
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_set_character_unprintable) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(set_character(img, 0, 0, (char) 0) != SUCCESS, "set_character should return FAILURE if the character value is less than 32");
    ck_assert_msg(set_character(img, 3, 3, (char) 17) != SUCCESS, "set_character should return FAILURE if the character value is less than 32");
    ck_assert_msg(set_character(img, 4, 4, (char) 127) != SUCCESS, "set_character should return FAILURE if the character value is greater than 126");
    ck_assert_msg(set_character(img, 2, 4, (char) 238) != SUCCESS, "set_character should return FAILURE if the coordinate value is greater than 126");
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_set_character_square_first_pixel) {
    int width = 3, height = 3;
    struct ascii_image *img = _create_image(height, width, "test");
    ck_assert_msg(set_character(img, 0, 0, '@') == SUCCESS,
            "set_character should return SUCCESS when setting character (r: %d, c: %d) of a %dx%d image",
            0, 0,
            width, height
            );
    assert_image(img, &(struct ascii_image) {
        .name = "test", .width = width, .height = height,
        .data = "@.." "..." "..."
    });
    _free_image(img);
}
END_TEST

START_TEST(test_set_character_square_last_pixel) {
    int width = 3, height = 3;
    struct ascii_image *img = _create_image(height, width, "test");
    ck_assert_msg(set_character(img, 2, 2, '@') == SUCCESS,
            "set_character should return SUCCESS when setting character (r: %d, c: %d) of a %dx%d image",
            2, 2,
            width, height
            );
    assert_image(img, &(struct ascii_image) {
        .name = "test", .width = width, .height = height,
        .data = "..." "..." "..@"
    });
    _free_image(img);
}
END_TEST

START_TEST(test_set_character_single_column) {
    int width = 1, height = 4;
    struct ascii_image *img = _create_image(height, width, "test");
    ck_assert_msg(set_character(img, 2, 0, '@') == SUCCESS,
            "set_character should return SUCCESS when setting character (r: %d, c: %d) of a %dx%d image",
            2, 0,
            width, height
            );
    assert_image(img, &(struct ascii_image) {
        .name = "test", .width = width, .height = height,
        .data = "..@."
    });
    _free_image(img);
}
END_TEST

START_TEST(test_set_character_rectangle) {
    int width = 4, height = 3;
    struct ascii_image *img = _create_image(height, width, "test");
    ck_assert_msg(set_character(img, 1, 2, '@') == SUCCESS,
            "set_character should return SUCCESS when setting character (r: %d, c: %d) of a %dx%d image",
            1, 2,
            width, height
            );
    assert_image(img, &(struct ascii_image) {
        .name = "test", .width = width, .height = height,
        .data = "...." "..@." "...."
    });
    _free_image(img);
}
END_TEST

START_TEST(test_set_character_basic) {
    struct ascii_image *img = _create_big_image();
    struct ascii_image *img_copy = _create_big_image();
    _set_character(img_copy, 2, 3, '=');
    ck_assert_msg(set_character(img, 2, 3, '=') == SUCCESS,
            "set_character should return SUCCESS when setting character (r: %d, c: %d) of a %dx%d image",
            2, 3,
            _big_image_width, _big_image_height
            );
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

/* draw_diagonal_line tests */

START_TEST(test_draw_diagonal_line_NULL) {
    struct ascii_image img = { .width = 5, .height = 5, .name = "test", .data = NULL };
    ck_assert_msg(draw_diagonal_line(NULL, 0, 0, 1, 1, '@') != SUCCESS, "draw_diagonal_line should return FAILURE on a NULL image");
    ck_assert_msg(draw_diagonal_line(&img, 0, 0, 1, 1, '@') != SUCCESS, "draw_diagonal_line should return FAILURE on an image with NULL data");
}
END_TEST

START_TEST(test_draw_diagonal_line_valid_coords) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(draw_diagonal_line(img, -1, -1, 0, 0, '@') != SUCCESS, "draw_diagonal_line should return FAILURE if a coordinate value is less than 0. (-1, -1) & (0, 0)");
    ck_assert_msg(draw_diagonal_line(img, 0, -1, 0, 0, '@') != SUCCESS, "draw_diagonal_line should return FAILURE if a coordinate value is less than 0. (0, -1) & (0, 0)");
    ck_assert_msg(draw_diagonal_line(img, 0, 0, -1, 0, '@') != SUCCESS, "draw_diagonal_line should return FAILURE if a coordinate value is less than 0. (0, 0) & (-1, 0)");
    ck_assert_msg(draw_diagonal_line(img, 0, 0, 0, -1, '@') != SUCCESS, "draw_diagonal_line should return FAILURE if a coordinate value is less than 0. (0, 0) & (0, -1)");
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_draw_diagonal_line_unprintable) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(draw_diagonal_line(img, 0, 0, 1, 1, (char) 0) != SUCCESS, "draw_diagonal_line should return FAILURE if the character value is less than 32");
    ck_assert_msg(draw_diagonal_line(img, 2, 3, 4, 1, (char) 17) != SUCCESS, "draw_diagonal_line should return FAILURE if the character value is less than 32");
    ck_assert_msg(draw_diagonal_line(img, 4, 1, 2, 3, (char) 127) != SUCCESS, "draw_diagonal_line should return FAILURE if the character value is greater than 126");
    ck_assert_msg(draw_diagonal_line(img, 1, 1, 0, 0, (char) 238) != SUCCESS, "draw_diagonal_line should return FAILURE if the coordinate value is greater than 126");
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_draw_diagonal_line_not_straight) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(draw_diagonal_line(img, 0, 0, 3, 4, '@') != SUCCESS, "draw_diagonal_line should return FAILURE if the path is not straight. Row 0, col 0 to row 3, col 4.");
    ck_assert_msg(draw_diagonal_line(img, 2, 2, 4, 6, '@') != SUCCESS, "draw_diagonal_line should return FAILURE if the path is not straight. Row 2, col 2 to row 4, col 6.");
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_draw_diagonal_line_negative_slope) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(draw_diagonal_line(img, 0, 0, 2, 2, '@') == SUCCESS, "draw_diagonal_line should return SUCCESS if the path is straight. Row 0, col 0 to row 2, col 2.");
    ck_assert_msg(draw_diagonal_line(img, 4, 4, 3, 3, '%') == SUCCESS, "draw_diagonal_line should return SUCCESS if the path is straight. Row 4, col 4 to row 3, col 3.");
    _draw_diagonal_line(img_copy, 0, 0, 2, 2, '@');
    _draw_diagonal_line(img_copy, 4, 4, 3, 3, '%');
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_draw_diagonal_line_positive_slope) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(draw_diagonal_line(img, 0, 2, 2, 0, '@') == SUCCESS, "draw_diagonal_line should return SUCCESS if the path is straight. Row 0, col 2 to row 2, col 0.");
    ck_assert_msg(draw_diagonal_line(img, 4, 0, 2, 2, '%') == SUCCESS, "draw_diagonal_line should return SUCCESS if the path is straight. Row 4, col 0 to row 2, col 2.");
    _draw_diagonal_line(img_copy, 0, 2, 2, 0, '@');
    _draw_diagonal_line(img_copy, 4, 0, 2, 2, '%');
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

START_TEST(test_draw_diagonal_line_comprehensive) {
    struct ascii_image *img = _create_small_image();
    struct ascii_image *img_copy = _create_small_image();
    ck_assert_msg(draw_diagonal_line(img, 0, 0, 4, 4, '@') == SUCCESS, "draw_diagonal_line should return SUCCESS if the path is straight. Row 0, col 0 to row 4, col 4.");
    _draw_diagonal_line(img_copy, 0, 0, 4, 4, '@');
    assert_image(img, img_copy);

    ck_assert_msg(draw_diagonal_line(img, 4, 0, 0, 4, '%') == SUCCESS, "draw_diagonal_line should return SUCCESS if the path is straight. Row 4, col 0 to row 0, col 4.");
    _draw_diagonal_line(img_copy, 4, 0, 0, 4, '%');
    assert_image(img, img_copy);
    _free_image(img);
    _free_image(img_copy);
}
END_TEST

/* destroy_image tests */

START_TEST(test_destroy_image_NULL_image) {
    destroy_image(NULL);
}
END_TEST

START_TEST(test_destroy_image_NULL_name) {
    struct ascii_image *img = _create_image(3, 3, NULL);
    destroy_image(img);
}
END_TEST

START_TEST(test_destroy_image_NULL_data) {
    char *name = "test";
    struct ascii_image *img = _create_image(3, 3, name);
    free(img->data);
    img->data = NULL;
    destroy_image(img);
}
END_TEST

START_TEST(test_destroy_image_full_image) {
    char *name = "test";
    struct ascii_image *img = _create_image(3, 3, name);
    destroy_image(img);
}
END_TEST

/* replace_extension tests */
START_TEST(test_replace_extension_NULL) {
    struct ascii_image *img = _create_image(3, 3, "test.png");
    ck_assert_msg(replace_extension(NULL, ".jpeg") != SUCCESS, "replace_extension should return FAILURE when passed a NULL image");
    ck_assert_msg(replace_extension(img, NULL) != SUCCESS, "replace_extension should return FAILURE when passed a NULL extension");
    ck_assert_msg(replace_extension(NULL, NULL) != SUCCESS, "replace_extension should return FAILURE when passed a NULL image and extension");
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_empty_name) {
    struct ascii_image *img = _create_image(3, 3, ".png");
    char *expected_name = ".gif";
    ck_assert_msg(replace_extension(img, ".gif") == SUCCESS, "replace_extension should return SUCCESS when passed an image with filename with nothing before the extension");
    ck_assert_msg(img->name != NULL, "replace_extension should not set 'image->name' to NULL");
    ck_assert_msg(strncmp(img->name, expected_name, strlen(expected_name) + 1) == 0, "expected name \"%s\", but got name \"%s\"", expected_name, img->name);
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_short_name) {
    struct ascii_image *img = _create_image(3, 3, "a.png");
    char *expected_name = "a.gif";
    ck_assert_msg(replace_extension(img, ".gif") == SUCCESS, "replace_extension should return SUCCESS when passed a valid image");
    ck_assert_msg(img->name != NULL, "replace_extension should not set 'image->name' to NULL");
    ck_assert_msg(strncmp(img->name, expected_name, strlen(expected_name) + 1) == 0, "expected name \"%s\", but got name \"%s\"", expected_name, img->name);
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_empty_extension) {
    struct ascii_image *img = _create_image(3, 3, "main.c");
    char *expected_name = "main";
    ck_assert_msg(replace_extension(img, "") == SUCCESS, "replace_extension should return SUCCESS when passed an empty extension");
    ck_assert_msg(img->name != NULL, "replace_extension should not set 'image->name' to NULL");
    ck_assert_msg(strncmp(img->name, expected_name, strlen(expected_name) + 1) == 0, "expected name \"%s\", but got name \"%s\"", expected_name, img->name);
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_no_extension) {
    struct ascii_image *img = _create_image(3, 3, "cs2110");
    char *expected_name = "cs2110";
    ck_assert_msg(replace_extension(img, ".jpeg") == SUCCESS, "replace_extension should return SUCCESS when passed an image with filename with no extension");
    ck_assert_msg(img->name != NULL, "replace_extension should not set 'image->name' to NULL");
    ck_assert_msg(strncmp(img->name, expected_name, strlen(expected_name) + 1) == 0, "expected name \"%s\", but got name \"%s\"", expected_name, img->name);
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_longer_extension) {
    struct ascii_image *img = _create_image(3, 3, "image.png");
    char *expected_name = "image.jpeg";
    ck_assert_msg(replace_extension(img, ".jpeg") == SUCCESS, "replace_extension should return SUCCESS when passed a valid image");
    ck_assert_msg(img->name != NULL, "replace_extension should not set 'image->name' to NULL");
    ck_assert_msg(strncmp(img->name, expected_name, strlen(expected_name) + 1) == 0, "expected name \"%s\", but got name \"%s\"", expected_name, img->name);
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_multiple_extensions) {
    struct ascii_image *img = _create_image(3, 3, "image.png.tar.gz");
    char *expected_name = "image.png.tar.bz2";
    ck_assert_msg(replace_extension(img, ".bz2") == SUCCESS, "replace_extension should return SUCCESS when passed an image with filename with multiple extensions");
    ck_assert_msg(img->name != NULL, "replace_extension should not set 'image->name' to NULL");
    ck_assert_msg(strncmp(img->name, expected_name, strlen(expected_name) + 1) == 0, "expected name \"%s\", but got name \"%s\"", expected_name, img->name);
    _free_image(img);
}
END_TEST

START_TEST(test_replace_extension_alloc_failure) {
    struct ascii_image *img = _create_image(3, 3, "image.png");
    mallocs_until_fail = 0;
    char *name = img->name;
    ck_assert_msg(replace_extension(img, ".gif") == FAILURE, "replace_extension should return FAILURE when allocation fails");
    ck_assert_msg(img->name == name, "replace_extension should not change 'image->name' when allocation fails");
    _free_image(img);
}
END_TEST

Suite *tl4_suite(void)
{
    Suite *s = suite_create("tl4_suite");

    // is_printable test
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_is_printable);

    // set_character tests
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_NULL);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_out_of_bounds);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_unprintable);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_square_first_pixel);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_square_last_pixel);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_single_column);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_rectangle);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_set_character_basic);

    // draw_diagonal_line tests
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_NULL);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_valid_coords);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_unprintable);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_not_straight);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_negative_slope);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_positive_slope);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_draw_diagonal_line_comprehensive);

    // destroy_image tests
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_destroy_image_NULL_image);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_destroy_image_NULL_name);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_destroy_image_NULL_data);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_destroy_image_full_image);

    // replace_extension tests
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_NULL);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_empty_name);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_short_name);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_empty_extension);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_no_extension);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_longer_extension);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_multiple_extensions);
    tcase_singleton(s, reset_mallocs_until_fail, NULL, test_replace_extension_alloc_failure);

    return s;
}
