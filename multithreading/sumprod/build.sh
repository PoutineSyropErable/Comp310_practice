#/bin/bash
rm core.*

gcc -g -pthread -o a a.c


ulimit -c unlimited
./a
