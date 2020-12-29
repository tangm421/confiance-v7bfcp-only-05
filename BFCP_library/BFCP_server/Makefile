CC = gcc
CC_W32 = i586-mingw32-gcc
OPTS = -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O2 -Werror -Wunused
LIBS = -lpthread -lssl -lcrypto
LIBS_W32 = -lws2_32 -lpthreadGC2 -lssleay32 -llibeay32 #-s
DEPS_W32 = -L./
WIN32 = -DWIN32

so : bfcp_server.o bfcp_link_list.o bfcp_floor_list.o bfcp_user_list.o bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o
	$(CC) -ggdb -shared -o bfcp_server.so bfcp_server.o bfcp_link_list.o bfcp_floor_list.o bfcp_user_list.o bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o $(OPTS) $(LIBS)

%.o: %.c
	$(CC) -ggdb -c $< -o $@ $(OPTS)

dll : bfcp_server.obj bfcp_link_list.obj bfcp_floor_list.obj bfcp_user_list.obj bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj
	$(CC_W32) $(WIN32) -ggdb -shared -o bfcp_server.dll bfcp_server.obj bfcp_link_list.obj bfcp_floor_list.obj bfcp_user_list.obj bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj $(OPTS) $(DEPS_W32) $(LIBS_W32)

%.obj: %.c
	$(CC_W32) $(WIN32) -ggdb -c $< -o $@ $(OPTS)

linux : main.o bfcp_server.o bfcp_link_list.o bfcp_floor_list.o bfcp_user_list.o bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o
	$(CC) -ggdb -o bfcp_server main.o bfcp_server.o bfcp_link_list.o bfcp_floor_list.o bfcp_user_list.o bfcp_messages.o bfcp_messages_build.o bfcp_messages_parse.o $(OPTS) $(LIBS)

win32 : main.obj bfcp_server.obj bfcp_link_list.obj bfcp_floor_list.obj bfcp_user_list.obj bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj
	$(CC_W32) $(WIN32) -ggdb -o bfcp_server.exe main.obj bfcp_server.obj bfcp_link_list.obj bfcp_floor_list.obj bfcp_user_list.obj bfcp_messages.obj bfcp_messages_build.obj bfcp_messages_parse.obj $(OPTS) $(DEPS_W32) $(LIBS_W32)

clean :
	rm *.o *.obj *~ bfcp_server.so bfcp_server.dll bfcp_server bfcp_server.exe

all:
	make so && make dll && make linux && make win32
