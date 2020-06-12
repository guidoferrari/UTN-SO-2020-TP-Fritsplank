#include <stdio.h>
#include <stdlib.h>
#include "Team.h"
#include "./Model/Pokemon.h"
#include "./AppInit/EntrenadoresInit.h"
#include "./AppInit/HilosEntrenadores.h"

int main(int argc, char *argv[]) {

	t_log* logger = iniciar_logger();

	t_config* config = leerConfigDesde("src/team.config");
	//obtiene la lista de entrenadores desde el config
	quickLog("Se arrancan a cargar los entrenadores");
	t_list* entrenadores = getEntrenadoresDesde("src/team.config");
	quickLog("Ya fueron todos los entrenadores cargados con sus posiciones, objetivos y atrapados");

	char* IP_BROKER = config_get_string_value(config, "IP_BROKER");
	int PUERTO_BROKER = config_get_int_value(config, "PUERTO_BROKER");

	int puertoTeam = config_get_int_value(config, "PUERTO_TEAM");

	t_list* objetivosTotales = getObjetivosTotalesDesde(entrenadores);
	t_list* objetivosAtrapados = getTotalAtrapadosDesde(entrenadores);
	t_list* objetivosGlobales = getObjetivosGlobalesDesde(objetivosTotales, objetivosAtrapados);
	log_info(logger, "La cantidad de pokemones objetivos es: %d",list_size(objetivosTotales));
	log_info(logger, "La cantidad de pokemones atrapados es: %d",list_size(objetivosAtrapados));
	log_info(logger, "La cantidad de pokemones globales que faltan por atrapar es: %d",list_size(objetivosGlobales));

//	int conexionGameBoy = crearSocket();
//
//	//TODO: Si muere la conexion del broker, escucho al GameBoy. Y intentar reconexion del Broker varias veces
//	if (escuchaEn(conexionGameBoy, puertoTeam)) {
//		quickLog("Escuchando conexiones del GameBoy");
//	}
//
//	//se queda bloqueado esperando que el gameboy se conecte
//	int socketGameBoy = aceptarConexion(conexionGameBoy);
//
//	id_proceso idProcesoConectado;
//	idProcesoConectado = iniciarHandshake(socketGameBoy, TEAM);
//	log_info(logger, "El id del proceso con el que me conecte es: %d",
//			idProcesoConectado);

	int conexionBroker = conectarABroker(IP_BROKER, PUERTO_BROKER);

	int suscripcionAppeared = crearSocket();
	int suscripcionCaught = crearSocket();
	int suscripcionLocalized = crearSocket();
	int socketGet = crearSocket();
	int socketCatch = crearSocket();

	if (conectarA(suscripcionAppeared, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de appeared_pokemon");
	}
	if (conectarA(suscripcionCaught, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de caught_pokemon");
	}
	if (conectarA(suscripcionLocalized, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Suscripto a la cola de localized_pokemon");
	}
	if (conectarA(socketGet, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de get para poder enviarle mensajes");
	}
	if (conectarA(socketCatch, IP_BROKER, PUERTO_BROKER)) {
		quickLog("Ya se conecto a la cola de catch para poder enviarle mensajes");
	}

	//ENVIA MSJ GET CON LA LISTA DE LOS OBJETIVOS GLOBALES
	//DENTRO DE SELECTS
	enviarGetDesde(objetivosGlobales, socketGet);
	quickLog("Se enviaron los mensajes get por cada pokemon objetivo");

	t_list* pokemonesLibres = list_create();
//	//verificar que el id como respuesta vuelva a enviarse a traves de ese socket
//
//	//recibe los nombres de pokemones encontrados libres con sus posiciones
//	//y si se necesitan (estan en los objetivos globales) se agregan a la lista de pokemones libres
	//recibirLocalizedYGuardalos(suscripcionLocalized, objetivosGlobales, pokemonesLibres);
	recibirAppearedYGuardarlos(suscripcionAppeared, objetivosGlobales, pokemonesLibres);

	crearHilosDeEntrenadores(entrenadores);
	quickLog("Se crea un hilo por cada entrenador");


	//probando las funciones del planificador
//	PokemonEnElMapa* pokemonDePrueba1 = newPokemon();
//	t_posicion posicionDePrueba1;
//	posicionDePrueba1.posicionX = 1;
//	posicionDePrueba1.posicionY = 1;
//	setPosicionTo(pokemonDePrueba1, posicionDePrueba1);
//	setNombreTo(pokemonDePrueba1, "Delficapa");
//	list_add(pokemonesLibres, pokemonDePrueba1);
//	pasarDeNewAReady(entrenadores, pokemonesLibres);
//	Entrenador* entrenador1 = list_get(entrenadores, 0);
//	log_info(logger, "El estado del primer entrenador tendria que pasar de new a ready y se muestra : %d", entrenador1->estado);
//	//


	list_destroy_and_destroy_elements(entrenadores, free);

	return 0;
}


void terminar_programa(int conexion, t_log* logger, t_config* config) {
	if (logger != NULL) {
		log_destroy(logger);
	}

	if (config != NULL) {
		config_destroy(config); //destruye la esctructura de config en memoria, no elimina el archivo de config
	}

	liberar_conexion(conexion);

}
