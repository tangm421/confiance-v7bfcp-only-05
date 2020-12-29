CC = gcc
CC_W32 = i586-mingw32-gcc
OPTS = -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O2 #-Werror -Wunused
LIBS = -lpthread
LIBS_W32 = -lws2_32 #-s
DEPS_W32 = -L./
WIN32 = -DWIN32

so : bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o
	$(CC) -ggdb -shared -o bfcp_messages.so bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o $(OPTS) $(LIBS)

%.o: %.c
	$(CC) -ggdb -c $< -o $@ $(OPTS)

dll : bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj
	$(CC_W32) $(WIN32) -ggdb -shared -o bfcp_messages.dll bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj $(OPTS) $(DEPS_W32) $(LIBS_W32)

%.obj: %.c
	$(CC_W32) $(WIN32) -ggdb -c $< -o $@ $(OPTS)

linux : testcode.o bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o
	$(CC) -ggdb -o testcode testcode.o bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o $(OPTS) $(LIBS)

win32 : testcode.obj bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj
	$(CC_W32) $(WIN32) -ggdb -o testcode.exe testcode.obj bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj $(OPTS) $(DEPS_W32) $(LIBS_W32)

clean :
	rm *.o *.obj bfcp_messages.so bfcp_messages.dll *~ mesg.hex testcode testcode.exe

all:
	make so && make dll && make linux && make win32
