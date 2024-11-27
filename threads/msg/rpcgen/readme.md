# README: Generating `time_service` Using `rpcgen`

This guide explains how to generate and compile the server and client code for `time_service` using `rpcgen`. Ensure all necessary files are in the same directory.

---

## Prerequisites

1. **Install Required Tools**:
   - Install `rpcgen` and `rpcbind` on your system:
     ```bash
     sudo apt install rpcbind rpcgen  # For Debian-based distros
     ```
     or
     ```bash
     sudo pacman -S rpcbind rpcgen  # For Arch-based distros
     ```

2. **Start `rpcbind` Service**:
   - Ensure `rpcbind` is running:
     ```bash
     sudo systemctl start rpcbind
     ```

---

## Step-by-Step Instructions

### 1. Generate Code Using `rpcgen`

Run the following command in the directory containing `time_service.x`:

```bash
rpcgen time_service.x


is generates the following files:

    time_service.h: Header file with function prototypes and constants.
    time_service_clnt.c: Client-side stub code.
    time_service_svc.c: Server-side stub code.
    time_service_xdr.c: Serialization/deserialization (XDR routines).
    time_service_xdr.h: Header file for XDR routines.

2. Compile the Server

Compile the server code using the generated time_service_svc.c and time_service_xdr.c, along with your custom server logic file (time_service_server.c):

gcc -o time_service_server time_service_svc.c time_service_xdr.c time_service_server.c -lnsl

3. Compile the Client

Compile the client code using the generated time_service_clnt.c and time_service_xdr.c, along with your custom client logic file (time_service_client.c):

gcc -o time_service_client time_service_clnt.c time_service_xdr.c time_service_client.c -lnsl

4. Run the Server

Start the server to listen for client requests:

./time_service_server

5. Run the Client

Execute the client to send requests to the server:

./time_service_client

File Reference

Ensure the following files are in the same directory:

    time_service.x: The .x file defining the RPC interface.
    time_service_server.c: Custom server logic file.
    time_service_client.c: Custom client logic file.
    Generated files: time_service.h, time_service_clnt.c, time_service_svc.c, time_service_xdr.c, time_service_xdr.h.

Example Workflow

    Generate Code:

rpcgen time_service.x

Compile Server:

gcc -o time_service_server time_service_svc.c time_service_xdr.c time_service_server.c -lnsl

Compile Client:

gcc -o time_service_client time_service_clnt.c time_service_xdr.c time_service_client.c -lnsl

Run Server:

./time_service_server

Run Client:

    ./time_service_client

Follow these steps to successfully generate, compile, and run the time_service using rpcgen.
