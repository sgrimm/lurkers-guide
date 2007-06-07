# Makefile for SWISH
# Kevin Hughes, 3/12/95

CC= gcc

CFLAGS= -O2 -pipe

LIBS=	-lm

OBJS=	check.o file.o index.o search.o error.o \
	hash.o list.o mem.o string.o merge.o swish.o

.c.o:
	$(CC) -c $(CFLAGS) $<

all:	swish

swish:	$(OBJS)
	$(CC) -o swish $(CFLAGS) $(OBJS) $(LIBS)
	chmod 755 swish
	strip swish

quant:
#	make clean
	make CFLAGS="-g" $(OBJS)
	make quant-link

quant-link:
	quantify $(CC) -o swish -g $(CFLAGS) $(OBJS) $(LIBS)

pure:
#	make clean
	make CFLAGS="-g" $(OBJS)
	make pure-link

pure-link:
	purify $(CC) -o swish -g $(CFLAGS) $(OBJS) $(LIBS)

$(OBJS):	Makefile config.h swish.h

clean:
	rm -f ./swish *.o .pure swish.pure* *qx *qv ./index.swish
