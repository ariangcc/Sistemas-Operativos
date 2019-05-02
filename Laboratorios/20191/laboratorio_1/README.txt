****************************

PREPARACIÓN LABORATORIO 1
tema: comunicación entre procesos relacionados

****************************

Conceptos importantes antes de iniciar: 
 
1. llamada al sistema: 

En computación, una llamada al sistema es la forma programática en la que un programa de computadora solicita un servicio desde el kernel del sistema operativo en el que se ejecuta. Una llamada al sistema es una forma en que los programas interactúan con el sistema operativo. Un programa informático realiza una llamada al sistema cuando realiza una solicitud al kernel del sistema operativo. La llamada al sistema proporciona los servicios del sistema operativo a los programas del usuario a través de la interfaz del programa de aplicación (API). Proporciona una interfaz entre un proceso y un sistema operativo para permitir que los procesos a nivel de usuario soliciten servicios del sistema operativo. Las llamadas al sistema son los únicos puntos de entrada al sistema del kernel. Todos los programas que necesiten recursos deben usar llamadas al sistema.

los servicios a los que se puede acceder mediante una llamada al sistema son: Creación y gestión de procesos, gestión de memoria, accesos a archivos, manejo de dispositivos, etc.


2. funciones importantes: 

- getpid(): 
pertenece a la biblioteca unistd.h y devuelve el id del proceso actual

- getppid(): 
pertenece a la biblioteca unistd.h y devuelve el ID del proceso padre del proceso actual, nunca arroja error

Nota
En algún momento del tiempo, no es necesario que el proceso hijo se ejecute primero o que el proceso padre se asigne primero a la CPU, cualquier proceso puede tener la CPU asignada, en algún momento. Además, la identificación del proceso puede diferir durante las diferentes ejecuciones.


- getuid() : 
pertenece a la biblioteca unistd.h y retorna el UserID real del proceso llamado, este UserID real es la cuenta del propietario de este proceso 

- fork():  
crea un nuevo proceso el cual es llamado 'proceso hijo' que se ejecuta simultaneamente (multiprogramación) con el 'proceso padre', despues de su creación ambos procesos, padre e hijo se ejecutan a partir de la siguiente instruccion de la llamada a fork().

teniendo pid = fork():

si pid es igual a 0 significa que se ha creado el proceso hijo y la ejecucion continua a partir de este proceso hijo (estamos en el proceso hijo)

si pid es mayor a 1 significa que estamos en el proceso padre, el valor de este pid es el del proceso hijo recien creado.

- wait(int *status): 
los estados se espera se usan para esperar cambios de estado de un proceso hijo (si se detuvo, si esta detenido o si esta siendo reanudado). esta llamada al sistema suspende la ejecucion de proceso de llamada hasta que uno de sus procesos hijos termine.


si hay exito retorna el id del proceso del proceso hijo terminado , -1 si hay errores. 

- waitpid(pid_t pid, int *status, int options): 
esta llamada al sistema suspende la ejecucion del proceso de llamada hasta que un proceso hijo especificado por el pid como argumento cambie de estado.o hasta que  se  produce  una  señal  cuya acción es finalizar el proceso actual o llamar a la función manejadora de la señal. por default waitpid() espera solo por el termino de un proceso hijo pero como vimos puede esperar por otros estados.


- fflush(): 
se usa normalmente para el flujo de salida solamente. Su propósito es limpiar (o vaciar) el búfer de salida y mover los datos almacenados a la consola (en caso de stdout) o disco (en caso de flujo de salida de archivo).

El valor de pid puede ser uno de los siguientes:

       < -1   lo  que significa esperar a que cualquier proceso hijo cuyo ID del proceso es 		      igual al valor absoluto de pid.

       -1     lo que  significa  que  espera  por  cualquier  proceso  hijo;  este  es  el  		      mismo comportamiento que tiene wait.

       0      lo  que  significa  que  espera  por cualquier proceso hijo cuyo ID es igual al 		      del proceso llamante.

       > 0    lo que significa que espera por el proceso hijo cuyo ID es igual al valor de pid.

