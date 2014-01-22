#Target del Makefile
TARGET = BlackJack

#Nombre del ejecutable final
EXEC_S = Servidor_BlackJack
EXEC_C = Cliente_BlackJack

#Compilador a usar
CC = g++

#Librerias a usar
LIBS = 

#Compiler Comands
CFLAGS = -c

#Headers
HEADERS = $(wildcard *.h)

#Objects
OBJECTS = main_servidor.o main_cliente.o FuncionesExtras.o Carta.o Mazo.o Jugador.o Socket.o SocketServidor.o Servidor.o SocketCliente.o Cliente.o

#Carpeta de Objetos
OUT = out/

OWR_S = $(OUT)main_servidor.o $(OUT)FuncionesExtras.o $(OUT)Carta.o $(OUT)Mazo.o $(OUT)Jugador.o $(OUT)Socket.o $(OUT)SocketServidor.o $(OUT)Servidor.o
OWR_C = $(OUT)main_cliente.o $(OUT)FuncionesExtras.o $(OUT)Carta.o $(OUT)Mazo.o $(OUT)Jugador.o $(OUT)Socket.o $(OUT)SocketCliente.o $(OUT)Cliente.o

default: $(TARGET)
all: default

$(TARGET): $(OBJECTS)
		$(CC) $(OWR_S) -o $(EXEC_S) $(LIBS)
		$(CC) $(OWR_C) -o $(EXEC_C) $(LIBS)

%.o: %.cpp $(HEADERS)
		$(CC) $(CFLAGS) $< -o $(OUT)$@ $(LIBS)

clean:
		rm -rf $(OUT)*o ($TARGET)
