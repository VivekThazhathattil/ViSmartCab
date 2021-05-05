#include "../include/passenger.h"
#include "../include/consts.h"
#include <stdio.h>

int main(){
	
	Passenger p;	
	printf("%d %d %d %d\n", p.getPos(0,0), p.getPos(1,0), p.getPos(0,1), p.getPos(1,1));
	return 0;
}
