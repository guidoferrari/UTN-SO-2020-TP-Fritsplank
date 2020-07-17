
#ifndef BROKER_H_
#define BROKER_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/collections/list.h>
#include<utils.h>


pthread_t escucharACKAppearedPokemon;
pthread_t escucharACKLocalizedPokemon;
pthread_t escucharACKCaughtPokemon;


t_log* iniciar_logger(void);
t_config* leer_config(void);
void terminar_programa(t_config*);
void iniciar_servidor(void);
void liberarConexion(int socket_cliente);

#endif /* BROKER_H_ */
