CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
LDFLAGS = -pthread

all: seq_pi par_pi_pthreads

seq_pi: src/seq_pi.c
	$(CC) $(CFLAGS) -o seq_pi src/seq_pi.c

par_pi_pthreads: src/par_pi_pthreads.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o par_pi_pthreads src/par_pi_pthreads.c

clean:
	rm -f seq_pi par_pi_pthreads
