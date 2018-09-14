candy: candy.c
	gcc -std=c99 -s -Os -Wall -o candy candy.c
	
candy.c: candy.cn
	./candy candy.cn candy.c
	./candy candy.cn candy.h
	
clean:
	rm candy
	rm test_out.c
	rm test_out.h
	
test: candy
	@echo Testing...
	@./candy test.cn test_out.c
	@./candy test.cn test_out.h
	@cmp test_out.c test.c
	@cmp test_out.h test.h
	@echo Tests passed.