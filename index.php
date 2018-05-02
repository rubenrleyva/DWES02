<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "
http://www.w3.org/TR/html4/loose.dtd">
<!-- Desarrollo Web en Entorno Servidor -->
<!-- Unidad 2 : Trabajar con bases de datos en PHP -->
<!-- Autor: Rubén Ángel Rodriguez Leyva -->

<html>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=UTF-8">
        <title>DWES - Tarea 2 - Forrare Bank</title>
        <link/>
        
    </head>
    <body>
        
        <?php
        
        // Se añade el archivo donde se encuentran todas las funciones.
        include 'funciones.inc';
        
        // Función que contiene la cabecera web (espero mejorarla con el tiempo).
        cabecera("Unidad 2 - Trabajar con bases de datos en PHP", "FORRARE BANK");
        
        // Establecemos la zona horaria
        date_default_timezone_set('europe/Madrid');
        
        // Estado en el que que se encuentra el menú principal.
        $estado = true;
        
        // Modo en el que se encontrará el menú administrador o usuario.
        $modoMenu = "";
                
        // Iniciamos algunas variables que se utilizarán para los usuarios.
        $login = ""; // Para el login
        $password =""; // Para el password.
        $passwordRepe=""; // Para comprobar el password.
        $nombre =""; // Para el concepto.
        $fnacimiento = ""; // Para la cantidad.
        $errores = ""; // Para los errores que se produzcan.

        // Iniciamos algunas variables que se utilizarán en los movimientos.
        $fecha = ""; // Para la fecha
        $concepto =""; // Para el concepto.
        $cantidad = ""; // Para la cantidad.
        
        
        // Comenzamos la conexión
        //conexion();

        // En caso de pulsar un botón
        if (isset($_REQUEST['eleccion'])) {
            
            // le pasamos a la variable elección el nombre del botón pulsado.
            $eleccion = $_REQUEST['eleccion'];
            
            // A través de un switch al que le pasamos la elección escogemos la opción que corresponda.
            switch ($eleccion) {
                
                /**
                 * +++++++++++++++++++++++++++++++++++++++++
                 * OPCIONES PARA EL ADMINISTRADOR Y USUARIOS
                 * +++++++++++++++++++++++++++++++++++++++++
                 */
                
                // Con está opción activamos el login para el administrador
                case "Gestionar Usuarios":
                    
                    // Activamos el menú principal encima
                    principal(true);
                    
                    // Modo en el que se encontrará el menú cuando entremos en administrador
                    $modoMenu = "modoAdministrador";
                    
                    // Le pasamos la función que contiene el formulario para el usuario
                    // tiene como argumentos, el nombre del tipo de login, y 
                    // si se va a borrar un usuario
                    introduccionUsuario("Login Administrador", $modoMenu, "Iniciar sesión");
                    
                    // Desactivamos el menú principal debajo.
                    $estado = false;
                    break;
                
                // Con está opción activamos el login para el resto de usuarios
                case "Gestionar mi cuenta":
                    
                    // Activamos el menú principal
                    principal(true);
                    
                    // Modo en el que se encontrará el menú cuando entremos en usuarios
                    $modoMenu = "modoUsuario";
                    
                    // Le pasamos la función que contiene el formulario para el usuario
                    // tiene como argumentos, el nombre del tipo de login, y 
                    // si se va a borrar un usuario
                    introduccionUsuario("Login Usuario", $modoMenu, "Iniciar sesión");
                    
                    // Desactivamos el menú principal debajo.
                    $estado = false;
                    break;
                 
                // Si pulsamos sobre salir volveremos al menú principal
                case "Salir";
                
                    // Se habilita el menú principal
                    $estado = true;
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    break;
                               
                // Con esta iniciamos la sesión del administrador o usuario
                case "Iniciar sesión":
                    
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Recogemos la variable login
                        $login = $_REQUEST['login'];
                    
                        // Recogemos la variable password
                        $password = $_REQUEST['password'];
                    
                        // Recogemos la variable modoMenu
                        $modoMenu = $_REQUEST['modoMenu'];
                    
                        // Activamos el menú principal debajo.
                        $estado = true;
                    
                        // En caso de que sea el administrador
                        if($modoMenu == "modoAdministrador"){
                        
                            // Para comprobar que es el administrador en la comprobación
                            $administrador = true;
                        
                            // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                            $buscarUsuarios = false;
                        
                            // Comprobamos que existe el administrador en el código (en este caso no en la base de datos).
                            if(comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password)){
                            
                                // Desactivamos el menú principal debajo.
                                $estado = false;
                            
                                // Si exite el administrador entramos en menú de opciones
                                funcionesAdministrador();
                            
                            // en caso contrario avisamos por pantalla    
                            }else{       
                                echo "<center><b><h2>Error: El usuario administrador no es correcto</h2></b></center>";
                            }  
                            
                        // en caso contrario será un usuario
                        }else if($modoMenu == "modoUsuario"){

                            // Para comprobar que no es el administrador en la comprobación
                            $administrador = false;
                        
                            // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                            $buscarUsuarios = true;
                        
                            // Comprobamos si el usuario existe en la base de datos
                            if(comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password)){
                            
                                // Desactivamos el menú principal debajo.
                                $estado = false;
                            
                                // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                                $buscarUsuarios = false;
                            
                                // Buscamos el nombre completo del usuario.
                                $nombreCabecera = comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password);
                        
                                // Le pasamos al formulario de usuario el nombre completo y el login para poder realizar
                                // más tareas en otras funciones que requieren del login.
                                funcionesUsuario($nombreCabecera, $login);

                            // en caso contrario avisamos por pantalla    
                            }else{ 
                                echo "<center><b><h2>Error: El usuario introducido no es correcto</h2></b></center>";
                            } 
                        }  
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                    
                    
                /**
                 * ++++++++++++++++++++++++++++++
                 * OPCIONES PARA EL ADMINISTRADOR
                 * ++++++++++++++++++++++++++++++
                 */
                
                // Con esta opción creamos un nuevo usuario
                case "Nuevo Usuario":
                    
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Escondemos el menú principal
                        $estado = false;
                    
                        // Dejamos visible el menú del administrador
                        funcionesAdministrador();
                    
                        // Le ponemos nombre al botón
                        $boton = "Aceptar nuevo";
                            
                        // Indicamos que es un usuario nuevo
                        $tipo = "Nuevo Usuario";
                            
                        // Indicamos si es una modificación del usuario o no
                        $modificacion = false;
                            
                        // Indicamos si se debe de mostrar para borrar
                        $borrar = false;
                    
                        // Llamamos de nuevo a la función indicando los errores que hay y pasándole la
                        // la información ya introducida anteriormente.
                        cambiosUsuario($dwes, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion);
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Al pulsar sobre esta opción verificamos lo datos para crear un nuevo usuario
                case "Aceptar nuevo":

                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Escondemos el menú principal
                        $estado = false;
                    
                        // Dejamos visible el menú del administrador
                        funcionesAdministrador();
                    
                        // Recogemos las distintas variables que serán utilizadas
                        $login = $_REQUEST['login']; // Para el login
                        $password = $_REQUEST['password']; // Para el password
                        $passwordRepe = $_REQUEST['passwordRepe']; // Para la comprobación del password
                        $nombre = $_REQUEST['nombre']; // Para el nombre completo
                        $fnacimiento = $_REQUEST['fnacimiento']; // Para la fecha de nacimiento
                                               
                        // Cambiamos el formato de la fecha para que concuerde con la base de datos en caso de no ser
                        // ni firefox ni chrome
                        if($fnacimiento != null){
                            $fnacimientoN = date("Y-m-d",strtotime($fnacimiento)); // Para la fecha de nacimiento
                        }
    
                        // Comprobamos si hay errores al introducir el nuevo usuario.
                        $errores = comprobarDatos($login, $password, $passwordRepe, $nombre, $fnacimiento, $fecha, $concepto, $cantidad, true);
                    
                        // si no los hay continuamos
                        if($errores != ""){
                        
                            // le damos un espacio más 
                            $errores .= "<br>";
                        
                            // Le ponemos nombre al botón
                            $boton = "Aceptar nuevo";
                            
                            // Indicamos que es un usuario nuevo
                            $tipo = "Nuevo Usuario";
                            
                            // Indicamos si es una modificación del usuario o no
                            $modificacion = false;
                        
                            // Indicamos si se debe de mostrar para borrar
                            $borrar = false;
                        
                            // Llamamos de nuevo a la función indicando los errores que hay y pasándole la
                            // la información ya introducida anteriormente.
                            cambiosUsuario($dwes, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion);
                        
                        // en caso contrario continuamos    
                        }else{
                        
                            // Para comprobar que no es el administrador en la comprobación 
                            $administrador = false;
                        
                            // Desactivamos el menú principal
                            $estado = false;
                        
                            // Indicamos que vamos a buscar al usuario dentro de la BD
                            $buscarUsuarios = true;
                        
                            // Hacemos la comprobación de que no existe un login igual en la BD
                            if(!comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password)){
                            
                                $tipoConsulta = "nuevoUsuario";
                            
                                // Ejercutamos el guardado
                                consultasBDAdministrador($dwes, $tipoConsulta, $login, $password, $nombre, $fnacimientoN);
                            
                                // Avisamos de que se ha realizado con exito
                                echo "<center><b><h2>Se ha introducido un nuevo usuario con login: $login</h2></b></center>";
                        
                            // en caso contrario avisamos de que existe ya un usuario con ese login
                            }else{
                            
                                // Indicamos que el login de usuario ya existe en otro usuario
                                $errores .= "Error: El login de usuario introducido ya existe.<br><br>";
                            
                                // Le ponemos nombre al botón
                                $boton = "Aceptar nuevo";
                            
                                // Indicamos que es un usuario nuevo
                                $tipo = "Nuevo Usuario";
                            
                                // Indicamos si es una modificación del usuario o no
                                $modificacion = false;
                            
                                // Indicamos si se debe de mostrar para borrar
                                $borrar = false;
                            
                                // Llamamos de nuevo a la función indicando los errores que hay y pasándole la
                                // la información ya introducida anteriormente.
                                cambiosUsuario($dwes, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion);
                            } 
                        }    
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción elegimos el usuario a modificar
                case "Modificar Usuario":
                    
                    // Mantenemos desactivado el menú principal
                    $estado = false;
                    
                    // Dejamos visible el menú del administrador
                    funcionesAdministrador();
                    
                    // Modo en el que se encontrará el menú cuando entremos en administrador
                    $modoMenu = "modoAdministrador";
                    
                    // Llamamos a la función para introducir el usuario
                    introduccionUsuario("Modificar Usuario", $modoMenu, "Modificar");
                    
                    break;
                
                // Con esta opción comprobamos el usuario y mostramos sus datos
                case "Modificar":
                    
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Dejamos visible el menú del administrador
                        funcionesAdministrador();
                    
                        // Recogemos la variable login para continuar más
                        // adelante utilizandola
                        $login = $_REQUEST['login'];
                    
                        // Ponemos el password como NULL (no nos hace falta en este caso
                        $password = NULL;
                    
                        // Para comprobar que no es el administrador en la comprobación
                        $administrador = false;
                        
                        // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                        $buscarUsuarios = NULL;
                        
                        // Comprobamos si el usuario existe en la base de datos
                        if(comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password)){
                                  
                            // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                            $buscarUsuarios = false;
                       
                            // Buscamos el nombre completo del usuario.
                            $nombreCabecera = comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password);
                        
                            // Le ponemos nombre al botón
                            $boton = "Modificarlo";
                            
                            // Indicamos que es un usuario nuevo
                            $tipo = "Modificar el usuario con nombre ".$nombreCabecera;
                            
                            // Indicamos si es una modificación del usuario o no
                            $modificacion = true;
                        
                            // Indicamos si se debe de mostrar para borrar
                            $borrar = false;
                            
                            // Llamamos de nuevo a la función indicando los errores que hay y pasándole la
                            // la información ya introducida anteriormente.
                            cambiosUsuario($dwes, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion);
                    
                        // en caso de no existir avisamos por pantalla    
                        }else{
                            echo "<center><b><h2>Error: El usuario introducido no existe</h2></b></center>"; 
                        }
                    }

                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción comprobamos que los nuevos datos son correctos y modificamos el usuario   
                case "Modificarlo":
                   
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Dejamos visible el menú del administrador
                        funcionesAdministrador();
                    
                        // Recogemos las distintas variables
                        $login = $_REQUEST['login']; // Para el login
                        $password = $_REQUEST['password']; // Para el password
                        $passwordRepe = $_REQUEST['passwordRepe']; // Para la repetición del password
                        $nombre = $_REQUEST['nombre']; // Para el nombre completo
                        $fnacimiento = $_REQUEST['fnacimiento']; // Para la fecha de nacimiento.
                        
                        /// Cambiamos el formato de la fecha para que concuerde con la base de datos en caso de no ser
                        // ni firefox ni chrome
                        if($fnacimiento != null){
                            $fnacimientoN = date("Y-m-d",strtotime($fnacimiento)); // Para la fecha de nacimiento
                        }
                        
                        // Comprobamos si existen errores a la hora de modificar eel usuario
                        $errores = comprobarDatos($login, $password, $passwordRepe, $nombre, $fnacimiento, $fecha, $concepto, $cantidad, true);
                        
                        // Si no existen errores
                        if($errores != ""){
                            
                            // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                            $buscarUsuarios = false;
                        
                            // Indicamos en el busquedad que no es usuario administrador
                            $administrador = false;
                       
                            // Buscamos el nombre completo del usuario.
                            $nombreCabecera = comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password);
                        
                            // le damos un espacio más 
                            $errores .= "<br>";
                        
                            // Le ponemos nombre al botón
                            $boton = "Modificarlo";
                            
                            // Indicamos que es un usuario nuevo
                            $tipo = "Modificar el usuario con nombre ".$nombreCabecera;
                            
                            // Indicamos si es una modificación del usuario o no
                            $modificacion = true;
                        
                            // Indicamos si se debe de mostrar para borrar
                            $borrar = false;
                            
                            // Llamamos de nuevo a la función indicando los errores que hay y pasándole la
                            // la información ya introducida anteriormente.
                            cambiosUsuario($dwes, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion);
                        
                        // en caso contrario continuamos    
                        }else{
                        
                            // Para comprobar que no es el administrador en la comprobación 
                            $administrador = false;
                        
                            // Indicamos que vamos a buscar al usuario dentro de la BD
                            $buscarUsuarios = NULL;
                        
                            // Hacemos la comprobación de que no existe un login igual en la BD
                            if(comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password)){
                    
                                // Indicamos el tipo de consulta que se va a realizar
                                $tipoConsulta = "actualizarDatos";
                        
                                // Ejercutamos el guardado
                                consultasBDAdministrador($dwes, $tipoConsulta, $login, $password, $nombre, $fnacimientoN);
                            }                         
                        }
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción elegimos borrar un usuario
                case "Borrar Usuario":
                    
                    // Desactivamos el menú principal debajo.
                    $estado = false;
                    
                    // Dejamos visible el menú del administrador
                    funcionesAdministrador();
                    
                    // Modo en el que se encontrará el menú cuando entremos en usuarios
                    $modoMenu = "modoUsuario";
                    
                    // Le pasamos la función que contiene el formulario para el usuario
                    // tiene como argumentos, el nombre del tipo de login, y 
                    // si se va a borrar un usuario
                    introduccionUsuario("Borrar Usuario", $modoMenu, "Borrar");
                    
                    break;
                
                // Con esta opción comprobamos que exite el usuario a borrar
                case "Borrar":
                    
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Dejamos visible el menú del administrador
                        funcionesAdministrador();
                    
                        // Recogemos las distintas variables
                        $login = $_REQUEST['login']; // El login del usuario
                        $pass = NULL; // El password del usuario
                    
                        // Indicamos que no es una busquedad de administrador
                        $administrador = false;
                    
                        // Indicamos que es la busqueda de un usuario sin pass
                        $buscarUsuarios = NULL;
                    
                        // Comprobamos si existe el usuario
                        if(comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password)){
                        
                            // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                            $buscarUsuarios = false;
                       
                            // Buscamos el nombre completo del usuario.
                            $nombreCabecera = comprobarUsuario($dwes, $administrador, $buscarUsuarios, $login, $password);
                        
                            // Le ponemos nombre al botón
                            $boton = "Borrarlo";
                            
                            // Indicamos que es un usuario nuevo
                            $tipo = "Borrar el usuario con nombre ".$nombreCabecera;
                            
                            // Indicamos si es una modificación del usuario o no
                            $modificacion = true;
                        
                            // Indicamos si se debe de mostrar para borrar
                            $borrar = true;
                            
                            // Llamamos de nuevo a la función indicando los errores que hay y pasándole la
                            // la información ya introducida anteriormente.
                            cambiosUsuario($dwes, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion);
                        
                        // en caso contrario avisamos de que no existe.
                        }else{
                            echo "<center><b><h2>Error: El usuario introducido no existe</h2></b></center>"; 
                        }
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción comprobamos si es administrador y si no lo es lo borramos
                case "Borrarlo":
 
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Dejamos visible el menú del administrador
                        funcionesAdministrador();
                    
                        // Recogemos las variables necesarias
                        $login = $_REQUEST['login']; // Para el login
                    
                        // Pasamos las variables innecesarias a NULL
                        $password = NULL; // Para el password
                        $nombre = NULL; // Para el nombre
                        $fnacimiento = NULL; // Para la fecha de nacimiento
                    
                        // Si el usuario que deseamos borrar es el Administador se avisa mediante mensaje.
                        if($login == 'dwes'){
                        
                            echo "<center><b><h2>Error: El usuario introducido es el Administrador, no se puede borrar.</h2></b></center>";
                    
                        // en caso de que sea un usuari normal
                        }else{
                        
                            // Para comprobar que no es el administrador en la comprobación 
                            $administrador = false;
                        
                            // Para comprobar si estamos buscando a un usuario o solo queremos su nombre
                            $buscarUsuarios = false;
                        
                            // Comprobamos si el usuario existe
                            if(comprobarUsuario($dwes, $administrador, $buscarUsuarios ,$login, $password)){
                            
                                // Indicamos el tipo de consulta que se va a realizar
                                $tipoConsulta = "borrarUsuario";
                            
                                consultasBDAdministrador($dwes, $tipoConsulta, $login, $password, $nombre, $fnacimiento);
                        
                            // en caso de que no exista el usuario avisamos 
                            } else {
                                echo "<center><b><h2>Error: El usuario introducido no existe</h2></b></center>";
                            }     
                        }
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
 
                /**
                 * ++++++++++++++++++++++++++
                 * OPCIONES PARA LOS USUARIOS
                 * ++++++++++++++++++++++++++
                 */
                
                // Con esta opción vemos lo últimos 4 movimientos
                case "Ultimos movimientos":
                      
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Recogemos las variables que se utilizarán
                        $login = $_REQUEST['login']; // Para el login del usuario
                        $nombreCabecera = $_REQUEST['nombre']; // Para el nombre completo de la cabecera de las opciones
                    
                        //Indicamos que se van a mostrar los 4 últimos movimientos
                        $tipoUltimos = "Últimos movimientos(4 últimos movimientos)"; 
                    
                        // Variable encargada de indicar si es una devolución para cambiar la forma de mostrar los últimos movimientos.
                        $devolucion = false; 
                    
                        // Variable encargada de indicar si se muestran solo los recibos
                        $soloRecibos = false; 
                   
                        // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                        // el nombre completo del usuario para la cabecera y el login del usuario
                        funcionesUsuario($nombreCabecera, $login);
                    
                        // Función encargada de mostrar los últimos 4 movimientos, le pasamos
                        // 5 argumentos, la base de datos, el login del usuario, el tipo de 
                        // movimientos a mostrar, si es devolución, y si solo se muestran los
                        // recibos.
                        mostrarMovimientos($dwes, $login, $tipoUltimos, $devolucion, $soloRecibos);

                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción empezamos con el ingreso de dinero
                case "Ingresar dinero":

                    // Desactivamos el menú principal debajo.
                    $estado = false;
                    
                    // Recogemos las variables que se utilizarán
                    $login = $_REQUEST['login']; // Para el login del usuario
                    $nombreCabecera = $_REQUEST['nombre']; // Para el nombre completo de la cabecera de las opciones
                    
                    // Indicamos el tipo de opción que se va a llevar a cabo
                    $tipo = "Ingreso de dinero:";
                    
                    // Indicamos el botón de opción que se va a llevar a cabo
                    $boton = "Ingresar cantidad";
                    
                    // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                    // el nombre completo del usuario para la cabecera y el login del usuario
                    funcionesUsuario($nombreCabecera, $login);                  
                    
                    // Función encargada de generar el formulario para introducir el ingreso, tenemos
                    // 7 argumentos, el tipo que en este caso es ingreso, errores si se producen al ingresar
                    // la fecha, el concepto y la cantidad del ingreso, y el nombre completo del usuario.
                    movimientos($tipo, $boton, $login, $errores, $fecha, $concepto, $cantidad, $nombreCabecera);
                    
                    break;
                
                // Con esta opción ingresamos el dinero
                case "Ingresar cantidad":
                    
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Recogemos ambas variables
                        $nombreCabecera = $_REQUEST['nombre']; // Nombre completo del usuario para la cabecera
                        $login = $_REQUEST['login']; // El login del usuario
                        $concepto = $_REQUEST['concepto']; // El concepto del ingreso
                        $cantidad = $_REQUEST['cantidad']; // La cantidad del ingreso
                        $fecha = $_REQUEST['fecha']; // La fecha del ingreso
                        
                        /// Cambiamos el formato de la fecha para que concuerde con la base de datos en caso de no ser
                        // ni firefox ni chrome
                        if($fecha != null){
                            $fechaN = date("Y-m-d",strtotime($fecha)); // Cambiamos el formato de la fecha para la BD
                            
                        }
                        
                        // Comprobamos si existen errores.
                        $errores = comprobarDatos($login, $password, $passwordRepe, $nombre, $fnacimiento, $fecha, $concepto, $cantidad, false);
                    
                        if($errores != ""){
                            // Indicamos el tipo de opción que se va a llevar a cabo
                            $tipo = "Ingreso de dinero:";
                    
                            // Indicamos el botón de opción que se va a llevar a cabo
                            $boton = "Ingresar cantidad";
                    
                            // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                            // el nombre completo del usuario para la cabecera y el login del usuario
                            funcionesUsuario($nombreCabecera, $login);                  
                    
                            // Función encargada de generar el formulario para introducir el ingreso, tenemos
                            // 7 argumentos, el tipo que en este caso es ingreso, errores si se producen al ingresar
                            // la fecha, el concepto y la cantidad del ingreso, y el nombre completo del usuario.
                            movimientos($tipo, $boton, $login, $errores, $fecha, $concepto, $cantidad, $nombreCabecera);
                        
                        // en caso contrario continuamos con el ingreso
                        }else{
                        
                            // Indicamos que no es una devolución
                            $devolucion = false;
                        
                            // Indicamos que es un ingreso
                            $tipoguardado = "ingresoDinero";
                        
                            // Ponemos como NULL el número de código
                            $codigoMov = NULL;
                        
                            // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                            // el nombre completo del usuario para la cabecera y el login del usuario
                            funcionesUsuario($nombreCabecera, $login); 
                        
                            // Ejercutamos la consulta en este caso de guardado
                            consultasBDUsuario($dwes, $tipoguardado, $login, $fechaN, $concepto, $cantidad, $codigoMov);
                            
                            // Avisamos de que se ha realizado con exito
                            echo"<center><b><h2>Se ha ingresado con éxito el concepto: $concepto</h2></b></center>";
                        
                            // Indicamos el tipo de último movimiento que es
                            $tipoUltimos = "Últimos movimientos(4 últimos movimientos)";
                        
                            // Variable encargada de indicar si se muestran solo los recibos
                            $soloRecibos = false; 
                        
                            // Función encargada de mostrar los últimos 4 movimientos, le pasamos
                            // 5 argumentos, la base de datos, el login del usuario, el tipo de 
                            // movimientos a mostrar, si es devolución, y si solo se muestran los
                            // recibos.
                            mostrarMovimientos($dwes, $login, $tipoUltimos, $devolucion, $soloRecibos);
                        }
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción iniciamos el proceso de pago de recibos    
                case "Pagar recibos":
                    
                    // Desactivamos el menú principal debajo.
                    $estado = false;
                    
                    // Recogemos algunas variables
                    $login = $_REQUEST['login']; // Para el login del usuario
                    $nombreCabecera = $_REQUEST['nombre']; // Para el nombre completo del usuario
                    $fecha = ""; // Para la fecha de ingreso    
                    $concepto = ""; // Para el concepto del ingreso
                    $cantidad = ""; // Para la cantidad a ingresar
                            
                    // Indicamos el tipo de opción que se va a llevar a cabo
                    $tipo = "Pago de recibos:";
                    
                    // Indicamos el botón de opción que se va a llevar a cabo
                    $boton = "Pagar cantidad";
                    
                    // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                    // el nombre completo del usuario para la cabecera y el login del usuario
                    funcionesUsuario($nombreCabecera, $login);                  
                    
                    // Función encargada de generar el formulario para introducir el ingreso, tenemos
                    // 7 argumentos, el tipo que en este caso es ingreso, errores si se producen al ingresar
                    // la fecha, el concepto y la cantidad del ingreso, y el nombre completo del usuario.
                    movimientos($tipo, $boton, $login, $errores, $fecha, $concepto, $cantidad, $nombreCabecera);

                    break;
                
                // Con esta opción se pagará el recibo
                case "Pagar cantidad":
                    
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Recogemos ambas variables
                        $nombreCabecera = $_REQUEST['nombre']; // Para el nombre completo de la cabecera
                        $login = $_REQUEST['login']; // Para el login del usuario
                        $fecha = $_REQUEST['fecha']; // Para la fecha de pago del recibo
                        $concepto = $_REQUEST['concepto']; // Para el concepto del recibo
                        $cantidad = $_REQUEST['cantidad']; // Para la cantidad del recibo
                      
                        /// Cambiamos el formato de la fecha para que concuerde con la base de datos en caso de no ser
                        // ni firefox ni chrome
                        if($fecha != null){
                            $fechaN = date("Y-m-d",strtotime($fecha)); // Para la fecha de ingreso
                            
                        }
                        
                        // Comprobamos si existen errores a la hora de pagar
                        $errores = comprobarDatos($login, $password, $passwordRepe, $nombre, $fnacimiento, $fecha, $concepto, $cantidad, false);
                    
                        // si no los hay
                        if($errores != ""){
                            // Indicamos el tipo de opción que se va a llevar a cabo
                            $tipo = "Pago de recibos:";
                    
                            // Indicamos el botón de opción que se va a llevar a cabo
                            $boton = "Pagar cantidad";
                    
                            // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                            // el nombre completo del usuario para la cabecera y el login del usuario
                            funcionesUsuario($nombreCabecera, $login);                  
                    
                            // Función encargada de generar el formulario para introducir el ingreso, tenemos
                            // 7 argumentos, el tipo que en este caso es ingreso, errores si se producen al ingresar
                            // la fecha, el concepto y la cantidad del ingreso, y el nombre completo del usuario.
                            movimientos($tipo, $boton, $login, $errores, $fecha, $concepto, $cantidad, $nombreCabecera);
                        
                        // en caso contrario continuamos    
                        }else{
                        
                            // Indicamos que es un ingreso
                            $tipoguardado = "pagaRecibos";
                        
                            // Ponemos como NULL el número de código
                            $codigoMov = NULL;
                        
                            // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                            // el nombre completo del usuario para la cabecera y el login del usuario
                            funcionesUsuario($nombreCabecera, $login); 
                        
                            // Ejercutamos la consulta en este caso de guardado
                            consultasBDUsuario($dwes, $tipoguardado, $login, $fechaN, $concepto, $cantidad, $codigoMov);
                            
                            // Avisamos de que se ha realizado con exito
                            echo"<center><b><h2>Se ha pagado el recibo con éxito, su concepto es: $concepto</h2></b></center>";
                        
                            // Indicamos el tipo de último movimiento que es
                            $tipoUltimos = "Recibos del usuario";
                        
                            // Variable encargada de indicar si se muestran solo los recibos
                            $soloRecibos = false; 
                        
                            // Indicamos que no es una devolución
                            $devolucion = false;
                        
                            // Función encargada de mostrar los últimos 4 movimientos, le pasamos
                            // 5 argumentos, la base de datos, el login del usuario, el tipo de 
                            // movimientos a mostrar, si es devolución, y si solo se muestran los
                            // recibos.
                            mostrarMovimientos($dwes, $login, $tipoUltimos, $devolucion, $soloRecibos);
                        }
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                 
                // Con esta opción se comienza el proceso de devolución de recibos  
                case "Devolver recibos":
                       
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Recogemos algunas variables
                        $login = $_REQUEST['login']; // Para el login del usuario
                        $nombreCabecera = $_REQUEST['nombre']; // Para el nombre completo del usuario
                    
                        //Indicamos que se va a delvolver el recibo
                        $tipoUltimos = "Devolver Recibos"; 
                   
                        // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                        // el nombre completo del usuario para la cabecera y el login del usuario
                        funcionesUsuario($nombreCabecera, $login); 
                    
                        // Función utilizada para la devolución de los recibos, dispone de 4
                        // argumentos, la BD, el login del usuario, el tipo de de tarea y
                        // el nombre completo de usuario para la cabecera
                        devolverRecibos($dwes, $login, $tipoUltimos, $nombreCabecera);
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // Con esta opción devolvemos el recibo elegido
                case "Devolverlo":
                          
                    // Iniciamos y comprobamos la conexión
                    if(conexion()){
                    
                        // Desactivamos el menú principal debajo.
                        $estado = false;
                    
                        // Recogemos algunas variables
                        $login = $_REQUEST['login']; // Para el login del usuario
                        $nombreCabecera = $_REQUEST['nombre']; // Para el nombre completo del usuario
                        $codigoMov = $_REQUEST['codigoMov']; // Para el código del movimiento (recibo)
                    
                        // Tipo de consulta a realizar
                        $tipoConsulta = "devolucionRecibo";
                   
                        // Mostramos las funciones del usuario, a la que le pasamos dos argumentos
                        // el nombre completo del usuario para la cabecera y el login del usuario
                        funcionesUsuario($nombreCabecera, $login); 
                    
                        // Ejercutamos la consulta en este caso de devolución
                        consultasBDUsuario($dwes, $tipoConsulta, $login, $fecha, $concepto, $cantidad, $codigoMov);
                        
                        
                        //Indicamos que se va a delvolver el recibo
                        $tipoUltimos = "Recibos"; 
                        
                        // Indicamos que no es una devolución
                        $devolucion = false;
                        
                        // Función encargada de mostrar los últimos 4 movimientos, le pasamos
                        // 5 argumentos, la base de datos, el login del usuario, el tipo de 
                        // movimientos a mostrar, si es devolución, y si solo se muestran los
                        // recibos.
                        mostrarMovimientos($dwes, $login, $tipoUltimos, $devolucion, true);
                    }
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
                
                // En otro caso 
                default:
                    
                    // Se activa el menú principal
                    $estado = true;
                    
                    // Cerramos la conexión con la base de datos
                    desconexion();
                    
                    break;
            }
        }
        
        // Menú principal con un argumento que lo activa o desactiva
        principal($estado);
        
        // Pie de la página Web
        pie("Desarrollo web en entorno servidor - Tarea 2");  
        ?>
        </form>
    </body>
</html>


