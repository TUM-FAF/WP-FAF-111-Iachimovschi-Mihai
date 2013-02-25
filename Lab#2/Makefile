HEADERS = include/resource.h
OBJS =	obj/winmain.o obj/resource.o
INCLUDE_DIRS = -I./include

WARNS = -Wall

CC = /usr/local/i386-mingw32-3.4.5/bin/i386-mingw32-g++
CFLAGS = -O3 ${WARNS}
LDFLAGS = -s -lcomctl32 -Wl,--subsystem,windows -mwindows
RC = /usr/local/i386-mingw32-3.4.5/bin/i386-mingw32-windres

all : lab.exe

lab.exe : ${OBJS}
	${CC} -o "$@" ${OBJS} ${LDFLAGS}

clean :
	rm obj/*.o "lab.exe"

obj/%.o : src/%.cpp ${HEADERS}
	${CC} ${CFLAGS} ${INCLUDE_DIRS} -c $< -o $@

obj/resource.o : res/resource.rc res/Application.manifest res/Application.ico include/resource.h
	${RC} -I./include -I./res -i $< -o $@
