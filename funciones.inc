<?php
/**
* @modulo      Desarrollo Web en Entorno Servidor
* @Tema        Unidad 2 : Trabajar con bases de datos en PHP
* @Unidad      2
* @author      Rubén Ángel Rodriguez Leyva
*/
/**
 * ++++++++++++++++++++++++++++++++++++++++++++
 * DE USO COMPARTIDO (ADMINISTRADOR Y USUARIOS)
 * ++++++++++++++++++++++++++++++++++++++++++++
 */


/**
 * Función creada para establecer la conexión con la BD
 */
function conexion()
{
    try{
        // La bd
       global $dwes;
       
       // Comprobar la IP, en Windows 127.0.0.1 en MAC en mi caso 192.168.64.2
       $dwes = new PDO('mysql:host=127.0.0.1;dbname=banca_electronica', 'dwes', 'dwes');
       
       // Habilitamos las excepciones para poder capturarlas y tratarlas
       $dwes->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
    // En caso de excepción   
    } catch (PDOException $ex) {
        
        // Avisamos de que se ha producido un error con la conexión
        echo "<center><b><h2>Se ha producido un error en la conexión</h2></b></center>";
        
        // Y mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
        echo "<center><b><h3>Error: " .$ex->getMessage()."</h3></b></center>";
    }
    
    // Retornamos la conexción
    return $dwes;
}


/**
 * Función creada para la desconexión con la BD
 */
function desconexion()
{
    try{
        
        // La BD
        global $dwes;
        
        // Desconectamos
        $dwes = null;
        
    // En caso de excepción    
    } catch (PDOException $ex) {
        
        // Avisamos de que se ha producido un error con la conexión
        echo "<center><b><h2>Se ha producido un error póngase en contacto con el administrador de la Web</h2></b></center>";
        
        // Y mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
        echo "<center><b><h3>Error: " .$ex->getMessage()."</h3></b></center>";
    }
}


/**
 * Función creada para mostrar la pantalla principal
 * 
 * @param type $estado La variable que nos indica si debemos mostrarlo o no
 */
function principal($estado){
    
    // Comprobamos si podemos mostrar la pantalla principal.
    if($estado){
    ?>
        <form name="formulario" action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
            <div>
                <table style="margin: 10px auto 15px auto">
                    <tr>         
                        <td><input type='submit' name='eleccion' value='Gestionar Usuarios'/></td>
                        <td><input type='submit' name='eleccion' value='Gestionar mi cuenta'/></td>
                    </tr>
                </table>
            </div>
        </form>
    <?php  
    }              
}


/**
 * Función encargada del menú de introducción del login
 * en caso de que quedaramos borrar o modificar un usuario
 * también haremos uso de dicha función para introducir 
 * el nombre del mismo.
 * 
 * @param type $titulo El titulo del formulario
 * @param type tipo El tipo si es administrador o usuario normal
 * @param type $tipoTarea El tipo de tarea a desarrollar, Iniciar, Borrar, Modificar
 */
function introduccionUsuario($titulo, $tipo, $tipoTarea) {
    
    ?>
        <div>
            <table style="margin: 10px auto 15px auto">
                <form method=post action=$_SERVER[PHP_SELF]>
                    <tr><td width='30%'></td><td width='70%'></td></tr>
                    <tr><td colspan=2><b><i><?php echo $titulo; ?> :<i></b></td></tr>
                    <tr><td>Login:</td><td><input type=text name=login value='' size=50 maxlength=50></td></tr>
                    <?php
                        if($tipoTarea == "Iniciar sesión"){
                            echo "<tr><td>Password:</td><td><input type=password name=password value='' size=50 maxlength=50></td></tr>";
                        }
                    ?>                    
                    <tr><td colspan=2><i>Por favor, ingrese los datos solicitados y haga clic en '<?php echo $tipoTarea; ?>'. Todos los campos son obligatorios.</i></td></tr>
                    <tr><td><input type=submit name=eleccion value='<?php echo $tipoTarea; ?>'/>
                            <input type=hidden name=modoMenu value='<?php echo $tipo; ?>'/>
                </form>
            </table> 
        </div>
    <?php
}


/**
 * Función creada para comprobar que existe o logeo del administrador o usuario
 * en este último se pueden dar 3 casos, que deseemos saber solo si se encuentra
 * en la BD, que deseamos logearnos o saber el nombre completo del login. 
 * 
 * @param type $bd La conexión con la BD
 * @param type $administrador Nos indica si es o no la comprobación del administrador
 * @param type $login El login del usuario
 * @param type $pass El password del usuario
 */
