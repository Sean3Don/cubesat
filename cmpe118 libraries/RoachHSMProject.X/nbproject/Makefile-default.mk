#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/CMPE118/src/AD.c C:/CMPE118/src/BOARD.c C:/CMPE118/src/ES_CheckEvents.c C:/CMPE118/src/ES_Framework.c C:/CMPE118/src/ES_LookupTables.c C:/CMPE118/src/ES_PostList.c C:/CMPE118/src/ES_Queue.c C:/CMPE118/src/ES_Timers.c C:/CMPE118/src/ES_TimerService.c C:/CMPE118/src/pwm.c C:/CMPE118/src/roach.c C:/CMPE118/src/serial.c RoachFrameworkEvents.c RoachMain.c C:/CMPE118/src/ES_KeyboardInput.c C:/CMPE118/src/ES_TattleTale.c RoachHSM.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/331920610/AD.o ${OBJECTDIR}/_ext/331920610/BOARD.o ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o ${OBJECTDIR}/_ext/331920610/ES_Framework.o ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o ${OBJECTDIR}/_ext/331920610/ES_PostList.o ${OBJECTDIR}/_ext/331920610/ES_Queue.o ${OBJECTDIR}/_ext/331920610/ES_Timers.o ${OBJECTDIR}/_ext/331920610/ES_TimerService.o ${OBJECTDIR}/_ext/331920610/pwm.o ${OBJECTDIR}/_ext/331920610/roach.o ${OBJECTDIR}/_ext/331920610/serial.o ${OBJECTDIR}/RoachFrameworkEvents.o ${OBJECTDIR}/RoachMain.o ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o ${OBJECTDIR}/RoachHSM.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/331920610/AD.o.d ${OBJECTDIR}/_ext/331920610/BOARD.o.d ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d ${OBJECTDIR}/_ext/331920610/ES_Framework.o.d ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d ${OBJECTDIR}/_ext/331920610/ES_PostList.o.d ${OBJECTDIR}/_ext/331920610/ES_Queue.o.d ${OBJECTDIR}/_ext/331920610/ES_Timers.o.d ${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d ${OBJECTDIR}/_ext/331920610/pwm.o.d ${OBJECTDIR}/_ext/331920610/roach.o.d ${OBJECTDIR}/_ext/331920610/serial.o.d ${OBJECTDIR}/RoachFrameworkEvents.o.d ${OBJECTDIR}/RoachMain.o.d ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d ${OBJECTDIR}/RoachHSM.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/331920610/AD.o ${OBJECTDIR}/_ext/331920610/BOARD.o ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o ${OBJECTDIR}/_ext/331920610/ES_Framework.o ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o ${OBJECTDIR}/_ext/331920610/ES_PostList.o ${OBJECTDIR}/_ext/331920610/ES_Queue.o ${OBJECTDIR}/_ext/331920610/ES_Timers.o ${OBJECTDIR}/_ext/331920610/ES_TimerService.o ${OBJECTDIR}/_ext/331920610/pwm.o ${OBJECTDIR}/_ext/331920610/roach.o ${OBJECTDIR}/_ext/331920610/serial.o ${OBJECTDIR}/RoachFrameworkEvents.o ${OBJECTDIR}/RoachMain.o ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o ${OBJECTDIR}/RoachHSM.o

# Source Files
SOURCEFILES=C:/CMPE118/src/AD.c C:/CMPE118/src/BOARD.c C:/CMPE118/src/ES_CheckEvents.c C:/CMPE118/src/ES_Framework.c C:/CMPE118/src/ES_LookupTables.c C:/CMPE118/src/ES_PostList.c C:/CMPE118/src/ES_Queue.c C:/CMPE118/src/ES_Timers.c C:/CMPE118/src/ES_TimerService.c C:/CMPE118/src/pwm.c C:/CMPE118/src/roach.c C:/CMPE118/src/serial.c RoachFrameworkEvents.c RoachMain.c C:/CMPE118/src/ES_KeyboardInput.c C:/CMPE118/src/ES_TattleTale.c RoachHSM.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/331920610/AD.o: C:/CMPE118/src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/AD.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/AD.o.d" -o ${OBJECTDIR}/_ext/331920610/AD.o C:/CMPE118/src/AD.c   
	
