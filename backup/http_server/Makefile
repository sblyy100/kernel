server:main.c server.o log.o online.o response.o
	gcc -g -o server main.c server.o log.o online.o response.o -lpthread
log.o:log.c log.h
	gcc -c log.c log.h -lpthread
server.o:server.c server.h myhead.h log.h
	gcc -c -D DEBUG server.c server.h myhead.h log.h -lpthread
online.o:online.c online.h myhead.h
	gcc -c online.c online.h myhead.h
response.o:response.c response.h myhead.h
	gcc -c response.c response.h myhead.h

clean:
	rm *.o
