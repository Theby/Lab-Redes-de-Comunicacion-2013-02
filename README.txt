Laboratorio de Redes de Comunicación I/II
Entrega Lab 01: Miércoles 11 de Diciembre 2013
Entrega Lab 02: Miécoles 8 de Enero 2014

El programa se desarrollará en OMNet++ 4.3.1 usado JRE
http://www.omnetpp.org/

Tras descargar el archivo deben asegurarse de descomprimirlo en una ruta de acceso
sin espacios.
En omnetpp-4.3.1\doc\InstallGuide.pdf pueden encontrar todas las instrucciones
para instalar el programa.

En el siguiente link pueden encontrar toda la información relevante a la
programación en OMNet++ 4.3.1: http://omnetpp.org/doc/omnetpp/manual/usman.html

También existe un transformador de archivos para pasar las versiones antiguas
a la nueva, para ello deben ingresar con la consola de OMNet++ a la carpeta
migrate/ y ejecutar la aplicación migrate que corresponde, la que transformará
los archivos iterativamente.

Todos los documentos, como guias, enunciados, tutoriales, TODO; pueden ser
encontrados en la carpeta Documentos del programa.

*************** IMPORTANTE ***************
El contenido de este Git corresponde a el Project/Workspace de OMNet++.
Se recomienda tener el repositorio en /samples/Laboratorio_Redes_01_02
(o el nombre de preferencia) de la carpeta de OMNet++ para que sea más directa
la compilación y la sincronización de los archivos.

Para bajar GIT:
https://code.google.com/p/msysgit/downloads/list

--- GIT BASH

Para Empezar a trabajar deben hacer un clone del proyecto:
   git clone https://myuser@github.com/Theby/Lab-Redes-de-Comunicacion-2013-02.git myfolder
Tal que:
   myuser: tu nombre de usuario
   myfolder: nombre de la carpeta donde se ubicará el proyecto, si no se especifica tendrá
   el mismo nombre que el repositorio.

RECORDAR: Hacer la copia en la carpeta samples/nombre_carpeta dentro de la 
carpeta de OMNet++.

Luego pueden realizar un push para actualizar información, aunque esto
no es necesario la primera vez:
   git push origin master
      -> introduzco clave

No olviden realizar un commit claro cada vez que realicen un push.
   Ejemplo:
      (agrega todos los archivos modificados y no actualizados)
      git add --all 
         o bien
      (agrega el archivo especificado)
      git add nombre.asdf
		
      git commit -m "Explicando que cambio se ha realizado"
	  git push origin master
         -> introduzco clave
			
Antes de empezar a trabajar en algo nuevo recuerden hacer un pull para
actualizar el contenido de sus pc con el contenido actual del repositorio:
   git pull origin master
      -> introduzco clave

Ni los ejecutables ni la carpeta out/ se sube al repositorio, esto debido
al .gitignore, por lo que estos archivos pueden ser de cualquier naturaleza
en sus computadores.

Si van a realizar pruebas que pueden arruinar todo, les recomiendo hacer
un Fork/Branch del Proyecto para hacer un merge después, si es que
todo funciona bien.

En caso de existir un branch, el Master no debe ser modificado hasta que se
realize un MERGE.

Si encuentran algun error/bug o suben contenido que aún no esta terminado
y por lo tanto presenta errores, es ALTAMENTE recomendable que creen un
Issue en la pestaña Issue de Bitbucket, para que no se nos pasen errores
sin darnos cuenta. Tenganlo presente!!.




--- GIT GUI:

Esto permite trabajar con interfaz grafica, todo esta allí, solo deben elegir
la carpeta del respositorio y todo se hace por botones.

En lo personal prefiero GIT BASH



