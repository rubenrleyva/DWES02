
-- Base de datos: `banca_electronica`
--
CREATE DATABASE IF NOT EXISTS `banca_electronica` DEFAULT CHARACTER SET utf8 COLLATE utf8_spanish_ci;
USE `banca_electronica`;

--
-- Estructura de las tablas
--
create table IF NOT EXISTS Usuarios (
 login varchar(10) primary key,
 password varchar(128) not null,
 nombre varchar(30) not null,
 fNacimiento date not null
 );

create table IF NOT EXISTS Movimientos (
 codigoMov varchar(4),
 loginUsu varchar(20),
 fecha date not null,
 concepto varchar(20) not null,
 cantidad float not null,
 primary key (codigoMov, loginUsu),
 foreign key (loginUsu) references Usuarios(login)
);
