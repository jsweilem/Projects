CC=		    gcc
CFLAGS=		-g -Wall -Werror -std=gnu99 -Iinclude
LD=		    gcc
LDFLAGS=	-L.
AR=		    ar
ARFLAGS=	rcs
TARGETS=	bin/spidey

all:		$(TARGETS)

bin/spidey:		src/spidey.o lib/libspidey.a
	$(LD) $(LDFLAGS) -o $@ $^

lib/libspidey.a:	src/forking.o src/request.o src/single.o src/socket.o src/utils.o src/handler.o
	$(AR) $(ARFLAGS) $@ $^

src/spidey.o:	src/spidey.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/forking.o:	src/forking.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/request.o:	src/request.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/single.o:	src/single.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/socket.o:	src/socket.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/utils.o:	src/utils.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

src/handler.o:	src/handler.c include/spidey.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) lib/*.a src/*.o *.log *.input

.PHONY:		all test clean
