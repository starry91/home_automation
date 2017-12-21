bin_PROGRAMS=homeserver
homeserver_SOURCES=Appliances.cpp AppToCpp_Server.cpp AppToCppServer.grpc.pb.cc AppToCppServer.pb.cc Configuration.cpp CppServerMain.cpp Data.cpp ExtFunctions.cpp RelayServerClient.cpp relay_server.grpc.pb.cc relay_server.pb.cc Room.cpp Server.cpp
homeserver_LDFLAGS=-L/usr/local/lib64 -pthread -lz -ljsoncpp
LOCATION=/usr/local/lib64
homeserver_LDADD=${LOCATION}/libgrpc++.a ${LOCATION}/libgrpc_unsecure.a ${LOCATION}/libgrpc++_unsecure.a ${LOCATION}/libprotobuf.a ${LOCATION}/libgpr.a ${LOCATION}/libcares.a
homeserver_CXXFLAGS=-I../include -I/usr/local/include