function comprobarUsuario($bd, $administrador, $buscar, $login, $pass)
{   
    // Comprobamos si quién se logea es el administrador
    if($administrador){
        
        // En caso del que el login y password coincidan devolvemos true
        if(($login == "dwes") && ($pass == "dwes")){
            return true;
        } else {
            return false;
        }
    
    // en caso contrario comprobamos que es un usuario y si exite en la BD
    // o si se loguea bien con su pass y login o por último si queremos 
    // saber su nombre completo.
    } else {
        
        try {
        
            // En caso de querer saber el nombre del usuario
            if(!$buscar){
                
                $stmt = $bd->prepare('SELECT login, password, nombre, fNacimiento FROM Usuarios WHERE login like :login');
        
                $stmt->execute(array(':login' => $login));

                while ($datos = $stmt->fetch()) {
                    if($datos[0]){ 
                        return $datos['nombre'];  
                    }
                }
                
            // en caso de que solo sea loguear al usuario con login y pass
            } else if($buscar) {
                
                $stmt = $bd->prepare('SELECT * FROM Usuarios WHERE login like :login AND password like :pass');
        
                $stmt->execute(array(':login' => $login,
                ':pass' => $pass));

                while ($datos = $stmt->fetch()) {
                    if($datos[0]){ 
                        return true;
                    }
                }
                
            // en caso de que se quiera saber solo si existe el usuario en la BD sin necesidad de pass
            }else{
                
                $stmt = $bd->prepare('SELECT * FROM Usuarios WHERE login like :login');
        
                $stmt->execute(array(':login' => $login));

                while ($datos = $stmt->fetch()) {
                    if($datos[0]){ 
                        return true;
                    }
                }
            }
    
        // Posibles excepciones generadas   
        } catch(PDOException $ex) {
            
            // Avisamos de que se ha producido un error con la conexión
            echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
            // Y mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
             echo "<center><b><h3>Error: " .$ex->getMessage()."</h3></b></center>";
        }
    }
}


/**
 * ++++++++++++++++++++
 * DE USO ADMINISTRADOR
 * ++++++++++++++++++++
 */

/**
 *  Función encargada de mostrar el menú del administrador
 */
function funcionesAdministrador()
{   
    ?>
        <!-- Menú con los diferentes botones necesarios para la tarea -->
        <form name="formulario" action="$_SERVER[PHP_SELF]" method="post">
            <div>
                <table style="margin: 10px auto 15px auto">
                    <tr><td colspan=3><b><i>Bienvenid@ Administrador <i></b></td></tr>
                    <tr>
                        <td><input type="submit" name="eleccion" value="Nuevo Usuario"/></td>
                        <td><input type="submit" name="eleccion" value="Modificar Usuario"/></td>
                        <td><input type="submit" name="eleccion" value="Borrar Usuario"/></td>
                        <td><input type="submit" name="eleccion" value="Salir"/></td>
                    </tr>
                </table>
            </div>
        </form>
    <?php
}


/**
 * Función utilizada para realizar diferentes tareas con los usuarios.
 * 
 * @param type $bd La base de datos
 * @param type $errores Los errores que se pueden generar
 * @param type $tipo El tipo de tarea que se realiza
 * @param type $borrar Si se trata de borrar un usuario
 * @param type $boton El titulo del botón
 * @param type $login El login del usuario
 * @param type $password El password del usuario
 * @param type $passwordRepe La repetición del password
 * @param type $nombre El nombre completo del usuario
 * @param type $fnacimiento La fecha de nacimiento
 * @param type $modificacion Si es una modificación del usuario
 */
