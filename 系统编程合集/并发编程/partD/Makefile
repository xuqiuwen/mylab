all:
	# empty

stack:
	gcc -g concur-stack.c test-stack.c -o stack.out
	./stack.out

hash:
	gcc -g concur-hash.c test-hash.c -o hash.out
	./hash.out

hash-little:
	gcc -g concur-hash-little.c test-hash-little.c -o hash-little.out
	./hash-little.out

clean:
	rm -rf *.out
