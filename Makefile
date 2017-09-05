HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += -I/usr/local/include -pthread -g
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib64 -L/usr/lib64 -lgrpc++       \
           -lprotobuf -lpthread \
	   -lz -lgrpc++_unsecure -lgrpc_unsecure -lgpr -lgrpc_csharp_ext -ljsoncpp
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = .

vpath %.proto $(PROTOS_PATH)

#all: system-check greeter_client greeter_server greeter_async_client greeter_async_client2 greeter_async_server

main_server: latest_Server.o relay_server.pb.o relay_server.grpc.pb.o functions.o Classes.o 
	$(CXX) $^ $(LDFLAGS) -o $@

clean : 
	rm -f *.o main_server
