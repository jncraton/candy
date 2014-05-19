candy.exe: candy.c
	gcc -std=c99 -Wall -o candy.exe candy.c
	
candy.c: candy.cn
	candy < candy.cn > candy.c
	
clean:
	rm candy.exe
	rm candy.c
	rm test_out.c
	
test:
	@echo Testing...
	@candy.exe < test.cn > test_out.c
	@cmp test_out.c test.c