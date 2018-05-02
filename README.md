# DWES02
Desarrollo Web Entorno Servidor: Tarea 2

Debes crear una aplicación en PHP que gestione la banca electrónica del banco Forrare que vimos en la tarea anterior, añadiéndole otras funcionalidades. En esta ocasión TODOS los datos serán almacenados en una base de datos en MySQL llamada banca_electronica (se adjunta el script para la creación de la base de datos: banca_electronica3.sql

El acceso a la base de datos debe hacerse a través del usuario dwes con contraseña dwes. La conexión debe hacerse mediante PDO.

En la página principal (index.php)  se debe mostrar el menú que de acceso a:

  Administrar usuarios: gestión de altas, bajas y modificaciones de los usuarios que pueden acceder a la banca electrónica.
  Para acceder mostramos un formulario en el que solicitamos el login de administrador y la contraseña (ocultando los           caracteres de la contraseña). El login y contraseña de administrador siempre serán los mismos que para acceder a la base de   datos (usuario dwes con contraseña dwes). Si lo introduce bien se mostrará un nuevo menú:
  Nuevo usuario: Introducimos un nuevo usuario, comprobando que su login no existe en el sistema, que sus datos personales no   están vacíos y que la contraseña se introduce 2 veces y es coincidente.
  Modificar usuario: A partir de un login de usuario, mostraremos sus datos y solicitaremos la modificación de los mismos,       excepto el login de usuario.
  Borrar usuario: A partir de un login, eliminaremos un usuario y todos sus movimientos de banca electrónica.
  Salir: Volvemos al menú principal.
  Iniciar sesión: Mostramos un formulario en el que solicitamos el login de usuario y su contraseña (ocultando los caracteres   de la contraseña). Comprobamos si está registrado en el sistema y en caso afirmativo le permitimos el acceso a la banca       electrónica.
  Mostramos en el margen superior el nombre y apellidos del usuario y a continuación el menú de la banca electrónica, que no     es otro que el desarrollado en la tarea anterior, añadiéndoles una nueva opción:
  Últimos movimientos: Muestra los últimos 4 movimientos.
  Ingresar dinero: Formulario de ingreso de dinero.
  Pagar recibo: Formulario de pago de recibos.
  Devolver recibo: Formulario de devolución de recibos.
  Salir: Volvemos al menú principal.
El menú con el que estemos navegando en ese momento, debe aparecer en todas las páginas para permitir la navegación (menú en la parte superior). Por ejemplo, si estamos ingresando dinero, en la parte superior debe aparecer el menú de banca electrónica completo.

Todas las operaciones de inserción/almacenamiento, modificación, eliminación y consulta, deberán realizarse mediante el uso de la base de datos.

Deben usarse funciones al menos para el acceso a la base datos: conexión, acceso y manipulación de datos, etc. Todas estas funciones deben estar definidas en un fichero llamado funciones.inc que debe ser incluido en todas las páginas de la aplicación.

Debe gestionarse el manejo de errores al menos siempre que se trabaja con la base de datos. Además deberás avisar cuándo las tablas están vacías, si no existe tal registro, etc.
