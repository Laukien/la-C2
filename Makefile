NAME := la
VERSION := `cat VERSION`
GCCVER := $(shell expr `gcc -dumpversion | cut -d '.' -f 2` \>= 8)

BINDIR := bin
OBJDIR := obj
LIBDIR := lib
PREFIX := /usr/local

CFLAGS += -I inc
CFLAGS += -O3
CFLAGS += -W
CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -pedantic
CFLAGS += -march=nocona -mtune=generic

#WINDOWS
ifdef WIN32
CC := x86_64-w64-mingw32-gcc
CFLAGS +=
LDFLAGS +=
AR := x86_64-w64-mingw32-ar
ARFLAGS := -rcs
ARNAME := $(NAME).$(VERSION).lib
RANLIB := x86_64-w64-mingw32-ranlib
EXT := .exe
endif

#LINUX
ifndef WIN32
CC := gcc
CFLAGS += -ggdb3
#LDFLAGS += -lpthread
AR := ar
ARFLAGS := -rcs
ARNAME := lib$(NAME).$(VERSION).a
SONAME := lib$(NAME).$(VERSION).so
RANLIB := ranlib
EXT :=
endif


all: init clean core example

init:
	@echo
	@echo ===  INIT ===
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

core: core-static core-dynamic

core-static:
	@echo
	@echo === CORE STATIC ===
	rm -f $(OBJDIR)/*
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/boolean.o src/core/boolean.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/character.o src/core/character.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/config.o src/core/config.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/console.o src/core/console.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/datetime.o src/core/datetime.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/directory.o src/core/directory.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/directorylist.o src/core/directorylist.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/file.o src/core/file.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/filecopy.o src/core/filecopy.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/filelist.o src/core/filelist.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/memory.o src/core/memory.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/message.o src/core/message.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/number.o src/core/number.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/string.o src/core/string.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/stringarray.o src/core/stringarray.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/stringbuffer.o src/core/stringbuffer.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/stringlist.o src/core/stringlist.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/stringmap.o src/core/stringmap.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/stringsplit.o src/core/stringsplit.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/system.o src/core/system.c
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME) $(OBJDIR)/*.o
	$(RANLIB) $(LIBDIR)/$(ARNAME)
ifdef WIN32
	ln -fs $(ARNAME) $(LIBDIR)/$(NAME).lib
else
	ln -fs $(ARNAME) $(LIBDIR)/lib$(NAME).a
endif

core-dynamic:
ifndef WIN32
	@echo
	@echo === CORE DYNAMIC ===
	$(CC) $(CFLAGS) -shared -fPIC -Wl,-soname,$(SONAME) -o $(LIBDIR)/$(SONAME)\
		src/core/boolean.c\
		src/core/character.c\
		src/core/config.c\
		src/core/console.c\
		src/core/datetime.c\
		src/core/directory.c\
		src/core/directorylist.c\
		src/core/file.c\
		src/core/filecopy.c\
		src/core/filelist.c\
		src/core/memory.c\
		src/core/message.c\
		src/core/number.c\
		src/core/string.c\
		src/core/stringarray.c\
		src/core/stringbuffer.c\
		src/core/stringlist.c\
		src/core/stringmap.c\
		src/core/stringsplit.c\
		src/core/system.c
endif

example:
	@echo
	@echo === EXAMPLE ===
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/array_1$(EXT) src/example/array_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/array_2$(EXT) src/example/array_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/config_1$(EXT) src/example/config_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/console_1$(EXT) src/example/console_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/console_2$(EXT) src/example/console_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/console_3$(EXT) src/example/console_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/atomic_1$(EXT) src/example/atomic_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/directory_1$(EXT) src/example/directory_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/directory_2$(EXT) src/example/directory_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_1$(EXT) src/example/file_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_2$(EXT) src/example/file_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_3$(EXT) src/example/file_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_4$(EXT) src/example/file_4.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_5$(EXT) src/example/file_5.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_1$(EXT) src/example/list_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_2$(EXT) src/example/list_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_3$(EXT) src/example/list_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_4$(EXT) src/example/list_4.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/network_1$(EXT) src/example/network_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/network_2$(EXT) src/example/network_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/memory_1$(EXT) src/example/memory_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/number_1$(EXT) src/example/number_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_1$(EXT) src/example/parameter_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_2$(EXT) src/example/parameter_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_3$(EXT) src/example/parameter_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/process_1$(EXT) src/example/process_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_1$(EXT) src/example/string_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_2$(EXT) src/example/string_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_3$(EXT) src/example/string_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_4$(EXT) src/example/string_4.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_5$(EXT) src/example/string_5.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_6$(EXT) src/example/string_6.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_simple$(EXT) src/example/string_simple.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/stringbuffer_1$(EXT) src/example/stringbuffer_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_1$(EXT) src/example/system_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_2$(EXT) src/example/system_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_3$(EXT) src/example/system_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	#$(CC) $(CFLAGS) -I src -o $(BINDIR)/thread_1$(EXT) src/example/thread_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)

clean:
	@echo
	@echo === CLEAN ===
	rm -f $(BINDIR)/*
	#rm -f $(LIBDIR)/*
	rm -f $(OBJDIR)/*

install:
	@echo
	@echo === INSTALL ===
	install -o root -g root $(LIBDIR)/$(ARNAME) $(DESTDIR)$(PREFIX)/lib
	ln -f -s $(DESTDIR)$(PREFIX)/lib/$(ARNAME) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a
	install -o root -g root $(LIBDIR)/$(SONAME) $(DESTDIR)$(PREFIX)/lib
	ln -f -s $(DESTDIR)$(PREFIX)/lib/$(SONAME) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).so
	mkdir -p $(DESTDIR)$(PREFIX)/include/$(NAME)
	install -o root -g root inc/$(NAME)/*.h $(DESTDIR)$(PREFIX)/include/$(NAME)
	ldconfig $(DESTDIR)$(PREFIX)/lib

uninstall:
	@echo
	@echo === UNINSTALL ===
	rm -f $(DESTDIR)$(PREFIX)/lib/lib$(NAME).*
	rm -rf $(DESTDIR)$(PREFIX)/include/$(NAME)
	ldconfig $(DESTDIR)$(PREFIX)/lib