El valor de options es un OR de cero o más de las siguientes constantes:

       WNOHANG
              que significa que vuelve inmediatamente si ningún hijo ha terminado.

       WUNTRACED

Si status no es NULL, wait o waitpid almacena la  información  de  estado  en  la  memoria
apuntada por status.


*************************************************************
qué es el descriptor de archivo? 
es el numero entero que identifica de forma unica un archivo abierto del proceso

que es la tabla de descriptores de archivos? 
es la coleccion de indices de matriz de enteros que son descriptores de archivos en los
que los elementos son punteros a las entradas de tablas de archivos

que es la entrada de la tabla de archivos: 
las entradas de la tabla de archivos son una estructura sustituta en memoria para un archivo
abierto.que se crea cuando la solicitud de proceso abre el archivo y estas entradas mantienen la posición del archivo.

Descriptor de archivos estandar: 
cuando cualquier proceso inicia, la tabla de descriptores de archivo de ese proceso ,0, 1, 2 se abre automáticamente, (De forma predeterminada) cada una de estas entradas de tabla de archivo de referencias de 3 fd para un archivo llamado / dev / tty

Leer desde stdin => leer desde fd 0 : cada vez que escribimos un carácter desde el teclado, se lee desde stdin hasta fd 0 y se guarda en el archivo llamado / dev / tty. 

Escribir a stdout => escribir a fd 1 : cada vez que veamos una salida a la pantalla de video, es desde el archivo llamado / dev / tty y escrito a stdout en la pantalla a través de fd 1. 

Escriba a stderr => escriba a fd 2 : Nosotros vea cualquier error en la pantalla de video, también es de ese archivo, escriba a stderr en pantalla a través de fd 2.
*************************************************************

- pipe(int fds[2]):
es una conexión entre dos procesos de modo que la salida estándar de un proceso se convierte en la entrada estándar del otro, estas sirven para la comunicación entre procesos, esta comunicacion es unidireccional.

fd[0] sera el descriptor de archivo de lectura 
fd[1] sera el descriptor de archivo de escritura

devuelve 0 en caso de exito -1 fracaso 
los pipes se comportan  como FIFO primero en entrar primero en salir, cola
el tamaño de lectura y escritura no tiene que coincidir aquí. Podemos escribir 512 bytes a la vez pero solo podemos leer 1 byte a la vez en una canalización

una ventaja del pipe es que una vez llamado, los descriptores seguiran abiertos en todos los procesos secundarios y tambien en el principal.

BELLO DICE:
 
un pipe puede estar vacio, si algun proceso intenta leer de aqui se bloquea hasta que haya algo en el pipe, tambien se bloquea cuando el pipe esta lleno.

cuando se hace open en un archivo, recibo un descriptor de archivo este descriptor me sirve tanto para leer como para grabar en el archivo.

en el pipe se necesita 2 descriptores el 0 es de lectura el 1 de escritura

cerrar pipes es importante pues una vex que algo se lee el pipe queda limpio, esto hace que si algun otro proceso intenta leer va a bloquearse.


al iniciarse un programa los descriptores de archivo son por defecto 0 1 2 luego cuando se hace pipe estos toman 3 4 de la tabla de descriptores pero en el codigo es  0 1 del arreglo 

PARA EVITAR ALGUN TIPO DE INTERBLOQUEO Y PORQUE SE SABE QUE EL FLUJO DEL PIPE ES UNIDIRECCIONAL SE TIENE QUE ESPECIFICAR QUE POR CADA PIPE HAY UN SOLO PROCESO DE LECTURA Y UN SOLO PROCESO QUE ESCRIBE


- dup(oldfd):
crea  una copia del descriptor de archivo oldfd y usa el menor espacio disponible o sea el primer espacio que encuentra para colocar ahi la copia

- dup2(oldfd, newfd):
ya no lo crea en cualquier parte, lo crea en el nuevo file descriptor