${OBJECTDIR}/_ext/331920610/BOARD.o: C:/CMPE118/src/BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/BOARD.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/BOARD.o.d" -o ${OBJECTDIR}/_ext/331920610/BOARD.o C:/CMPE118/src/BOARD.c   
	
${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o: C:/CMPE118/src/ES_CheckEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o C:/CMPE118/src/ES_CheckEvents.c   
	
${OBJECTDIR}/_ext/331920610/ES_Framework.o: C:/CMPE118/src/ES_Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_Framework.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_Framework.o C:/CMPE118/src/ES_Framework.c   
	
${OBJECTDIR}/_ext/331920610/ES_LookupTables.o: C:/CMPE118/src/ES_LookupTables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o C:/CMPE118/src/ES_LookupTables.c   
	
${OBJECTDIR}/_ext/331920610/ES_PostList.o: C:/CMPE118/src/ES_PostList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_PostList.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_PostList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_PostList.o C:/CMPE118/src/ES_PostList.c   
	
${OBJECTDIR}/_ext/331920610/ES_Queue.o: C:/CMPE118/src/ES_Queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_Queue.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_Queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_Queue.o C:/CMPE118/src/ES_Queue.c   
	
${OBJECTDIR}/_ext/331920610/ES_Timers.o: C:/CMPE118/src/ES_Timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_Timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_Timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_Timers.o C:/CMPE118/src/ES_Timers.c   
	
${OBJECTDIR}/_ext/331920610/ES_TimerService.o: C:/CMPE118/src/ES_TimerService.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_TimerService.o C:/CMPE118/src/ES_TimerService.c   
	
${OBJECTDIR}/_ext/331920610/pwm.o: C:/CMPE118/src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/pwm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/pwm.o.d" -o ${OBJECTDIR}/_ext/331920610/pwm.o C:/CMPE118/src/pwm.c   
	
${OBJECTDIR}/_ext/331920610/roach.o: C:/CMPE118/src/roach.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/roach.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/roach.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/roach.o.d" -o ${OBJECTDIR}/_ext/331920610/roach.o C:/CMPE118/src/roach.c   
	
${OBJECTDIR}/_ext/331920610/serial.o: C:/CMPE118/src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/serial.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/serial.o.d" -o ${OBJECTDIR}/_ext/331920610/serial.o C:/CMPE118/src/serial.c   
	
${OBJECTDIR}/RoachFrameworkEvents.o: RoachFrameworkEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RoachFrameworkEvents.o.d 
	@${FIXDEPS} "${OBJECTDIR}/RoachFrameworkEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/RoachFrameworkEvents.o.d" -o ${OBJECTDIR}/RoachFrameworkEvents.o RoachFrameworkEvents.c   
	
${OBJECTDIR}/RoachMain.o: RoachMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RoachMain.o.d 
	@${FIXDEPS} "${OBJECTDIR}/RoachMain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/RoachMain.o.d" -o ${OBJECTDIR}/RoachMain.o RoachMain.c   
	
${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o: C:/CMPE118/src/ES_KeyboardInput.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o C:/CMPE118/src/ES_KeyboardInput.c   
	
${OBJECTDIR}/_ext/331920610/ES_TattleTale.o: C:/CMPE118/src/ES_TattleTale.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o C:/CMPE118/src/ES_TattleTale.c   
	
${OBJECTDIR}/RoachHSM.o: RoachHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RoachHSM.o.d 
	@${FIXDEPS} "${OBJECTDIR}/RoachHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/RoachHSM.o.d" -o ${OBJECTDIR}/RoachHSM.o RoachHSM.c   
	
else
${OBJECTDIR}/_ext/331920610/AD.o: C:/CMPE118/src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/AD.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/AD.o.d" -o ${OBJECTDIR}/_ext/331920610/AD.o C:/CMPE118/src/AD.c   
	
${OBJECTDIR}/_ext/331920610/BOARD.o: C:/CMPE118/src/BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/BOARD.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/BOARD.o.d" -o ${OBJECTDIR}/_ext/331920610/BOARD.o C:/CMPE118/src/BOARD.c   
	
${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o: C:/CMPE118/src/ES_CheckEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_CheckEvents.o C:/CMPE118/src/ES_CheckEvents.c   
	
${OBJECTDIR}/_ext/331920610/ES_Framework.o: C:/CMPE118/src/ES_Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_Framework.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_Framework.o C:/CMPE118/src/ES_Framework.c   
	
${OBJECTDIR}/_ext/331920610/ES_LookupTables.o: C:/CMPE118/src/ES_LookupTables.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_LookupTables.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_LookupTables.o C:/CMPE118/src/ES_LookupTables.c   
	
${OBJECTDIR}/_ext/331920610/ES_PostList.o: C:/CMPE118/src/ES_PostList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_PostList.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_PostList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_PostList.o C:/CMPE118/src/ES_PostList.c   
	
${OBJECTDIR}/_ext/331920610/ES_Queue.o: C:/CMPE118/src/ES_Queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_Queue.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_Queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_Queue.o C:/CMPE118/src/ES_Queue.c   
	
${OBJECTDIR}/_ext/331920610/ES_Timers.o: C:/CMPE118/src/ES_Timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_Timers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_Timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_Timers.o C:/CMPE118/src/ES_Timers.c   
	
${OBJECTDIR}/_ext/331920610/ES_TimerService.o: C:/CMPE118/src/ES_TimerService.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_TimerService.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_TimerService.o C:/CMPE118/src/ES_TimerService.c   
	
${OBJECTDIR}/_ext/331920610/pwm.o: C:/CMPE118/src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/pwm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/pwm.o.d" -o ${OBJECTDIR}/_ext/331920610/pwm.o C:/CMPE118/src/pwm.c   
	
${OBJECTDIR}/_ext/331920610/roach.o: C:/CMPE118/src/roach.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/roach.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/roach.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/roach.o.d" -o ${OBJECTDIR}/_ext/331920610/roach.o C:/CMPE118/src/roach.c   
	
${OBJECTDIR}/_ext/331920610/serial.o: C:/CMPE118/src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/serial.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/serial.o.d" -o ${OBJECTDIR}/_ext/331920610/serial.o C:/CMPE118/src/serial.c   
	
${OBJECTDIR}/RoachFrameworkEvents.o: RoachFrameworkEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RoachFrameworkEvents.o.d 
	@${FIXDEPS} "${OBJECTDIR}/RoachFrameworkEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/RoachFrameworkEvents.o.d" -o ${OBJECTDIR}/RoachFrameworkEvents.o RoachFrameworkEvents.c   
	
${OBJECTDIR}/RoachMain.o: RoachMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RoachMain.o.d 
	@${FIXDEPS} "${OBJECTDIR}/RoachMain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/RoachMain.o.d" -o ${OBJECTDIR}/RoachMain.o RoachMain.c   
	
${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o: C:/CMPE118/src/ES_KeyboardInput.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_KeyboardInput.o C:/CMPE118/src/ES_KeyboardInput.c   
	
${OBJECTDIR}/_ext/331920610/ES_TattleTale.o: C:/CMPE118/src/ES_TattleTale.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/331920610 
	@${RM} ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/331920610/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/331920610/ES_TattleTale.o C:/CMPE118/src/ES_TattleTale.c   
	
${OBJECTDIR}/RoachHSM.o: RoachHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RoachHSM.o.d 
	@${FIXDEPS} "${OBJECTDIR}/RoachHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/RoachHSM.o.d" -o ${OBJECTDIR}/RoachHSM.o RoachHSM.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)    -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/RoachHSMProject.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
