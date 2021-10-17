DROP DATABASE IF EXISTS juego;
CREATE DATABASE juego;
USE juego;
CREATE TABLE jugadores (
	nombre VARCHAR(20) PRIMARY KEY NOT NULL,
	contraseña INTEGER NOT NULL
)ENGINE = InnoDB;

CREATE TABLE partidas (
	ID INTEGER PRIMARY KEY NOT NULL AUTO_INCREMENT,
	fecha_hora DATETIME NOT NULL,
	duracion INTEGER NOT NULL,
	ganador TEXT NOT NULL
)ENGINE = InnoDB;


CREATE TABLE puntuacion (
	posicion INTEGER NOT NULL,
	puntos INTEGER NOT NULL,
	jugador VARCHAR(20) NOT NULL,
	partida INTEGER NOT NULL,
	FOREIGN KEY (jugador) REFERENCES jugadores(nombre),
	FOREIGN KEY (partida) REFERENCES partidas(ID)
)ENGINE = InnoDB;

INSERT INTO jugadores VALUES('julen', 1234);
INSERT INTO jugadores VALUES('david', 4321);
INSERT INTO jugadores VALUES('cristina', 5678);

INSERT INTO partidas VALUES(1,'2021-09-30 18:00:00',37,'julen');
INSERT INTO partidas VALUES(2,'2021-10-05 21:00:00',44,'david');
INSERT INTO partidas VALUES(3,'2021-10-07 05:00:00',36,'cristina');
INSERT INTO partidas VALUES(4,'2021-10-10 09:00:00',38,'julen');
INSERT INTO partidas VALUES(5,'2021-10-15 20:32:00',52,'cristina');
INSERT INTO partidas VALUES(6,'2021-10-30 18:53:00',55,'julen');
INSERT INTO partidas VALUES(7,'2021-11-01 13:15:00',100,'david');

INSERT INTO puntuacion VALUES(1,20,'julen',1);
INSERT INTO puntuacion VALUES(3,18,'david',2);
INSERT INTO puntuacion VALUES(2,19,'cristina',3);
INSERT INTO puntuacion VALUES(5,15,'julen',4);
INSERT INTO puntuacion VALUES(4,16,'cristina',5);
INSERT INTO puntuacion VALUES(6,10,'julen',6);
INSERT INTO puntuacion VALUES(7,5,'david',7);

 




