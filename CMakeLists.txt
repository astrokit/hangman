cmake_minimum_required(VERSION 3.10)
project(hangman C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_C_FLAGS "-Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -O1")
#set(CMAKE_C_FLAGS "-Wall -Wextra -fsanitize=leak -fno-omit-frame-pointer -O1")
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(main main.c)