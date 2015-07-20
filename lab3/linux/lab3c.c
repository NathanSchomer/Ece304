//Luhn Checksum Validation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int validateLuhn(int num)
{
	int sum = 0, i;
	for(i = 1; num > 0; i += 2){
		sum += num%power(10,i);
		sum += 2*(num%power(10,i+1))%10 + 2*(num%power(10, i+1))/10;
		if(num < 100) break;
		else num /= 100;
	}
	if(sum % 10 == 0) return 1; //VALID
	else return 0; //INVALID
}

int power(int base, int exp)
{
	int i, result = base;
	for(i = 0; i < exp - 1; i++)
		result *= base;
	return result;
}

main()
{
	int num;
	int base = 10;
	scanf("%d", &num);	//get integer from user
	//printf("power(%d, %d) = %d",base,num,power(base, num));
	printf("\nValid?: %d\n", validateLuhn(num));
	return 0;
}
