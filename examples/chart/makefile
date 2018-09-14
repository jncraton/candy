all: chart

chart.c: chart.cn
	../../candy chart.cn chart.c
	../../candy chart.cn chart.h

chart: chart.c
	gcc chart.c -o chart -lpng -Wall -std=c99
	
test: chart
	./chart t.png