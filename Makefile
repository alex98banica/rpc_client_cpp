
build:
	rpcgen -C database.x
	g++ -o fileserver -g server.cpp database_svc.c database_xdr.c -lnsl
	g++ -o fileclient -g client.cpp database_clnt.c database_xdr.c -lnsl

clean:
	rm -f fileclient fileserver database_* databases_* database.h