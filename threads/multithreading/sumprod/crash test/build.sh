#!/bin/bash

#echo "core.%e.%p.%t" | sudo tee /proc/sys/kernel/core_pattern
# ^^ Run this so the core dumb can happen in the current dir after having backup whatever you had before

rm core.*


ulimit -c unlimited

gcc -g test_crash.c -o test_crash 

./test_crash


gdb ./test_crash ./core.test_crash.**


