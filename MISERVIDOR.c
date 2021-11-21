#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>

typedef struct {
	char nombre[20];
	int socket;
	int conectado;
}Conectado;

typedef struct {
	Conectado conectados[100];
	int num;
	int current;
}ListaConectados;

typedef struct {
	Conectado conectados[4];
	int num;
	int current;
	int status;
}Partida;

typedef struct {
	Partida partidas[100];
	int num;
	int totales;
}ListaPartidas;


ListaConectados milista;
ListaPartidas lpartidas;

int i;
int sockets[100];


int Pon (ListaConectados *lista, char nombre[20],int socket)
{
	if(lista->num == 100)
		return -1;
	else{
		strcpy(lista->conectados[lista->num].nombre, nombre);
		lista->conectados[lista->num].socket = socket;		
		lista->num++;
		return 0;
	}	
}


int DamePosicion (ListaConectados *lista, char nombre[20]){
	int i = 0;
	int encontrado = 0;
	while((i < lista->num) && !encontrado)
	{
		if( strcmp(lista->conectados[i].nombre,nombre) == 0){
			encontrado = 1;
		}
		if ( !encontrado){
			
			i=i+1;
		}	
	}
	if (encontrado)
		return i;
	else
		return -1;
}


int Eliminar(ListaConectados *lista,char nombre[20])
{
	int pos = DamePosicion(lista,nombre);
	if(pos == -1)
		return -1;
	else{
		int i = 0;
		for(i = pos; i < (lista->num-1);i++)
		{
			lista->conectados[i] = lista->conectados[i+1];
			strcpy(lista->conectados[i].nombre, lista->conectados[i+1].nombre);
			lista->conectados[i].socket = lista->conectados[i+1].socket;
		}
		lista->num--;
		return 0;
	}	
}


void DameConectados (ListaConectados *lista, char conectados[300]) {
	//sprintf(conectados,"%d",lista->num);
	int i;
	for(i=0;i<lista->num;i++)
	{
		sprintf(conectados,"%s-%s", conectados, lista->conectados[i].nombre);
	}	
}



int DameSocket (ListaConectados *lista, char nombre [20])
{ //Devuelve el socket o -1 si no esta en la lista
	int i = 0;
	int encontrado =0;
	while ((i<lista->num)&&(encontrado == 0))
	{
		if (strcmp(lista->conectados[i].nombre, nombre) == 0)
		{
			encontrado = 1;
			return lista->conectados[i].socket;
		}
		i++;
	}
	if (!encontrado)
		return -1;
}


