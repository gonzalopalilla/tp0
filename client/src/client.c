#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente


	// Escribi: "Hola! Soy un log"

	log_info(logger,"Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	valor = config_get_string_value(config,"CLAVE");

	// Loggeamos el valor de config

	log_info(logger, "Se obtiene desde el archivo config el valor de la clave: %s, el valor de la IP: %s y el valor del puerto: %s", valor, ip, puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "EjemploLogueo", true, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL) {
        printf("Error al crear el archivo de log.\n");
        exit(EXIT_FAILURE);
    }

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("../cliente.config");

	if (nuevo_config == NULL) {
        printf("Error al crear el config desde el archivo cliente.config.\n");
        exit(EXIT_FAILURE);
    }


	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger," %s",leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


while (1) {
    leido = readline("> ");

    if (!leido) {  
        break;  // Si readline devuelve NULL, salimos del bucle
    }

    if (strlen(leido) == 0) {  
        free(leido);  // Si se presiona solo Enter, liberamos memoria y salimos
        break;
    }

    log_info(logger, leido);  // Se loguea solo si hay contenido

    if (!strncmp(leido, "exit", 4)) {  
        free(leido);
        break;
    }

    free(leido);  // ¡Liberar memoria después de usarla!
}

}


void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	leido = readline("> ");
	
	 while (strcmp(leido,"") != 0){

		agregar_a_paquete(paquete,leido,strlen(leido) + 1);
		free(leido);
		leido = readline("> ");

		}

		free(leido);

		//Envio Paquete

		enviar_paquete(paquete,conexion);

		//Libero Paquete

		eliminar_paquete(paquete);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
