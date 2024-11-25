#!/bin/bash

# Compile the server
gcc -o time_service_server time_service_server.c time_service_svc.c time_service_xdr.c -I/usr/include/tirpc -ltirpc

# Compile the client
gcc -o time_service_client time_service_client.c time_service_clnt.c time_service_xdr.c -I/usr/include/tirpc -ltirpc
