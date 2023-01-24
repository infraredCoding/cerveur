#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * Implementation notes: (22 Jan 2023)
 *
 * Parameters will be stored as an array sorted on-demand.
 * If calling paramGet and it's unsorted, it will sort automatically.
 * If calling paramGet and it's already sorted, it doesn't touch it.
 * Adding a new parameter will toggle the is_sorted to false.
 *
 * I was considering using the tree implementation of Route but I think
 * it would make more sense to move the logic into its own data structure
 * that can be used across different components (like Route & Parameter)
 * because otherwise it would be confusing if we had a Route struct inside
 * of a Parameter. For now, I'll use this array structure.
 *
 */

struct Parameter
{
	char* key;
	char* value;
};

struct ParameterArray
{
	struct Parameter * parameters;
	bool is_sorted;
	size_t n_members;
	size_t capacity;
};

// Initialise array of parameters with capacity.
// Returns pointer to array on success, NULL on failure
struct ParameterArray * paramInit(const size_t capacity);

// Parse parameters from a string (e.g., when receiving ROUTE url).
// Takes an existing Parameter pointer and adds new parameters parsed from string.
// This will MODIFY the url inplace to remove all the parameter text before it's passed
// on to render a static template.
void paramParse(
		struct ParameterArray * params, 
		char* url);

// Clear all keys & values and set n_members back to zero.
// This allows us to reuse the struct for multiple requests.
void paramClear(struct ParameterArray * params);

// Add new parameter to parameter array.
// Returns true on success and false on failure (e.g., memory allocation)
bool paramAdd(
		struct ParameterArray * params,
		const char* key,
		const char* value);

// Sort parameters by keys ascending. This is primarily used internally
// but can be used externally if needed for whatever reason.
void paramSort(struct ParameterArray * params);

// Return a pointer to a parameter with a given key.
// Any modifications done will be reflected in parameter array.
struct Parameter * paramGet(
		struct ParameterArray * params,
		const char* key);

// Free all memory consumed by parameters
void paramFree(struct ParameterArray * params);

#endif
