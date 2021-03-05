build:
	gcc -Wall -g main.c stack.c queue.c  utils.c navigationAndHelpers.c alocFun.c freeFun.c deleteFun.c displayFun.c -o tema2
run: 
	./tema2
clean:
	rm tema2
