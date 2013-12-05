Laboratorio de Redes de Comunicación I/II
Entrega Lab 01: Miércoles 11 de Diciembre 2013

El programa se desarrollará en OMNet++ 4.3.1 usado JRE
http://www.omnetpp.org/

Tras descargar el archivo deben asegurarse de descomprimirlo en una ruta de acceso
sin espacios.
En omnetpp-4.3.1\doc\InstallGuide.pdf pueden encontrar todas las instrucciones
para instalar el programa.

En la sección de Bitbucket pueden encontrar una Guía práctica para empezar con 
OMNet++, también existe un ejemplo que es bastante similar a lo que tenemos que
desarrollar, sin embargo el ejemplo está escrito en una versión anterior a la 3.X,
por lo que hay tener cuidado al re-programar el ejemplo. Existe un archivo en la 
sección de descargas donde se pueden ver las principales diferencias de versiones.

En el siguiente link pueden encontrar toda la información relevante a la
programación en OMNet++ 4.3.1: http://omnetpp.org/doc/omnetpp/manual/usman.html

El enunciado del Laboratorio está en la sección Downloads.

*************** IMPORTANTE ***************
El contenido de este Bitbucket corresponde a el Project/Workspace de OMNet++,
por lo que no se recomienda agregar archivos externos al IDE en el repositorio.
Se recomienda tener el repositorio en /samples/Laboratorio_Redes_01_02
(o el nombre de preferencia) de la carpeta de OMNet++ para que sea más directa
la compilación y la sincronización de los archivos.

Para Empezar a trabajar deben hacer un clone del proyecto, pueden presionar
la opción que se encuentra arriba o usar:
   git clone https://Theby@bitbucket.org/Theby/lab-redes-de-comunicaci-n-i-ii.git
Como comando directamente en la consola Git Bash.
RECORDAR: Hacer la copia en la carpeta samples/nombre_carpeta dentro de la 
carpeta de OMNet++.
Luego pueden realizar un push para actualizar información:
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

Si van a realizar pruebas que pueden arruinar todo les recomiendo hacer
un Fork/Branch del Proyecto para hacer un merge después, si es que
todo funciona bien.

Si encuentran algun error/bug o suben contenido que aún no esta terminado
y por lo tanto presenta errores, es ALTAMENTE recomendable que creen un
Issue en la pestaña Issue de Bitbucket, para que no se nos pasen errores
sin darnos cuenta. Tenganlo presente!!.




