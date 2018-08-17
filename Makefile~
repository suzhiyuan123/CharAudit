test:main.o test.o test2.o test3.o error.o writen.o signalintr.o ttymodes.o

	cc -o test main.o test.o test2.o test3.o error.o writen.o signalintr.o ttymodes.o

main.o:main.c
	cc -c main.c
test.o:test.c
	cc -c test.c
test2.o:test2.c
	cc -c test2.c
test3.o:test3.c
	cc -c test3.c
error.o:error.c
	cc -c error.c
writen.o:writen.c
	cc -c writen.c
signalintr.o:signalintr.c
	cc -c signalintr.c
ttymodes.o:ttymodes.c
	cc -c ttymodes.c

install:
	cp test /home/su/work/
#	cp test /usr/bin/
clean:
	rm -rf *.o 
	rm test
#	rm /home/su/work/test

