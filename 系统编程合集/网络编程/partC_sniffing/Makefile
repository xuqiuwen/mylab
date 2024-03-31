all:
	gcc promiscuous_main.c packet-analyze.c -o main.out
	sudo ./main.out lo

sniff:
	gcc -o analysis.out filter.c main.c packet-process.c
	sudo ./analysis.out lo

clean:
	rm -rf *.out

