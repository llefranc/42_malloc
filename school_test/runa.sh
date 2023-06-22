#!/bin/sh
echo "\t\033[1;34m***********\033[0m"
echo "\t\033[1;34m** TESTS **\033[0m"
echo "\t\033[1;34m***********\033[0m"

path="."
export LD_LIBRARY_PATH=$path
export LD_PRELOAD=$path/libft_ademalloc.so
$@
