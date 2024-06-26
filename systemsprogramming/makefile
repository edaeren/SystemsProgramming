CLASSDIR = .\Libfdr
INCLUDE = -I$(CLASSDIR)\include
LIBS = $(CLASSDIR)\lib\libfdr.a
CC = gcc

EX  =  bin\odev.exe

all: $(EX) 

clean: 
	del bin\*

bin\odev.exe: src\odev.c $(LIBS)
	$(CC) $(INCLUDE) -o bin\odev.exe src\odev.c $(LIBS)
