#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb.o \
	${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb.o \
	${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o \
	${OBJECTDIR}/autogen_files/proto/relay_server.pb.o \
	${OBJECTDIR}/src/AppToCpp_Server.o \
	${OBJECTDIR}/src/Appliances.o \
	${OBJECTDIR}/src/Configuration.o \
	${OBJECTDIR}/src/CppServerMain.o \
	${OBJECTDIR}/src/Data.o \
	${OBJECTDIR}/src/ExtFunctions.o \
	${OBJECTDIR}/src/RelayServerClient.o \
	${OBJECTDIR}/src/Room.o \
	${OBJECTDIR}/src/Server.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/test_client

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/TestFiles/client1.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib64

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/home_server

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/home_server: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/home_server ${OBJECTFILES} ${LDLIBSOPTIONS} -lgrpc++ -lprotobuf -lpthread -lz -lgrpc++_unsecure -lgrpc_unsecure -lgpr -lgrpc_csharp_ext -ljsoncpp

${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb.o: autogen_files/proto/AppToCppServer.grpc.pb.cc
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb.o autogen_files/proto/AppToCppServer.grpc.pb.cc

${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb.o: autogen_files/proto/AppToCppServer.pb.cc
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb.o autogen_files/proto/AppToCppServer.pb.cc

${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o: autogen_files/proto/relay_server.grpc.pb.cc
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o autogen_files/proto/relay_server.grpc.pb.cc

${OBJECTDIR}/autogen_files/proto/relay_server.pb.o: autogen_files/proto/relay_server.pb.cc
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/relay_server.pb.o autogen_files/proto/relay_server.pb.cc

${OBJECTDIR}/src/AppToCpp_Server.o: src/AppToCpp_Server.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AppToCpp_Server.o src/AppToCpp_Server.cpp

${OBJECTDIR}/src/Appliances.o: src/Appliances.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Appliances.o src/Appliances.cpp

${OBJECTDIR}/src/Configuration.o: src/Configuration.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Configuration.o src/Configuration.cpp

${OBJECTDIR}/src/CppServerMain.o: src/CppServerMain.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CppServerMain.o src/CppServerMain.cpp

${OBJECTDIR}/src/Data.o: src/Data.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data.o src/Data.cpp

${OBJECTDIR}/src/ExtFunctions.o: src/ExtFunctions.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ExtFunctions.o src/ExtFunctions.cpp

${OBJECTDIR}/src/RelayServerClient.o: src/RelayServerClient.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RelayServerClient.o src/RelayServerClient.cpp

${OBJECTDIR}/src/Room.o: src/Room.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Room.o src/Room.cpp

${OBJECTDIR}/src/Server.o: src/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Server.o src/Server.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/test_client: ${TESTDIR}/TestFiles/client1.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/test_client $^ ${LDLIBSOPTIONS}  -lgrpc++ -lprotobuf -lpthread -lz -lgrpc++_unsecure -lgrpc_unsecure -lgpr -lgrpc_csharp_ext -ljsoncpp -L/usr/local/lib64 


${TESTDIR}/TestFiles/client1.o: TestFiles/client1.cpp 
	${MKDIR} -p ${TESTDIR}/TestFiles
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iautogen_files -Iinclude -I. -Iinclude -Iautogen_files -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/TestFiles/client1.o TestFiles/client1.cpp


${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb_nomain.o: ${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb.o autogen_files/proto/AppToCppServer.grpc.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	@NMOUTPUT=`${NM} ${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb_nomain.o autogen_files/proto/AppToCppServer.grpc.pb.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb.o ${OBJECTDIR}/autogen_files/proto/AppToCppServer.grpc.pb_nomain.o;\
	fi

${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb_nomain.o: ${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb.o autogen_files/proto/AppToCppServer.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	@NMOUTPUT=`${NM} ${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb_nomain.o autogen_files/proto/AppToCppServer.pb.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb.o ${OBJECTDIR}/autogen_files/proto/AppToCppServer.pb_nomain.o;\
	fi

${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb_nomain.o: ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o autogen_files/proto/relay_server.grpc.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	@NMOUTPUT=`${NM} ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb_nomain.o autogen_files/proto/relay_server.grpc.pb.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb_nomain.o;\
	fi

${OBJECTDIR}/autogen_files/proto/relay_server.pb_nomain.o: ${OBJECTDIR}/autogen_files/proto/relay_server.pb.o autogen_files/proto/relay_server.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	@NMOUTPUT=`${NM} ${OBJECTDIR}/autogen_files/proto/relay_server.pb.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/relay_server.pb_nomain.o autogen_files/proto/relay_server.pb.cc;\
	else  \
	    ${CP} ${OBJECTDIR}/autogen_files/proto/relay_server.pb.o ${OBJECTDIR}/autogen_files/proto/relay_server.pb_nomain.o;\
	fi

${OBJECTDIR}/src/AppToCpp_Server_nomain.o: ${OBJECTDIR}/src/AppToCpp_Server.o src/AppToCpp_Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/AppToCpp_Server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/AppToCpp_Server_nomain.o src/AppToCpp_Server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/AppToCpp_Server.o ${OBJECTDIR}/src/AppToCpp_Server_nomain.o;\
	fi

${OBJECTDIR}/src/Appliances_nomain.o: ${OBJECTDIR}/src/Appliances.o src/Appliances.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Appliances.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Appliances_nomain.o src/Appliances.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Appliances.o ${OBJECTDIR}/src/Appliances_nomain.o;\
	fi

${OBJECTDIR}/src/Configuration_nomain.o: ${OBJECTDIR}/src/Configuration.o src/Configuration.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Configuration.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Configuration_nomain.o src/Configuration.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Configuration.o ${OBJECTDIR}/src/Configuration_nomain.o;\
	fi

${OBJECTDIR}/src/CppServerMain_nomain.o: ${OBJECTDIR}/src/CppServerMain.o src/CppServerMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/CppServerMain.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CppServerMain_nomain.o src/CppServerMain.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/CppServerMain.o ${OBJECTDIR}/src/CppServerMain_nomain.o;\
	fi

${OBJECTDIR}/src/Data_nomain.o: ${OBJECTDIR}/src/Data.o src/Data.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Data.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data_nomain.o src/Data.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Data.o ${OBJECTDIR}/src/Data_nomain.o;\
	fi

${OBJECTDIR}/src/ExtFunctions_nomain.o: ${OBJECTDIR}/src/ExtFunctions.o src/ExtFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/ExtFunctions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ExtFunctions_nomain.o src/ExtFunctions.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/ExtFunctions.o ${OBJECTDIR}/src/ExtFunctions_nomain.o;\
	fi

${OBJECTDIR}/src/RelayServerClient_nomain.o: ${OBJECTDIR}/src/RelayServerClient.o src/RelayServerClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/RelayServerClient.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RelayServerClient_nomain.o src/RelayServerClient.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/RelayServerClient.o ${OBJECTDIR}/src/RelayServerClient_nomain.o;\
	fi

${OBJECTDIR}/src/Room_nomain.o: ${OBJECTDIR}/src/Room.o src/Room.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Room.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Room_nomain.o src/Room.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Room.o ${OBJECTDIR}/src/Room_nomain.o;\
	fi

${OBJECTDIR}/src/Server_nomain.o: ${OBJECTDIR}/src/Server.o src/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/Server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Iautogen_files -Iinclude -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Server_nomain.o src/Server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/Server.o ${OBJECTDIR}/src/Server_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/test_client || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
