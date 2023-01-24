#include "Parameters.h"

/*
 * Just testing the functionality locally so I don't have to
 * keep running/killing the server over and over.
 *
 * These could be converted to "proper" tests but for now
 * they are purely visual.
 */
int main()
{
	char* url = strdup("/someNested/route?name=Zach&val=100&jimmy=John");
	struct ParameterArray * params = paramInit(10);
	paramParse(params, url);
	printf("PARSED PARAMS:\n\n");
	for (size_t i = 0; i < params->n_members; ++i)
	{
		printf("KEY: %s\n", params->parameters[i].key);
		printf("VALUE: %s\n\n", params->parameters[i].value);
	}
	printf("CLEANED URL: %s\n\n", url);

	struct Parameter* search = NULL;

	printf("SEARCH KEY: name\n");
	search = paramGet(params, "name");
	printf("FOUND VALUE: %s\n\n", search->value);

	printf("SEARCH KEY: val\n");
	search = paramGet(params, "val");
	printf("FOUND VALUE: %s\n\n", search->value);

	printf("SEARCH KEY: xyz\n");
	search = paramGet(params, "xyz");
	if (!search)
		printf("KEY DOESN'T EXIST\n\n");

	paramFree(params);
	free(url);
	return 0;
}
