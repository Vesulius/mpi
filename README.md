# Mini pascal interpreter

Interpreter for the (very) simple pascal-inspired language minipascal. 

This is a cource project for University of Helsinki.

## Documentation

[Documentation for project](https://github.com/Vesulius/mpi/blob/main/documentation.md)

## Instructions

The interpreter is compiled using **g++** compiler (version 11.3.0). There are two settings for the compilation: normal and debug mode.

**Default compilation:**
``` g++ ./*.cpp -o mpi ```
This should produce regular executable binary file named **mpi**. 

**Debug compilation:**
``` g++ ./*.cpp -D DEBUG -o mpi ```
This should produce regular executable binary file named **mpi**.  Debug mode prints additional info about the interpreter such as tokens and abstract syntax tree when run.

**RUN:**  
``` ./mpi YOUR_MINIPASCAL_SOURCE_FILE ```