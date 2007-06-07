# Makefile for SWISH
# Kevin Hughes, 3/11/95

all:
	cd src; make
	cd src; mv swish ..

clean:
	rm -f ./swish ./index.swish
	cd src; make clean

test:
	./swish -i test.html -v -f index.swish
	./swish -f index.swish -w test
