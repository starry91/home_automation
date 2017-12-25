dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(home-automation, 0.1)
CXXFLAGS="-std=c++14"

AC_CONFIG_COMMANDS([cpp-proto-generate],[
tmp_protogen_dir=generated
echo "Generating cpp proto files from: $(ls proto | tr "\n" ", ")"
if test ! -d ${tmp_protogen_dir} ; then
	mkdir ${tmp_protogen_dir}
fi
protoc --cpp_out=${tmp_protogen_dir} --grpc_out=${tmp_protogen_dir} --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) -Iproto AppToCppServer.proto relay_server.proto
mv ${tmp_protogen_dir}/AppToCppServer.grpc.pb.h include/
mv ${tmp_protogen_dir}/AppToCppServer.pb.h include/
mv ${tmp_protogen_dir}/relay_server.grpc.pb.h include/
mv ${tmp_protogen_dir}/relay_server.pb.h include/
mv ${tmp_protogen_dir}/AppToCppServer.grpc.pb.cc src/
mv ${tmp_protogen_dir}/AppToCppServer.pb.cc src/
mv ${tmp_protogen_dir}/relay_server.grpc.pb.cc src/
mv ${tmp_protogen_dir}/relay_server.pb.cc src/
rmdir ${tmp_protogen_dir}
],[])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()


AC_PROG_CXX

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

