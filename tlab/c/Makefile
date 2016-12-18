
CFLAGS=-Wall -g

all: tlab_enabled_sim tlab_disabled_sim

%: %.o
	gcc $(CFLAGS) -o $@ $^

tlab_enabled_sim.o: tlab_sim.c
	gcc $(CFLAGS) -D USE_TLAB=1 -c -o $@ $^

tlab_disabled_sim.o: tlab_sim.c
	gcc $(CFLAGS) -D USE_TLAB=0 -c -o $@ $^