function cambiosUsuario($bd, $errores, $tipo, $borrar, $boton, $login, $password, $passwordRepe, $nombre, $fnacimiento, $modificacion)
{   
    // Comprobamos el tipo de navegador que utiliza
    $info = detect();
    
   ?>
    <div>
        <table style="margin: 10px auto 15px auto">
        <form method=post action="$_SERVER[PHP_SELF]">
            <tr><td width='30%'></td><td width='70%'></td></tr>
            <tr><td colspan=2><b><i><?php echo $errores; ?><i></b></td></tr>
            <tr><td colspan=2><b><i><?php echo $tipo; ?> :<i></b></td></tr>
            
            <?php
            // Si no son ni Chrome ni Firefox avisamos
            if($info['browser'] == "OTHER" && !$borrar){
                echo "<tr><td colspan=2><b><i>NOTA: Por su explorador deberá introducir la fecha como se muestra a continuación, ej: 06/02/1983. <i></b></td></tr>";
            }
            
            // Si se trata de una modificación o borrado mostramos algunos datos de forma diferente
            if($modificacion){
               
                // el login lo ponemos como solo lectura para que no pueda ser modificado
                echo "<tr><td>Login:</td><td><input type=text readonly=readonly name=login value= '$login' size=50 maxlength=50></td></tr>";
                
                try {
                    
                    // Preparamos la consulta a la base de datos
                    $stmt = $bd->prepare('SELECT password, nombre, fnacimiento FROM Usuarios WHERE login like :login');
                    
                    // la ejecutamos
                    $stmt->execute(array(':login' => $login ));
                    
                    // mientras existan datos en la base de datos
                    while ($datos = $stmt->fetch()) {
                        
                        // comprobamos que se corresponden con la consulta
                        if($datos[0]){
                            
                            // si es para borrar hacemos que todos los campos sean de solo lectura
                            if($borrar){
                                echo "<tr><td>Password:</td><td><input type=text readonly=readonly name=password value= '$datos[password]' size=50 maxlength=50></td></tr>";
                                echo "<tr><td>Nombre:</td><td><input type=text readonly=readonly name=nombre value= '$datos[nombre]' size=50 maxlength=50></td></tr>";
                                if($info['browser'] == "OTHER"){ 
                                    
                                    $formatoOther = date('d/m/Y', strtotime($datos['fnacimiento']));
                                    
                                    echo "<tr><td>Fecha de nacimiento:</td><td><input type=text readonly=readonly name=fnacimiento  value='$formatoOther' size=10 maxlength=10></td></tr>";
                                }else{
                                    echo "<tr><td>Fecha de nacimiento:</td><td><input type=date readonly=readonly name=fnacimiento  value='$datos[fnacimiento]'></td></tr>";
                                } 
  
                            // en caso contrario permitimos que se puedan modificar
                            }else{
                                echo "<tr><td>Password:</td><td><input type=text name=password value= '$datos[password]' size=50 maxlength=50></td></tr>";
                                echo "<tr><td>Repetir Password:</td><td><input type=text name=passwordRepe value='$datos[password]' size=50 maxlength=50></td></tr>";
                                echo "<tr><td>Nombre:</td><td><input type=text name=nombre value= '$datos[nombre]' size=50 maxlength=50></td></tr>";
                                if($info['browser'] == "OTHER"){
                                    
                                    $formatoOther = date('d/m/Y', strtotime($datos['fnacimiento']));
                                    echo "<tr><td>Fecha de nacimiento:</td><td><input type=text name=fnacimiento  value='$formatoOther' size=10 maxlength=10></td></tr>";
                                }else{
                                    
                                    echo "<tr><td>Fecha de nacimiento:</td><td><input type=date name=fnacimiento  value='$datos[fnacimiento]'></td></tr>";
                                }
                            }
                        }
                    }
                    
                // En caso de que salte alguna excepción    
                } catch(PDOException $ex) {
                    
                    // Avisamos de que se ha producido un error con la conexión
                    echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
                    // Y mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
                    echo "<center><b><h2>Error: " .$ex->getMessage()."</h2></b></center>";
                }
            
            // en caso de que no sea una modificación o borrado mostramos los campos de forma normal
            }else{    
                ?> 
                    <tr><td>Login:</td><td><input type=text name=login value='<?php echo $login; ?>' size=50 maxlength=50></td></tr>
                    <tr><td>Password:</td><td><input type=text name=password value='<?php echo $password; ?>' size=50 maxlength=50></td></tr>
                    <tr><td>Repetir Password:</td><td><input type=text name=passwordRepe value='<?php echo $passwordRepe; ?>' size=50 maxlength=50></td></tr>
                    <tr><td>Nombre:</td><td><input type=text name=nombre value='<?php echo $nombre; ?>' size=50 maxlength=50></td></tr>
                <?php
                    
                    // En caso de que se trate de un explorador diferente a Firefox o Chrome mostramos la fecha de nacimiento de otra forma, ya que date no es
                    // compatible con todos los exploradores.
                    if($info['browser'] == "OTHER"){

                        // Comprobamos si fnacimiento se encuentra vacía.
                        if($fnacimiento != null){
                            $formatoOther = date("m/d/Y", strtotime($fnacimiento));
                        }else{
                            $formatoOther = $fnacimiento;
                        }                      
                        
                        echo "<tr><td>Fecha de nacimiento:</td><td><input type=text name=fnacimiento  value='$formatoOther' size=10 maxlength=10></td></tr>";
                    }else{
                        echo "<tr><td>Fecha de nacimiento:</td><td><input type=date name=fnacimiento  value='$fnacimiento'></td></tr>";
                    }   
            }
            ?>
            <tr><td colspan=2><i>Por favor, ingrese los datos solicitados y haga clic en '<?php echo $boton; ?>'. Todos los campos son obligatorios.</i></td></tr>
            <tr><td><input type=submit name=eleccion value='<?php echo $boton; ?>'/></td><td></td>
            </form>
        </table> 
    </div>
    <?php
}


/**
 * Función encargada de las consultas a la BD por parte del administrador
 * en ella se podrá llevar a cabo la introducción de un nuevo usuario
 * la actualización de los datos de un usuario o el borrado de un usuario
 * y sus movimientos.
 * 
 * @param type $bd La base de datos
 * @param type $tipoguardado El tipo de guardado
 * @param type $login El login del usuario
 * @param type $password El password del usuario
 * @param type $nombre El nombre y apellidos del usuario
 * @param type $fnacimiento La fecha de nacimiento del usuario
 */
