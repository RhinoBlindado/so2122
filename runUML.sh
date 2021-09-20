#!/bin/bash

# [CASTELLANO]
# 
# Descripción: Pequeño script que inicia una maquina virtual de Fedora con UML
# Asignatura: Sistemas Operativos
# Autor: Valentino Lugli
# Fecha: Septiembre 2021
# 
# [ENGLISH]
# 
# Description: Small script that starts up a Fedora VM using UML
# Course: Sistemas Operativos
# Author: Valentino Lugli
# Date: September 2021

cp /fenix/depar/lsi/UML/*.gz /tmp
gunzip /tmp/*.gz
tmp/kernel32-3.0.4 ubda=tmp/Fedora14-x86-root_fs mem=1024m