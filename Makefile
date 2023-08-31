CFLAGS = -D_GNU_SOURCE -g

CFLAGS = -Wall -g

OBJS = src/main.o src/container_status.o src/create.o src/start.o src/utls.o

rrun: $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
		$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -f rrun $(OBJS)
