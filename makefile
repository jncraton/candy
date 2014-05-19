sugarc.exe: sugarc.c
	gcc -std=c99 -Wall -o sugarc.exe sugarc.c
	
sugarc.c: sugarc.sc
	sugarc < sugarc.sc > sugarc.c
	
clean:
	rm sugarc.exe
	rm sugarc.c