void *AtenderCliente (void *socket)
{
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn = *s;
	
	char buff[512];
	char buff2[512];
	char buff3[512];
	int ret;
	
	
	int terminar = 0;
	while(terminar==0){
		// Ahora recibimos su nombre, que dejamos en buff
		ret=read(sock_conn,buff, sizeof(buff));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		buff[ret]='\0';
		
		//Escribimos el nombre en la consola
		
		printf ("Se ha conectado: %s\n",buff);
		
		
		char *p = strtok( buff, "/");
		int codigo =  atoi (p);
		char nombre[20];
		char us[20];
		if (codigo == 7){
			p = strtok( NULL, "/");
			char nombre[20];
			strcpy(nombre,p);
			Eliminar(&milista,nombre);
			sprintf(buff2,"%s","7/i");
			write (sock_conn,buff2, strlen(buff2));
		}

		if (codigo == 6){
			DameConectados(&milista,buff2);
			//write (sock_conn,buff2, strlen(buff2));
		}

		if((codigo == 1) ||(codigo == 2)){
			p = strtok( NULL, "/");
			char nombre[20];
			strcpy (nombre, p);
			strcpy (us, nombre);
			p = strtok( NULL, "/");
			char contrasena[20];
			strcpy(contrasena,p);
			printf ("Codigo: %d, Nombre: %s, Contraseña: %s\n", codigo, nombre, contrasena);
			
			if (codigo == 1) //CODIGO DE ACCESO
			{
				printf("hola");
				MYSQL *conn;
				int err;
				
				// Estructura especial para almacenar resultados de consultas
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				
				int ID;
				char consulta [80];
				char consulta2 [80];
				char fecha[20];
				char hora[20];
				//Creamos una conexion al servidor MYSQL
				conn = mysql_init(NULL);
				if (conn==NULL) 
				{
					printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				//inicializar la conexion, indicando nuestras claves de acceso al servidor de BBDD
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "M10juego", 0, NULL,0);
				if (conn==NULL)
				{
					printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}	
				sprintf (consulta2,"SELECT nombre FROM jugadores WHERE nombre ='%s' and contraseña = '%s' ",nombre,contrasena);
				
				err=mysql_query (conn, consulta2);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				
				//recogemos el resultado de la consulta
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL){
					
					printf ("No se han obtenido datos en la consulta\n");
					strcpy(buff2,"1/NO,");
					write (sock_conn,buff2, strlen(buff2));
					
				}
				else{
					while(row!=NULL){
						// El resultado debe ser una matriz con una sola fila
						// y una columna que contiene el nombre
						//printf ("El ganador es: %s\n", row[3] );
						printf ("El usuario es %s\n", row[0]);
						row = mysql_fetch_row (resultado);	
					}

					strcpy(buff2,"1/SI,");
					Pon(&milista,nombre,sock_conn);
					write (sock_conn,buff2, strlen(buff2));
					printf("%s",buff2);	
				}	
			}	
			
			//sprintf (buff2,"%d,",strlen (nombre));
			
			
			else if(codigo == 2) //CODIGO DE REGISTRO
			{
				MYSQL *conn;
				int err;
				
				// Estructura especial para almacenar resultados de consultas
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				
				int ID;
				char consulta1 [80];
				char consulta2 [80];
				char fecha[20];
				char hora[20];
				//Creamos una conexion al servidor MYSQL
				conn = mysql_init(NULL);
				if (conn==NULL) 
				{
					printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				//inicializar la conexion, indicando nuestras claves de acceso al servidor de BBDD
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "M10juego", 0, NULL,0);
				if (conn==NULL)
				{
					printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}	
				sprintf (consulta1,"SELECT nombre FROM jugadores WHERE nombre ='%s' and contraseña = '%s' ",nombre,contrasena);
				sprintf (consulta2,"INSERT INTO jugadores values('%s','%s') ",nombre,contrasena);
				
				err=mysql_query (conn, consulta1);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				
				//recogemos el resultado de la consulta
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL){
					
					mysql_query (conn, consulta2);
					printf ("Registrado\n");
					strcpy(buff2,"2/SI,");
					write (sock_conn,buff2, strlen(buff2));	
				}
				else{
					strcpy(buff2,"2/NO,");
					write (sock_conn,buff2, strlen(buff2));
				}
			}	
		}


		else if((codigo == 4) || (codigo ==5)){
			p = strtok( NULL, "/");
			char nombre[20];
			strcpy (nombre, p);
			
			
			if (codigo == 4) //CODIGO CONSULTA PUNTOS TOTALES JUGADOR
			{
				printf("%s",buff);
				MYSQL *conn;
				int err;
				
				// Estructura especial para almacenar resultados de consultas
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				//char nombre[20];
				int ID;
				char consulta4 [80];
				//Creamos una conexion al servidor MYSQL
				conn = mysql_init(NULL);
				if (conn==NULL) 
				{
					printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				//inicializar la conexion, indicando nuestras claves de acceso al servidor de BBDD
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "M10juego", 0, NULL,0);
				if (conn==NULL)
				{
					printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				
				
				// Ahora vamos a buscar el nombre de la persona cuyo nombre es uno
				// dado por el usuario
				printf ("Dame el nombre del usuario:\n");
				
				// construimos la consulta SQL
				strcpy (consulta4,"SELECT SUM(puntuacion.puntos) FROM jugadores,puntuacion WHERE jugadores.nombre = '");
				strcat (consulta4, nombre);
				strcat (consulta4, "'AND jugadores.nombre = puntuacion.jugador");
				
				// hacemos la consulta
				err=mysql_query (conn, consulta4);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				
				//recogemos el resultado de la consulta
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL){
					printf ("No se han obtenido datos en la consulta\n");
					strcpy(buff2,"4/NO,");
					write (sock_conn,buff2, strlen(buff2));
				}
				else{
					// El resultado debe ser una matriz con una sola fila
					// y una columna que contiene el nombre
					printf ("La suma total de puntos es: %s\n", row[0] );
					sprintf(buff2,"4/%s",row[0]);
					write (sock_conn,buff2, strlen(buff2));
					mysql_close (conn);	
				}
				// cerrar la conexion con el servidor MYSQL
			}
			
			
			else if (codigo == 5) //CODIGO CONSULTA FECHA PARTIDAS DE UN JUGADOR
			{
				MYSQL *conn;
				int err;
				
				// Estructura especial para almacenar resultados de consultas
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				
				int ID;
				char consulta5 [80];
				//Creamos una conexion al servidor MYSQL
				conn = mysql_init(NULL);
				if (conn==NULL) 
				{
					printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				//inicializar la conexion, indicando nuestras claves de acceso al servidor de BBDD
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "M10juego", 0, NULL,0);
				if (conn==NULL)
				{
					printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				
				
				// Ahora vamos a buscar el nombre de la persona cuyo nombre es uno
				// dado por el usuario
				
				// construimos la consulta SQL
				strcpy (consulta5,"SELECT partidas.fecha_hora FROM jugadores,partidas WHERE jugadores.nombre = '");
				strcat (consulta5, nombre);
				strcat (consulta5, "'AND jugadores.nombre = partidas.ganador");
				
				// hacemos la consulta
				err=mysql_query (conn, consulta5);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					
					exit (1);
				}
				
				//recogemos el resultado de la consulta
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				
				if (row == NULL)
					printf ("No se han obtenido datos en la consulta\n");
				else{
					char salida[500];
					salida[0]='\0';
					while (row !=NULL) {
						// El resultado debe ser una matriz con una sola fila
						// y una columna que contiene el nombre
						//sprintf(salida,"%s %s",salida,row[0]);
						printf("contenido salida:%s\n",salida);
						printf("row0:%s\n",row[0]);
						sprintf(salida,"%s %s ",salida,row[0]);
						printf("nuevo valor de salida:%s\n",salida);
						printf ("%s jugo una partida el dia y hora: %s\n",nombre, row[0]);
						row = mysql_fetch_row (resultado);
					}
					printf("La salida definitiva es:%s\n",salida);
					sprintf(buff3,"5/%s",salida);
					printf("Envio la siguiente respuesta:%s\n",buff3);
					write (sock_conn,buff3, strlen(buff3));
					
				}
				
				// cerrar la conexion con el servidor MYSQL
				mysql_close (conn);
				//exit(0);
				
				
			}
		}
		else if (codigo == 3) //CODIGO CONSULTA GANADOR PARTIDA SEGUN FECHA
		{
			p = strtok( NULL, "/");
			char fechayhora[60];
			strcpy (fechayhora, p);
			
			MYSQL *conn;
			int err;
			
			
			// Estructura especial para almacenar resultados de consultas
			MYSQL_RES *resultado;
			MYSQL_ROW row;
			
			int ID;
			char consulta [200];
			
			//Creamos una conexion al servidor MYSQL
			conn = mysql_init(NULL);
			if (conn==NULL) 
			{
				printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
				
				exit (1);
			}
			//inicializar la conexion, indicando nuestras claves de acceso al servidor de BBDD
			
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "M10juego", 0, NULL,0);
			
			if (conn==NULL)
			{
				printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
				
				exit (1);
			}
			
			
			// Ahora vamos a buscar el nombre de la persona cuyo nombre es uno
			// dado por el usuario
			
			
			// construimos la consulta SQL
			strcpy (consulta,"SELECT * FROM partidas WHERE fecha_hora = '");
			strcat (consulta, fechayhora);
			
			strcat (consulta, "'");	
			printf("%s",consulta);
			// hacemos la consulta
			err=mysql_query (conn, consulta);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				
				exit (1);
			}
			
			//recogemos el resultado de la consulta
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row (resultado);
			
			if (row == NULL)
				printf ("No se han obtenido datos en la consulta\n");
			else{
				char salida[50];
				salida[0]='\0';
				while(row!=NULL){
					
					// El resultado debe ser una matriz con una sola fila
					// y una columna que contiene el nombre
					printf ("El ganador es: %s\n", row[3] );
					sprintf(salida,"%s %s",salida,row[3]);
					
					row = mysql_fetch_row (resultado);
				}
				sprintf(buff2,"3/%s",salida);
				write (sock_conn,buff2, strlen(buff2));
			}
			// cerrar la conexion con el servidor MYSQL
			mysql_close (conn);
			//exit(0);
			
			
		}


		else if (codigo == 9)
			// cuando el cliente invita a otra persona
		{
			char invitado [20];
			p = strtok( NULL, "/");
			strcpy (invitado, p);
			int socket_invitado;
			socket_invitado = DameSocket(&milista,invitado);
			printf ("Voy a invitar a %d, %s \n", socket_invitado, invitado);
			strcpy(buff2,"");
			printf("Estamos listos %s  \n", buff2);
			sprintf (buff2,"9/%s",us);
			
			printf("Estamos listos2 %s  \n", buff2);
			write (socket_invitado,buff2, strlen(buff2));
			//printf("estem aqui 3 %s\n",respuesta);
			strcpy(buff2,"");
			
		}
		else if (codigo == 8)
			// cuando invitan al cliente
		{
			char nombre1[20];
			p = strtok(NULL,"/");
			char invitador [20];
			//p = strtok( NULL, "/");
			strcpy (invitador, p);
			p = strtok( NULL, "/");
			char eleccion[10];
			strcpy(eleccion,p);
			
			int socket_invitador;
			socket_invitador = DameSocket(&milista,invitador);
			
			sprintf (buff2,"8/%s",eleccion);
			
			write (socket_invitador,buff2, strlen(buff2));
			printf("%s\n",buff2);
			strcpy(buff2,"");
		}
		


		if (codigo!=0)
		{
			char notificacion[300];
			
			
			DameConectados(&milista,notificacion);
			int j;
			char aux1[300];
			sprintf(aux1,"6/%s",notificacion);
			
			for(j=0;j< i;j++){
				write (sockets[j],aux1, strlen(aux1));
			}
			sprintf(aux1,"%s","");
			sprintf(notificacion,"%s","");
					
					
		}
	}
	close(sock_conn);
}



int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9000);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	
	
	
	
	
	
	
	
	
	
	pthread_t thread[100];
	int terminar2 = 0;
	
    for(i=0;terminar2 == 0;i++){
	printf ("Escuchando\n");
	
	sock_conn = accept(sock_listen, NULL, NULL);
	printf ("He recibido conexi?n\n");
	sockets[i] = sock_conn;
	char nombre[20];
	//sock_conn es el socket que usaremos para este cliente
	pthread_create(&thread[i], NULL, AtenderCliente,&sockets[i]);
	
		
	}
	//close(sock_conn); 
	
}
