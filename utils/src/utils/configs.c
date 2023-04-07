#include "configs.h"

t_config* iniciar_config(char* path)
{
	t_config* nuevo_config;

	nuevo_config = config_create(path);

	if (!nuevo_config) {
		printf("Error al crear la config\n");
		exit(2);
	}

	return nuevo_config;
}