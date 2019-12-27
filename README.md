# Hangman

This project is an implementation of the game "Hangman" in C language. 
The program reads word lists from a file and the user has to guess words within at most 11 tries.
The word lists and game results are stored in a simple chained list.

## About the game

Hangman is a classic word-guessing game where one player thinks of a word and the other player tries to guess it by suggesting letters within a limited number of attempts. 
The word is represented by a series of dashes, each dash representing a letter in the word. 
The game proceeds until the word is correctly guessed or the limit of attempts is reached, 
resulting in the guessing player's loss. 
It's a fun and engaging game that challenges players' vocabulary and deduction skills.

## Project structure

* testfiles
  * test01.txt
  * test02.txt
  * test03.txt
  * test04.txt
* CMakeLists.txt
* Dockerfile
* main.c
* README.md

## Requirements

* C >= 99
* CMake Version >= 3.10

## Usage

The easiest way to set up the project is to build a Docker image based on the provided Dockerfile and Makefile to use it within an interactive session. 
  
    $ docker build -t hangman .
    $ docker run --rm -it hangman

Alternatively, required build dependencies can be installed directly, e.g., on Ubuntu using CMake:

    $ mkdir build
    $ cmake -S ./ -B .
    $ make

Run the executable and use test file path as the program argument:
    
    $ ./main ../testfiles/<testfile>.txt

**Example:** ./main ../testfiles/test02.txt

