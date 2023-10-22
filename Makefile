CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -pedantic-errors
BINS=sharedlib.so main.out

all: $(BINS)

%.so: %.c
	mkdir -p sharedlib
	$(CC) $(CFLAGS) -shared -fPIC -o sharedlib/$@ $^

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f sharedlib/sharedlib.so main.out
	rm -rf *.dSYM