function consultasBDAdministrador($bd, $tipoguardado, $login, $password, $nombre, $fnacimiento)
{    
    try{
            
        // Si el tipo de guardado corresponde con un nuevo usuario
        if($tipoguardado == "nuevoUsuario"){
               
            // Se prepara la consulta
            $stmt = $bd->prepare('INSERT INTO Usuarios(login, password, nombre, fnacimiento)
                VALUES(:login, :password, :nombre, :fnacimiento);');
                
            // y la ejecutamos pasandole los diferentes datos
            $stmt->execute([':login' => $login,
                ':password' => crypt($password),
                ':nombre' => $nombre,
                ':fnacimiento' => $fnacimiento
                ]);  
            }
            
        // Si el tipo de guardado corresponde con una actualización de los datos de un usuario
        if($tipoguardado == "actualizarDatos"){
                    
            // Se prepara la consulta
            $stmt = $bd->prepare("UPDATE Usuarios SET password = :password, nombre = :nombre, fnacimiento = :fnacimiento
                WHERE login like :login");
                
            // y la ejecutamos añadiendo los nuevos datos
            $rows = $stmt->execute( array( ':login' => $login,
                ':password' => $password,
                ':nombre' => $nombre,
                ':fnacimiento' => $fnacimiento)
                );
                
            // si se corresponde nos avisará por mensaje de que se ha actualizado el usuario
            if($rows > 0){   
                echo "<center><b><h2>Se ha actualizado el usuario</h2></b></center>";
            } 
        }
            
        // Si el tipo de guardado corresponde con un borrado de usuario y sus movimientos
        // NOTA: creo que se podría haber metido todo en una misma consulta.
        if($tipoguardado == "borrarUsuario"){

            // se prepara la consulta
            $stmt = $bd->prepare('DELETE FROM Movimientos WHERE loginUsu like :login');
                
            // la ejecutamos según el login
            $rows = $stmt->execute(array(':login' => $login ));
                
            // vemos si existe
            if($rows > 0){
                    
                // en tal caso preparamos el borrado del usuario a través de su login
                $stmt = $bd->prepare('DELETE FROM Usuarios WHERE login like :login');
                
                // ejecutamos con el login del usuario a borrar
                $rows = $stmt->execute(array(':login' => $login ));
                    
                // si se da el caso avisamos por pantalla de que se ha borrado con éxito
                if($rows > 0){
                    echo "<center><b><h2>Se ha borrado el usuario y sus movimientos con éxito</h2></b></center>";
                }
            }
        }
        
    // en caso de excepciones
    } catch(PDOException $ex) {
            
        // Avisamos de que se ha producido un error con la conexión
        echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
        // Y mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
        echo "<center><b><h3>Error: " .$ex->getMessage()."</h3></b></center>";
    }       
}


/**
 * +++++++++++++++
 * DE USO USUARIOS
 * +++++++++++++++
 */

/**
 * Función encargada del menú principal de los usuarios.
 * 
 * @param type $nombre El nombre completo para la cabecera
 * @param type $login El login del usuario
 */
function funcionesUsuario($nombre, $login)
{
    ?>
        <div>
            <form name="formulario" action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
                <table style="margin: 10px auto 15px auto">
                    <tr><td colspan=3><b><i>Bienvenid@ <?php echo $nombre; ?> <i></b></td></tr>
                    <tr>
                        <td><input type="submit" name="eleccion" value="Ultimos movimientos"/></td>
                        <td><input type="submit" name="eleccion" value="Ingresar dinero"/></td>
                        <td><input type="submit" name="eleccion" value="Pagar recibos"/></td>
                        <td><input type="submit" name="eleccion" value="Devolver recibos"/></td>
                        <td><input type="submit" name="eleccion" value="Salir"/></td>
                    </tr>
                    <input type="hidden" name="nombre" value='<?php echo $nombre ?>'/>
                    <input type="hidden" name="login" value='<?php echo $login ?>'/>    
                </table>    
            </form>
        </div>
    <?php
}


/**
 * Función encargada de los diferentes movimientos de ingreso, pago y borrado.
 * 
 * @param type $tipo El tipo de opción que se llevará a cabo
 * @param type $boton El texto que tendrá el botón
 * @param type $login El login del usuario
 * @param type $errores Los errores que se pueden generar
 * @param type $fecha La fecha de ingreso o pago
 * @param type $concepto El concepto de ingreso o pago
 * @param type $cantidad La cantidad a ingresar o pagar
 * @param type $nombre El nombre completo del usuario para el encabezado
 */
function movimientos($tipo, $boton, $login, $errores, $fecha, $concepto, $cantidad, $nombre) {
       
    // Comprobamos el navegador
    $info = detect();
    
    ?>
    <div>
        <form method=post action=$_SERVER[PHP_SELF]>
            <table style="margin: 10px auto 15px auto">
                <tr><td width='30%'></td><td width='70%'></td></tr>
                <?php
                    // si existen errores se muestras.
                    if($errores != ""){
                        echo "<tr><td colspan=2><b>$errores</b><br></td></tr>";
                    }
                ?> 
                <tr><td colspan=2><b><?php echo $tipo; ?></b></td></tr>              
                <?php   
                
                    // Si no son ni Chrome ni Firefox avisamos
                    if($info['browser'] == "OTHER"){
                        echo "<tr><td colspan=2><b><i>NOTA: Por su explorador deberá introducir la fecha como se muestra a continuación, ej: 06/02/1983. <i></b></td></tr>";
                    }
                
                    if($info['browser'] == "OTHER"){
                        echo "<tr><td>Fecha:</td><td><input type=text name=fecha  value='$fecha' size=10 maxlength=10></td></tr>";
                    }else{
                        echo "<tr><td>Fecha:</td><td><input type=date name=fecha value='$fecha' size=10 maxlength=10></td></tr>";
                    }
                ?>
                <tr><td>Concepto:</td><td><input type=text name=concepto value='<?php  echo $concepto ?>' size=50 maxlength=50></td></tr>
                <tr><td>Cantidad:</td><td><input type=text name=cantidad value='<?php echo $cantidad ?>'size=50 maxlength=50></td></tr>
                <tr><td colspan=2><i>Por favor, ingrese los datos solicitados y haga clic en '<?php echo $boton; ?>'. Todos los campos son obligatorios.</i></td></tr>
                    <input type="hidden" name="nombre" value='<?php echo $nombre ?>' />
                    <input type="hidden" name="login" value='<?php echo $login ?>' />
                <tr><td><input type=submit name=eleccion value='<?php echo $boton; ?>' /></td><td></td>
            </table> 
        </form>
    </div>
    <?php
}


/**
 * Función encargada de el tipo de consultas que se puede llevar a cabo por
 * parte del usuario en la base de datos, ingreso dinero, pago de recibos 
 * y devolución.
 * 
 * @param type $bd La base de datos
 * @param type $tipoConsulta El tipo de consulta
 * @param type $login El login del usuario
 * @param type $fecha La fecha de ingreso o pago
 * @param type $concepto El concepto de ingreso o pago
 * @param type $cantidad La cantidad a ingresar o pagar
 * @param type $codigoMov El código del movimiento
 */
function consultasBDUsuario($bd, $tipoConsulta, $login, $fecha, $concepto, $cantidad, $codigoMov)
{  
    try{
            
        // Variable encanrgada del poner a cero el último código
        $ultimoCodigo = 0;
            
        // Variable de control para saber cual es el mayor
        $mayor = 0;
            
        // Preparamos la consulta para saber cual es el último código introducido
        $stmt = $bd->prepare('SELECT codigoMov FROM Movimientos ORDER BY codigoMov');
            
        // ejecutamos la consulta
        $stmt->execute();
            
        // mientras éxistan datos en la base de datos
        while( $datos = $stmt->fetch()){
                
            // le pasamos el código encontrado a la variable
            $ultimoCodigo = $datos[0];
                
            // comprobamos que es mayor que la variable de control
            if($ultimoCodigo > $mayor){
                    
                // Si es mayyor que la variable de control le pasamos a 
                // esta última el código comprobado
                $mayor = $ultimoCodigo;
            }
        }
            
        // Si tenemos que mayor es igual a NULL (es la primera introducción)
        // le damos el valor de cero
        if($mayor == NULL){
            $mayor = 0;
        }
            
        // Para la consulta de tipo ingreso de dinero
        if($tipoConsulta == "ingresoDinero"){
                  
            // preparamos la consulta
            $stmt = $bd->prepare('INSERT INTO Movimientos(codigoMov, loginUsu, fecha, concepto, cantidad)
                VALUES(:codigoMov, :loginUsu, :fecha, :concepto, :cantidad);
                ');
                
            // la ejecutamos pasándole los valores de las variables
            $stmt->execute([ ':codigoMov' => $mayor + 1,
                ':loginUsu' => $login,
                ':fecha' => $fecha,
                ':concepto' => $concepto,
                ':cantidad' => $cantidad  
                ]); 
        }
            
        // Para la consulta de pago de recibos
        if($tipoConsulta == "pagaRecibos"){
                  
            // Se prepara la consulta
            $stmt = $bd->prepare('INSERT INTO Movimientos(codigoMov, loginUsu, fecha, concepto, cantidad)
                VALUES(:codigoMov, :loginUsu, :fecha, :concepto, :cantidad);
                ');
                
            // se ejecuta pasándole los datos
            $stmt->execute([ ':codigoMov' => $mayor + 1,
                ':loginUsu' => $login,
                ':fecha' => $fecha,
                ':concepto' => $concepto,
                ':cantidad' => -$cantidad  
            ]); 
        }
            
        // Para la consulta de devolución de recibos
        if($tipoConsulta == "devolucionRecibo"){

            // se prepara la consulta
            $stmt = $bd->prepare('DELETE FROM Movimientos WHERE loginUsu like :login AND codigoMov like :codigoMov');
                
            // se ejecuta en un array con el login y el código del movimiento
            $rows = $stmt->execute(array(':login' => $login,
                ':codigoMov' => $codigoMov)
                );
                
            // si se encuentra avisamos de que se ha devuelto con éxito el recibo
            if($rows > 0){    
                echo "<center><b><h2>Se ha devuelto el recibo con código de movimiento: $codigoMov.</h2></b></center>";
            }
        }
         
    // en caso de que salten excepciones    
    } catch(PDOException $e) {
            
        // Avisamos de que se ha producido un error con la conexión
        echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
        // Mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
        echo "Error: " .$ex->getMessage();
    }       
}


/** 
 * Función encargada de los movimientos de los usuarios.
 * 
 * @param type $bd La base de datos 
 * @param type $login el login del usuario
 * @param type $tipoUltimos El tipo de movimiento a mostrar
 * @param type $devolucion Si son de devoluciones a mostrar
 * @param type $soloRecibos Si son solo recimos a mostras
 */
function mostrarMovimientos($bd, $login, $tipoUltimos, $devolucion, $soloRecibos) 
{  
    
    $saldoTotal = 0; // La variable es utilizada para que me sume todo el saldo que hay aunque solo aparecezcan los últimos cuatro movimientos.
    $contadorRecibos = 0; // La variable es utilizada para contar los recibos que hay, si no existe ninguno pasamos un mensaje.
    
    // Comenzamos averiguando cuantos movimientos hay
    try{
        // Preparamos la consulta
        $stmtSaldo = $bd->prepare('SELECT * FROM Movimientos WHERE loginUsu like :loginUsu');
        
        // La ejecutamos pasándole el login
        $stmtSaldo->execute(array(':loginUsu' => $login));
    
        // Mientras existan datos 
        while ($datos = $stmtSaldo->fetch()) {
        
            // que coincidan con la consulta
            if($datos[0]){
            
                // Sumamos uno al contador de recibos
                $contadorRecibos++;
            }  
        }  
        
    // capturamos posibles excepciones      
    } catch (PDOException $ex) {
        
        // Avisamos de que se ha producido un error con la conexión
        echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
        // Mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
        echo "Error: " .$ex->getMessage();
    }
       
    
    // Le restamos al número de recibos 4 para saber desde donde se debe de empezar
    // a contar
    $contadorRecibosMenos = $contadorRecibos - 4;

    ?>
     <div>
        <table  border="2px" style="margin: 1% auto 1% ">
            <form method=post action=$_SERVER[PHP_SELF]>
                <tr><td colspan=4 align=center><b><?php echo $tipoUltimos ?></b></td></tr>           
                <tr>
                    <th>Fecha</th>
                    <th>Concepto</th>
                    <th>Cantidad</th>
                    <?php
                    
                    // Comprobamos si es una devolución para mostrar en el último espacio un texto u otro.
                    if(!$devolucion){
                        
                        // Comprobamos si son solo recibos lo que debemos de mostrar
                        if(!$soloRecibos){
                            echo "<th>Saldo Contable</th>";
                        }
                    // en caso de devolución mostramos la casilla
                    }else{
                        echo "<th>DEVOLVER</th>"; 
                    }  
                echo "</tr>";
                
                try{
                    
                    // Preparamos la consulta
                    $stmt = $bd->prepare('SELECT * FROM Movimientos WHERE loginUsu like :loginUsu ORDER BY codigoMov + 0');
                  
                    // ejecutamos la consulta pasándole el login del usuario
                    $stmt->execute(array(':loginUsu' => $login));
                
                    // ponemos el contador a cero
                    $nuevoContador = 0;
                
                    // mientras existan datos
                    while ($datos = $stmt->fetch()) {
                    
                        // si el movimiento a mostrar se encuentra entre los 4 últimos movimientos
                        if(($nuevoContador >= $contadorRecibosMenos) && ($nuevoContador <= $contadorRecibos)){
                        
                            // y éxiste el movimiento
                            if($datos[0]){
                            
                                // se le asigna la cantidad de dinero
                                $cantidadDinero = $datos['cantidad'];
                            
                                // se hace la suma del saldo total
                                $saldoTotal = calcular_Saldo_Contable($saldoTotal, $cantidadDinero);
                            
                                // Si solo son los recibos y la cantidad es menor de cero
                                if($soloRecibos && $cantidadDinero < 0){  
                                    
                                    // Mostramos los diferentes datos en la tabla.
                                    echo " <tr>";
                                    echo "  <td align=center><small>".$datos['fecha']."</td>";
                                    echo "  <td align=center><small>".$datos['concepto']."</td>";
                           
                            
                                    // Si el valor de la cantidad es menor de cero lo ponemos en color rojo.
                                    if($datos['cantidad'] < 0){
                                        echo "<td align=center><small><font color=red>".round($datos['cantidad'], 2)." €</font></td>";
                            
                                    // en caso contrario lo dejamos normal.    
                                    }else{
                                        echo "<td align=center><small>".round($datos['cantidad'], 2)." €</td>";
                                    }
                                }
                            
                                // No son solo recibos lo que debemos mostrar
                                if(!$soloRecibos){
                                
                                    // Mostramos los diferentes datos en la tabla.
                                    echo " <tr>";
                                    echo "  <td align=center><small>".$datos['fecha']."</td>";
                                    echo "  <td align=center><small>".$datos['concepto']."</td>";
                           
                            
                                    // Si el valor de la cantidad es menor de cero lo ponemos en color rojo.
                                    if($datos['cantidad'] < 0){
                                        echo "<td align=center><small><font color=red>".round($datos['cantidad'], 2)." €</font></td>";
                            
                                    // en caso contrario lo dejamos normal.    
                                    }else{
                                        echo "<td align=center><small>".round($datos['cantidad'], 2)." €</td>";
                                    }
                                
                                    // En caso de que no sea una devolución.
                                    if(!$soloRecibos){
                                    
                                        // si se trata del último movimiento...
                                        if($nuevoContador >= $contadorRecibos - 1){
                                    
                                            // y además es menor de cero, lo ponemos en rojo y con un tamaño mayor.
                                            if($saldoTotal < 0){
                                                echo "<td align=center><small><font color=red size=3><strong>".round($saldoTotal, 2)." €</strong></font></small></td>";
                                    
                                            // en caso contrario se deja por defecto.    
                                            } else {
                                                echo "<td align=center><small><font size=3><strong>".round($saldoTotal, 2)." €</strong></small></td>";
                                            }
                                
                                        // si no se trara del último movimiento..
                                        } else {
                                
                                            // y es menor de cero, se pone de color rojo.
                                            if($saldoTotal < 0){
                                                echo "<td align=center><small><font color=red>".round($saldoTotal, 2)."€ </font></small></td>";
                                    
                                            //en caso contrario se deja por defecto.    
                                            } else {
                                                echo "<td align=center><small>".round($saldoTotal, 2)." €</small></td>";
                                            }
                                        }  
                                    }
                                }  
                            }
                        
                        // si no se encuentra entre los 4 últimos movimientos hacemos la suma del resto de cantidades
                        // también para no dejar ningún movimiento sin contar
                        }else{
                        
                            // se le asigna la cantidad de dinero
                            $cantidadDinero = $datos['cantidad'];
                        
                            // Calculamos el saldo total
                            $saldoTotal = calcular_Saldo_Contable($saldoTotal, $cantidadDinero);
                        }
                    
                        // Contamos la cantidad de recibos que hay.
                        $nuevoContador++;                
                    } 
                    
                // capturamos posibles excepciones    
                } catch (PDOException $e) {

                    // Avisamos de que se ha producido un error con la conexión
                    echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
                    // Mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
                    echo "Error: " .$e->getMessage();
                }
                
                
                // Si la cantidad de recibos es menor de 1 avisamos de que no existen recibos
                if($contadorRecibos < 1){
                        echo "<tr><td colspan=4 align=center>NO HAY MOVIMIENTOS QUE MOSTRAR</td></tr>";
                }    
                echo " </tr>";
            ?>
        </form>
        </table> 
    </div>
    <?php
}


/**
 * Función usada para calcular el campo saldo contable.
 * 
 * @param type $saldo le pasamos el saldo que hay en ese momento.
 * @param type $datos le pasamos el índice que debemos sumar.
 * 
 * @return type nos devuelve la suma del saldo.
 */
function calcular_Saldo_Contable(&$saldo, $datos)
{
    $saldo += $datos;
    return $saldo;
}


/**
 * Función encargada de devolver los diferentes recibos
 * 
 * @param type $bd La base de datos
 * @param type $login el login del usuario
 * @param type $tipoUltimos El tipo de mensaje que se le pasa
 * @param type $nombre El nombre completo del usuario para la cabecera
 */
function devolverRecibos($bd, $login, $tipoUltimos, $nombre) 
{  
    // La variable es utilizada para contar los recibos que hay, si no existe ninguno pasamos un mensaje.
    $contadorRecibos = 0; 
    
    ?>
     <div>
        <table  border="2px" style="margin: 1% auto 1% ">
            <form method=post action=$_SERVER[PHP_SELF]>
                <tr><td colspan=4 align=center><b><?php echo $tipoUltimos ?></b></td></tr>           
                <tr>
                    <th>Fecha</th>
                    <th>Concepto</th>
                    <th>Cantidad</th>
                    <?php
                        // En caso de que si sea una devolución.
                        echo "<th>DEVOLVER</th>"; 
                        echo "</tr>";
                        try{
                            // Preparamos la consulta
                            $stmt = $bd->prepare('SELECT * FROM Movimientos WHERE loginUsu like :loginUsu ORDER BY codigoMov + 0');
                            
                            // La ejecutamos
                            $stmt->execute(array(':loginUsu' => $login));

                            // Mientras existan datos
                            while ($datos = $stmt->fetch()) {

                                if($datos[0]){

                                    $cantidadDinero = $datos['cantidad'];        
                                    if($cantidadDinero < 0){
                                
                                        // Mostramos los diferentes datos en la tabla.
                                        echo " <tr>";
                                        echo "  <td align=center><small>".$datos['fecha']."</td>";
                                        echo "  <td align=center><small>".$datos['concepto']."</td>";
                           
                                        // Si el valor de la cantidad es menor de cero lo ponemos en color rojo.
                                        if($datos['cantidad'] < 0){
                                        
                                            echo "<td align=center><small><font color=red>".round($datos['cantidad'], 2)." €</font></td>";
                            
                                            // en caso contrario lo dejamos normal.    
                                            }else{
                                                echo "<td align=center><small>".round($datos['cantidad'], 2)." €</td>";
                                            }
                            
                                        // Si pulsamos sobre le número que aparece junto a los datos se le pasará el indice a borrar.
                                        echo " <td align=center><small><input type=submit name='codigoMov' value='$datos[codigoMov]' /></td>";
                                        echo " <input type=hidden name=nombre value='$nombre' />";
                                        echo " <input type=hidden name=login value='$login' />";
                                        echo " <input type='hidden' name='eleccion' value='Devolverlo'/>";
                            
                                        // Sumamos uno al contador de recibos
                                        $contadorRecibos++;
                                    }      
                                }
                            }
                            
                        // En caso de excepción la capturamos    
                        } catch (PDOException $ex) {
                            // Avisamos de que se ha producido un error con la conexión
                            echo "<center><b><h2>Se ha producido un error con la Base de Datos póngase en contacto con el administrador de la Web</h2></b></center>";
        
                            // Mostramos el tipo de error por pantalla (recomendable para saber dónde se encuentra exactamente el error).
                            echo "Error: " .$ex->getMessage();
                        }
                        
                
                        // Si el contador de recibos es menor de un avisamos por pantalla
                        if($contadorRecibos < 1){
                            echo "<tr><td colspan=4 align=center>NO HAY RECIBOS PARA DEVOLVER</td></tr>";
                        }    
                    echo " </tr>";
                ?>
        </form>
        </table> 
    </div>
    <?php
}

/**
 * Función encargada de comprobar los diferentes datos.
 * 
 * @param type $login El login del usuario.
 * @param type $password El password de usuario.
 * @param type $passwordRepe La comprobación del password introducido.
 * @param type $nombre El nombre del usuario.
 * @param type $fnacimiento La fecha de nacimiento del usuario.
 * @param type $fecha La fecha de ingreso o pago de recibo.
 * @param type $concepto El concepto del ingreso o pago.
 * @param type $cantidad La cantidad del ingreso o pago
 * @param type $tipoUsuario El tipo de usuario.
 * @return string El mensaje con los posibles errores.
 */
function comprobarDatos($login, $password, $passwordRepe, $nombre, $fnacimiento, $fecha, $concepto, $cantidad, $tipoUsuario){
    
    // Comprobamos el navegador
    $info = detect();
    
    // Variable encargada de los mensajes de error.
    $errores = "";
    
    // Si los datos a comporbar son para introducir usuarios
    if($tipoUsuario){
        
        // Comprobamos que el login no se encuentra vacío
        if($login == ""){
            $errores .= "Error: No has introducido un login.<br>";
        }
                    
        // Comprobamos que el password no se encuentra vacío
        if($password == ""){
            $errores .= "Error: No has introducido el primer password.<br>";
        }
                    
        // Comprobamos que el password de comprobación no se encuentra vacío
        if($passwordRepe == ""){
            $errores .= "Error: No has introducido la comprobación del password.<br>";
        }
                    
        // Comprobamos que el nombre no se encuentra vacío
        if($nombre == ""){
            $errores .= "Error: No has introducido un nombre.<br>";
        }
                    
        // Comprobamos que la fecha de nacimiento no se encuentra vacío
        if($fnacimiento == ""){
            $errores .= "Error: No has introducido una fecha de nacimiento correcta.<br>";
                        
        // en caso de que no se encuentre vacio comprobamos que la edad es mayor de 18
        }else{
            
            // Comprobamos si es un navegador sin soporte para input date
            if($info['browser'] == "OTHER"){
                
                if (!preg_match("/([0-9]{2})\/([0-9]{2})\/([0-9]{4})/", $fnacimiento)) {
                    $errores .= "Error: El formato de fecha no es válido.<br>";    
                }           
            }
             
            // Obtenemos el año de la fecha actual
            $fecha_actual = date("Y");
            
            // Octenemos el año de la fecha introducida.
            $fecha_introducida = date("Y", strtotime($fnacimiento));

            // Restamos ambos años
            $comparacion_fechas = $fecha_actual - $fecha_introducida;
                        
            // Comprobamos ambas fechas(solo el año).
            if($comparacion_fechas < 18){
                
                // en caso de tener menos de 18 años aavisamos
                $errores .= "Error: El usuario tiene menos de 18 años.<br>";
            } 
        }
                    
        // Comprobamos que los dos password no son iguales
        if(!($password == $passwordRepe)){
            $errores .= "Error: La contraseña de comprobación del password no es igual al password<br>";     
        }

    // en caso contrario será para hacer ingresos o pagos    
    }else{
        
        // Comprobamos que la fecha no se encuentra vacía
        if($fecha == ""){
            $errores .= "Error: No has introducido la fecha<br>";
                           
        // en caso de que no se encuentre vacía y para aprender más sobre fechas
        // he escogido en esta ocasión que la fecha debe ser igual al día en el
        // que nos encontramos, la comprobación la hago de una forma distinta a la
        // anterior (fecha de nacimiento).
        }else{
            
            // Comprobamos si es un navegador sin soporte para input date
            if($info['browser'] == "OTHER"){
                
                // Comprobamos si se adapta a los requisitos de formato de fecha
                if (!preg_match("/([0-9]{2})\/([0-9]{2})\/([0-9]{4})/", $fecha)) {
                    
                    // en caso de que no se avisa por pantalla.
                    $errores .= "Error: El formato de fecha no es válido.<br>";    
                }
                
                // He probado durante toda una mañana hacerlo con funciones de tiempo
                // y no ha funcionado sobre IE, esto es lo único que ha funcionado.
                $fechaPuesta = str_replace('/', '-', $fecha);
                
            }else{  
                // Pasamos la fecha al mismo formato
                $fechaPuesta = date_format(date_create($fecha), 'd-m-Y');
            }
            
            // Comprobamos la fecha de hoy formato 1
            $fechaHoy = date("d-m-Y");
            
            // Comprobamos la fecha de hoy formato 2
            $fechaHoyF = date("d/m/Y");

            // La fecha de ingreso debe ser igual al día en el que se está.
            if($fechaHoy != $fechaPuesta){
                $errores .= "Error: La fecha debe ser igual a hoy: $fechaHoyF.<br>";              
            }
        }         
          
        // Comprobamos que el password no se encuentra vacío
        if($concepto == ""){
            $errores .= "Error: No has introducido el concepto<br>";              
        }
                    
        // Comprobamos que el password de comprobación no se encuentra vacío
        if($cantidad < 1){
            $errores .= "Error: No has introducido una cantidad correcta (mayor de cero).<br>";      
        }
    }
    
    // Retornamos el valor
    return $errores; 
}

/**
 * Función creada para la cabecera de la web.
 * 
 * @param type $title le pasamos el texto que deseamos que aparezca.
 */
function cabecera($titulo1, $titulo2)
{
    echo "<center><b><h2>$titulo1</h2></b></center>"; // Muestra el mensaje de cabecera.
    echo "<center><b><h3>$titulo2</h3></b></center>";
}

/**
 * Función creada para el pie de la página web.
 * 
 * @param type $nombreTarea le pasamos el nombre de la tarea.
 */
function pie($nombreTarea)
{
    //Muestra el mensaje del pie
    echo "<center><small>Rubén Ángel Rodriguez Leyva</br> 
          <center><small>$nombreTarea</br>
          </body>
          </html>";
    exit; 
}

/**
 * Comprobar el explorador web que se está utilizando para
 * input de tipo date lo he extraido de un web y lo he modificado 
 * levemente para adptarlo a mis necesidades.
 * 
 * @return string El nombre del explorador web
 */
function detect(){
    
    // Se crea un array con los diferentes tipos de exploradores que disponen
    // de input date.
    $browser=array("MOZILLA","NETSCAPE","FIREFOX","CHROME");
 
    // definimos un valores por defecto para el navegador
    $info['browser'] = "OTHER";
 
    // buscamos el navegador con su sistema operativo
    foreach($browser as $parent){
        $s = strpos(strtoupper($_SERVER['HTTP_USER_AGENT']), $parent);  
        $f = $s + strlen($parent);
        if ($s){
            $info['browser'] = $parent;
	}
    }

    // Devolvemos el array de valores
    return $info;
}