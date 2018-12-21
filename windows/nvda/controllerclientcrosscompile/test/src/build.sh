#!/bin/bash

#CCPREFIX=/usr/bin/i686-w64-mingw32-
CCPREFIX=/usr/bin/x86_64-w64-mingw32-

${CCPREFIX}g++ -c test_nvda_controller_client.cpp

${CCPREFIX}g++ -o test_nvda_controller_client.exe \
    test_nvda_controller_client.o \
    -L. -lnvdaController64
