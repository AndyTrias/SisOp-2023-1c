#include <utils/conexiones.h>


int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
	                    server_info->ai_socktype,
	                    server_info->ai_protocol);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}


void inicializar_servidor(char* ip, char* puerto, t_log* logger)
{
	int socket_servidor = crear_servidor(ip, puerto);

	if (socket_servidor == -1) {
		log_error(logger, "No se pudo iniciar el servidor");
		exit(1);
	}

	log_info(logger, "Servidor iniciado en %s:%s", ip, puerto);

	t_list* lista;
	while (1) {
		int socket_cliente = esperar_cliente(socket_servidor);

		if (socket_cliente == -1) {
				log_error(logger, "No se pudo aceptar el cliente");
				exit(1);
		}

		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente, logger);
			break;
		/*case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;*/
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	log_info(logger, "Se conecto un cliente");
}

int inicializar_cliente(char* ip, char* puerto, t_log* logger)
{
	int socket_cliente = crear_conexion(ip, puerto);

	if (socket_cliente == -1) {
		log_error(logger, "No se pudo conectar al servidor %s:%s", ip, puerto);
		return -1;
	}

	log_info(logger, "Se conecto al servidor %s:%s", ip, puerto);

	return socket_cliente;
}

int crear_servidor(char *ip, char* puerto)
{

	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
    struct sockaddr_storage cliente_addr;
    socklen_t addr_size = sizeof(cliente_addr);

    int socket_cliente = accept(socket_servidor, (struct sockaddr *)&cliente_addr, &addr_size);
    if(socket_cliente == -1) {
        return -1;
    }
    return socket_cliente;
}

void terminar_conexiones(int num_sockets, ...) {
  va_list args;
  va_start(args, num_sockets);

  for (int i = 0; i < num_sockets; i++) {
    int socket_fd = va_arg(args, int);
    close(socket_fd);
  }

  va_end(args);
}
