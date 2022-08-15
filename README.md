# KNN Classification Server
This project allows a client to send unclassified Iris-flowers data to a classification server, 
and save the computed results locally.

## Communication
The server and client communicate over TCP, port 5000.
The default buffer size is 4096, and the default server IP is localhost.

The server address can be changed in `client_main.cpp`, and the 
port in `common.hpp`.

## Organization
The client's entry point is in `client_main.cpp`, and the
server's entry point is in `server_main.cpp`.

Client specific code (Communication related) is found in the `client`
directory, and server specific code (classifier, server communication code)
is in the `server` directory; code for dealing with 
classification-related mechanisms, such as csv parsing and the KNN algorithm itself, is
found in `server/classifier`.

Code that's shared between both - file sharing over TCP, for instance - is in `common.*`.

## Usage
The server can be run without any arguments, and the client as
```shell
<client> /path/to/unclassified_data /path/to/classified_results
```

## Installation
The executables for the client and the server are found in the repo 
itself, `out_client` and `out_server`. Alternatively, you can create them yourself:
```shell
g++ -std=c++11 -o out_server server_main.cpp common.cpp server/server.cpp server/classifier/csv.cpp server/classifier/metric.cpp
g++ -std=c++11 -o out_client client_main.cpp common.cpp client/client.cpp
```