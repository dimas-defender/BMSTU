#include <stdio.h>
#include <math.h>

float tr_perim(float a, float b, float h);

int main(void)
{
    float a, b, h;
	
    printf("Input first trapezoid base: ");
    scanf("%f", &a);
    printf("Input second trapezoid base: ");
    scanf("%f", &b);
    printf("Input trapezoid height: ");
    scanf("%f", &h);
    printf("Trapezoid perimeter is %.5f", tr_perim(a, b, h));
	
    return 0;
}

float tr_perim(float a, float b, float h)
{
    float part = fabs(a - b) / 2;
    float side = sqrt(pow(part, 2) + pow(h, 2));
    float p = a + b + 2 * side;
	
    return p;
}