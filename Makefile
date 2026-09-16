all:
	$(CC) $(CFLAGS) -o base-console base-console.c
clean:
	$(RM) -f base-console
