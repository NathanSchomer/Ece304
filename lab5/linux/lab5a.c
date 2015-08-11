//Combination Lock
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//add 5 second timeout

char* comb = "24135";

int getCombination()
{
	char key1 = comb[0];
	int currentState = -1;
	char input;
	while(1){
		//get button pushed by user via UART
		input = getchar();
		while(input == '\n')
			input = getchar();
			
		if(input == comb[0]){
			if(currentState == -1)
				currentState = 1;
			else
				return 0;
		}
		else if(input == comb[1]){
			if(currentState == 1)
				currentState = 2;
			else
				return 0;
		}
		else if(input == comb[2]){
			if(currentState == 2)
				currentState = 3;
			else
				return 0;
		}
		else if(input == comb[3]){
			if(currentState == 3)
				currentState = 4;
			else
				return 0;
		}
		else if(input == comb[4]){
			if(currentState == 4)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	
}

main()
{
	printf("Combination Lock\n");
	int attempts = 0;
	int permLock = 0;
	int unlocked = 0;
	for(;;){	//user gets 3 attempts to try
		if(unlocked == 1)
		{
			if(getchar() == 'c'){
				printf("New 5-digit combination: ");
				comb = "12345"; //modify value of comb here
				//getchar();
				//getline(comb, 6, stdin);
				printf("SYSTEM LOCKED!\n");
				unlocked = 0;
			}
		}
		else if(attempts < 3 && getCombination()){
			attempts = 0;
			unlocked = 1;
			printf("ACCESS GRANTED\n");
		}
		else if(attempts < 3){
			attempts++;
			printf("ACCESS DENIED\n");
		}
		else if(permLock == 0){
			permLock = 1;
			printf("\nTOO MANY ATTEMPTS.\n");
			printf("SYSTEM IS PERMANATELY LOCKED.\n");
		}
		else{
		}
	}
	return 0;
}
