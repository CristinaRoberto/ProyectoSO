#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>






int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char buff[512];
	char buff2[512];
	char buff3[512];
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
	
	int i;
	// Atenderemos solo 5 peticione
    for(;;){
	printf ("Escuchando\n");
	
	sock_conn = accept(sock_listen, NULL, NULL);
	printf ("He recibido conexi?n\n");
	//sock_conn es el socket que usaremos para este cliente
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
	if((codigo == 1) ||(codigo == 2)){
	p = strtok( NULL, "/");
	char nombre[20];
	strcpy (nombre, p);
	p = strtok( NULL, "/");
	char contrasena[20];
	strcpy(contrasena,p);
	
	printf ("Codigo: %d, Nombre: %s, Contraseña: %s\n", codigo, nombre, contrasena);
	
	if (codigo == 1)//Codigo de acceso
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
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "juego", 0, NULL,0);
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
			strcpy(buff2,"NO,");
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
			strcpy(buff2,"SI,");
			write (sock_conn,buff2, strlen(buff2));
		}
			
			
	
	}	
		
		//sprintf (buff2,"%d,",strlen (nombre));
	
	
	
	else if(codigo == 2){
		// Código de registro
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
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "juego", 0, NULL,0);
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
			strcpy(buff2,"SI,");
			write (sock_conn,buff2, strlen(buff2));
			
		}
		else{
			
			strcpy(buff2,"NO,");
			write (sock_conn,buff2, strlen(buff2));
		}
		
		
		
	}	
	}
	else if((codigo == 4) || (codigo ==5)){
		p = strtok( NULL, "/");
		char nombre[20];
		strcpy (nombre, p);
		
	 
		
		if (codigo == 4)
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
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "juego", 0, NULL,0);
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
			strcpy(buff2,"NO,");
			write (sock_conn,buff2, strlen(buff2));
		}
		else{
			// El resultado debe ser una matriz con una sola fila
			// y una columna que contiene el nombre
			printf ("La suma total de puntos es: %s\n", row[0] );
			sprintf(buff2,"%s",row[0]);
			write (sock_conn,buff2, strlen(buff2));
			mysql_close (conn);
			
			
		}
		
		
		// cerrar la conexion con el servidor MYSQL
		
		
		
	}
	
	
	else if (codigo == 5)
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
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "juego", 0, NULL,0);
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
			while (row !=NULL) {
				// El resultado debe ser una matriz con una sola fila
				// y una columna que contiene el nombre
				//sprintf(salida,"%s %s",salida,row[0]);
				sprintf(salida,"%s ",row[0]);
				printf ("%s jugo una partida el dia y hora: %s\n",nombre, row[0]);
				row = mysql_fetch_row (resultado);
		}
			printf("%s",buff3);
			sprintf(buff3,"%s",salida);
			write (sock_conn,buff3, strlen(buff3));
			
		}
			
			// cerrar la conexion con el servidor MYSQL
			mysql_close (conn);
			exit(0);
		
		
	}
	}
	else if (codigo == 3)
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
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "juego", 0, NULL,0);
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
			while(row!=NULL){
				
				// El resultado debe ser una matriz con una sola fila
				// y una columna que contiene el nombre
				printf ("El ganador es: %s\n", row[3] );
				sprintf(buff2,"%s",row[3]);
				
				row = mysql_fetch_row (resultado);
			}
			write (sock_conn,buff2, strlen(buff2));
		}
		// cerrar la conexion con el servidor MYSQL
		mysql_close (conn);
		exit(0);
		
		
	}
	}
		// Y lo enviamos
		
	
		// Se acabo el servicio para este cliente
		
		}
	close(sock_conn); 
	
}

