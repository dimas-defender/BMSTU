#include <stdio.h>

int bottles_amount(int s);

int main(void)
{
    int s;
	
    printf("Input amount of money: ");
    scanf("%d", &s);
    printf("You can buy %d bottle(s) of water", bottles_amount(s));
	
    return 0;
}

int bottles_amount(int s)
{
    int amount = (s - 20) / 25;
	
    return amount;
}