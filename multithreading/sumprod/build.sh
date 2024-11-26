#/bin/bash
rm core.*

gcc -g -pthread -o multithreaded_sumprod multithreaded_sumprod.c


ulimit -c unlimited
./multithreaded_sumprod
