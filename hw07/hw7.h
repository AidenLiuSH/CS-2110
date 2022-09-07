// DO NOT MODIFY THIS FILE

/**
 * @brief Header file for global macros, structures and fields to be used by the
 * hw7 program.
 */
#ifndef HW7_H
#define HW7_H

#include <stddef.h>

#define UNUSED_PARAM(x) ((void) x) // This macro is only used for turning off compiler errors initially
#define UNUSED_FUNC(x) ((void) x)  // This macro is only used for turning off compiler errors initially

// Success and failure codes for function return
#define SUCCESS 1
#define FAILURE 0

// Sizes for different arrays
#define MAX_POKEDEX_SIZE  64
#define MIN_NICKNAME_SIZE   3
#define MAX_NICKNAME_SIZE   10
#define MAX_SPECIESNAME_SIZE  12
#define MIN_SPECIESNAME_SIZE  4

// pokemon struct
struct pokemon {
    char    nickname[MAX_NICKNAME_SIZE];
    int     pokedexNumber;
    double  powerLevel;
    char    speciesName[MAX_SPECIESNAME_SIZE];
};
extern struct pokemon pokedex[MAX_POKEDEX_SIZE];
extern int size;
// Prototype hw7.c functions
int catchPokemon(const char *, int, double, const char *);
int updatePokemonNickname(struct pokemon, const char *);
int swapPokemon(int, int);
int releasePokemon(struct pokemon);
int comparePokemon(struct pokemon, struct pokemon);
void sortPokemon(void);

void printPokedex(void);

#endif
