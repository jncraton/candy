candy.exe: candy.c
	gcc -std=c99 -s -Os -Wall -o candy.exe candy.c
	
candy.c: candy.cn
	candy_stable candy.cn candy.c
	
clean:
	rm candy.exe
	rm candy.c
	rm test_out.c
	
test: candy.exe
	@echo Testing...
	@candy.exe test.cn test_out.c
	@candy.exe test.cn test_out.h
	@cmp test_out.c test.c