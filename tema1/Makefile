build:
	gcc -o server tema1_rpc_server.c tema1_svc.c tema1_xdr.c sc_map.c -lnsl -Wall
	gcc -o client tema1_rpc_client.c tema1_clnt.c tema1_xdr.c sc_map.c -lnsl -Wall

build-rpc:
	rpcgen -C tema1.x

clean:
	rm -f client server tema1.h tema1_svc.c tema1_clnt.c tema1_xdr.c
