tmp_protogen_dir=generated
all-am: gen_proto_files
gen_proto_files:
	mkdir ${tmp_protogen_dir}
	protoc --cpp_out=${tmp_protogen_dir} --grpc_out=${tmp_protogen_dir} --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin -Iproto AppToCppServer.proto relay_server.proto
	mv ${tmp_protogen_dir}/AppToCppServer.grpc.pb.h include/
	mv ${tmp_protogen_dir}/AppToCppServer.pb.h include/
	mv ${tmp_protogen_dir}/relay_server.grpc.pb.h include/
	mv ${tmp_protogen_dir}/relay_server.pb.h include/
	mv ${tmp_protogen_dir}/AppToCppServer.grpc.pb.cc src/
	mv ${tmp_protogen_dir}/AppToCppServer.pb.cc src/
	mv ${tmp_protogen_dir}/relay_server.grpc.pb.cc src/
	mv ${tmp_protogen_dir}/relay_server.pb.cc src/

SUBDIRS=src
