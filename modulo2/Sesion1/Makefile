# [CASTELLANO]
#	Descripción: Makefile para generar ejecutables C dando un nombre por parámetro
#	Uso: make n=<nombre_fichero_sin_extension>
# 	Asignatura: Sistemas Operativos
#	Autor: Valentino Lugli (Github: @RhinoBlindado)
# 	Fecha: Noviembre 2021

code = $(n)

$(code)_EXE: $(code).c
	gcc -o -lm $@ $<

clean:
	rm *_EXE
