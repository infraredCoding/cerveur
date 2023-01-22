#include "Parameters.h"

static void _print_critical()
{
	printf("============ CRITICAL ============\n");
	printf("Memory could not be allocated for parameters.\n");
}

struct ParameterArray * paramInit(const size_t capacity)
{
	struct ParameterArray * params = malloc(sizeof(struct ParameterArray));

	if (!params)
	{
		_print_critical();
		return NULL;
	}

	params->parameters = calloc(capacity, sizeof(struct Parameter));
	
	if (!params->parameters)
	{
		_print_critical();
		return NULL;
	}

	params->is_sorted = false;
	params->n_members = 0;
	params->capacity = capacity;

	for (size_t i = 0; i < capacity; ++i)
	{
		params->parameters[i].key = NULL;
		params->parameters[i].value = NULL;
	}

	return params;
}

bool paramAdd(
		struct ParameterArray * params,
		const char* key,
		const char* value)
{
	const size_t i = params->n_members;

	params->parameters[i].key = strdup(key);
	params->parameters[i].value = strdup(value);

	params->is_sorted = false;
	params->n_members++;

	// follow doubling rule
	if (params->n_members == params->capacity)
	{
		void* alloc = realloc(params->parameters, params->capacity * 2 * sizeof(struct Parameter));
		if (!alloc)
		{
			_print_critical();
			return false;
		}

		params->capacity *= 2;
		params->parameters = alloc;
	}

	return true;
}

static int key_cmp(const void* a, const void* b)
{
	const struct Parameter * _a = a;
	const struct Parameter * _b = b;
	
	return strcmp(_a->key, _b->key);
}

void paramSort(struct ParameterArray * params)
{
	qsort(params->parameters, params->n_members, sizeof(struct Parameter), &key_cmp);
	params->is_sorted = true;
}

struct Parameter * paramGet(
		struct ParameterArray * params,
		const char* key)
{
	if (!params->is_sorted)
		paramSort(params);

	return bsearch(key, params->parameters, params->n_members, sizeof(struct Parameter), &key_cmp);

}

void paramParse(
		struct ParameterArray * params,
		char* url)
{
	char* url_copy = strdup(url);
	memset(url, 0, strlen(url));

	// temporary large buffers to record the key/values before passing
	// them to paramAdd
	char key[1024] = {0};
	char value[1024] = {0};

	/*
	 * Parsing Rule:
	 * k = keys
	 * v = values
	 */
	char parsing_rule = 0;

	for (size_t i = 0; i < strlen(url_copy); ++i)
	{
		switch (url_copy[i])
		{
			case '?':
				parsing_rule = 'k';
				continue;
			case '=':
				parsing_rule = 'v';
				continue;
			case '&':
				paramAdd(params, key, value);
				memset(key, 0, 1024);
				memset(value, 0, 1024);
				parsing_rule = 'k';
				continue;
			default:
				if (parsing_rule == 0)
					strncat(url, &url_copy[i], 1);
				break;
		}

		switch (parsing_rule)
		{
			case 'k':
				strncat(key, &url_copy[i], 1);
				break;
			case 'v':
				strncat(value, &url_copy[i], 1);
				break;
		}
	}

	paramAdd(params, key, value);
}

void paramClear(struct ParameterArray * params)
{
	for (size_t i = 0; i < params->n_members; ++i)
	{
		free(params->parameters[i].key);
		params->parameters[i].key = NULL;

		free(params->parameters[i].value);
		params->parameters[i].value = NULL;
	}

	params->n_members = 0;
}

void paramFree(struct ParameterArray * params)
{
	paramClear(params);
	free(params);
	params = NULL;
}
