test:main.o test.o test2.o test3.o

	cc -o test main.o test.o test2.o test3.o

main.o:main.c
	cc -c main.c
test.o:test.c
	cc -c test.c
test2.o:test2.c
	cc -c test2.c
test3.o:test3.c
	cc -c test3.c

install:
	cp test /home/su/work/
#	cp test /usr/bin/
clean:
	rm -rf *.o 
	rm test
#	rm /home/su/work/test

