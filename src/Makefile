sim: common.c cache.c sim.c ll.c pt.c tlb.c
	gcc -o sim sim.c cache.c common.c ll.c pt.c tlb.c -I./include -Wall -Werror -g -DDEBUG -lm
clean: 
	rm sim
