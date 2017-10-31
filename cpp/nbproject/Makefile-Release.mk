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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o \
	${OBJECTDIR}/autogen_files/proto/relay_server.pb.o \
	${OBJECTDIR}/src/Activity.o \
	${OBJECTDIR}/src/Appliances.o \
	${OBJECTDIR}/src/Client.o \
	${OBJECTDIR}/src/ClientData.o \
	${OBJECTDIR}/src/Data.o \
	${OBJECTDIR}/src/ExtFunctions.o \
	${OBJECTDIR}/src/RelayServerClient.o \
	${OBJECTDIR}/src/Room.o \
	${OBJECTDIR}/src/Server.o \
	${OBJECTDIR}/src/latest_Server.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/home_automation

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/home_automation: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/home_automation ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o: autogen_files/proto/relay_server.grpc.pb.cc
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/relay_server.grpc.pb.o autogen_files/proto/relay_server.grpc.pb.cc

${OBJECTDIR}/autogen_files/proto/relay_server.pb.o: autogen_files/proto/relay_server.pb.cc
	${MKDIR} -p ${OBJECTDIR}/autogen_files/proto
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/autogen_files/proto/relay_server.pb.o autogen_files/proto/relay_server.pb.cc

${OBJECTDIR}/src/Activity.o: src/Activity.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Activity.o src/Activity.cpp

${OBJECTDIR}/src/Appliances.o: src/Appliances.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Appliances.o src/Appliances.cpp

${OBJECTDIR}/src/Client.o: src/Client.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Client.o src/Client.cpp

${OBJECTDIR}/src/ClientData.o: src/ClientData.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ClientData.o src/ClientData.cpp

${OBJECTDIR}/src/Data.o: src/Data.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Data.o src/Data.cpp

${OBJECTDIR}/src/ExtFunctions.o: src/ExtFunctions.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ExtFunctions.o src/ExtFunctions.cpp

${OBJECTDIR}/src/RelayServerClient.o: src/RelayServerClient.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/RelayServerClient.o src/RelayServerClient.cpp

${OBJECTDIR}/src/Room.o: src/Room.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Room.o src/Room.cpp

${OBJECTDIR}/src/Server.o: src/Server.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Server.o src/Server.cpp

${OBJECTDIR}/src/latest_Server.o: src/latest_Server.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/latest_Server.o src/latest_Server.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
