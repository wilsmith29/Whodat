CC = gcc

CFLAGS = -Wall -Wextra -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wunsafe-loop-optimizations -Wuninitialized -Werror

PROGS = whodat

all: $(PROGS)

$(PROGS): $(PROGS).o
	$(CC) $(CFLAGS) -o $(PROGS) $(PROGS).o
	chmod og-rx $(PROGS)

$(PROGS).o: $(PROGS).c
	$(CC) $(CFLAGS) -c $(PROGS).c

clean cls:
	rm -f $(PROGS) *.o *~ \#*

tar:
	tar cvfa Lab1_${LOGNAME}.tar.gz *.[ch] [mM]akefile *.bash
