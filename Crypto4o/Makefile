CC	= gcc
CFLAGS	= -Wall -O2 -g -I.
#CFLAGS	+= -D__DEBUG__
LDFLAGS	=

PROG	= prng
EPROG	= crypto4o
OBJS	= prng.o glfsr.o
EOBJS	= crypt.o glfsr.o crypto4o.o

all:	$(OBJS) $(EOBJS)
	$(CC) -o $(PROG) $(OBJS)
	$(CC) -o $(EPROG) $(EOBJS)
	$(CC) -o libcrypto4o.so -shared glfsr.o crypto4o.o
	nm libcrypto4o.so | sort > libcrypto4o.nm

run:
	./$(PROG) 2>output.bin | tail -n 150

observe:
	watch -n 1 "ls -hali output.*; echo; ls -ali output.*; echo; cat /proc/cpuinfo | grep MHz; echo; top -bn 1 | head -n 15"

clean:
	rm -f $(PROG) $(EPROG) *.o *~ *core output.* lib*

install:
	install -m 755 -g root -o root -s $(PROG) /usr/local/bin
	install -m 755 -g root -o root -s $(EPROG) /usr/local/bin
	install -m 755 -g root -o root $(EPROG).py /usr/local/bin
	install -m 644 -g root -o root libcrypto4o.so /usr/lib