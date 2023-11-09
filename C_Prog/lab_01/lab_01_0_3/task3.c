#include <stdio.h>
#include <math.h>

float weight(float h, float t);

float mass_ind(float m, float h);

int main(void)
{
    float h, t, m;
	
    printf("Input height in cm: ");
    scanf("%f", &h);
    printf("Input chest circumference in cm: ");
    scanf("%f", &t);
    printf("Input weight in kg: ");
    scanf("%f", &m);
    printf("Normal weight is %.5f\n", weight(h, t));
    printf("Body mass index is %.5f\n", mass_ind(m, h));
	
    return 0;
}

float weight(float h, float t)
{
    float normal_weight = h * t / 240;
	
    return normal_weight;
}

float mass_ind(float m, float h)
{
    float mass_index = m / pow(h / 100, 2);
	
    return mass_index;	
}