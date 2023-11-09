#include <stdio.h>

int triangle(float x1, float y1, float x2, float y2, float x3, float y3);

float sc(float x1, float y1, float x2, float y2, float x3, float y3);

int triangle_exist(float x1, float y1, float x2, float y2, float x3, float y3);

int main(void)
{
	float x1, y1, x2, y2, x3, y3;
	int rc, ret_key;
	
	printf("Input x1, y1, x2, y2, x3, y3: ");
	rc = scanf("%f %f %f %f %f %f", &x1, &y1, &x2, &y2, &x3, &y3);
	if ((rc == 6) && (triangle_exist(x1, y1, x2, y2, x3, y3)))
	{
		printf("Triangle type: %d", triangle(x1, y1, x2, y2, x3, y3));
		ret_key = 0;
	}		
	else
	{
		printf("Input error");
		ret_key = 1;
	}
	return ret_key;
}

int triangle_exist(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (((x1 - x2) * (y1 - y3) - (x1 - x3) * (y1 - y2) > 0) || ((x1 - x2) * (y1 - y3) - (x1 - x3) * (y1 - y2) < 0));
}

int triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	float sc1, sc2, sc3;
	int type;
	
	sc1 = sc(x1, y1, x2, y2, x3, y3);
	sc2 = sc(x2, y2, x1, y1, x3, y3);
	sc3 = sc(x3, y3, x1, y1, x2, y2);
	if (sc1 * sc2 * sc3 > 0)
		type = 0;
	else if (sc1 * sc2 * sc3 < 0)
		type = 2;
	else
		type = 1;
		
	return type;
}

float sc(float x1, float y1, float x2, float y2, float x3, float y3)
{
	float sc = (x1 - x2) * (x1 - x3) + (y1 - y2) * (y1 - y3);
	
	return sc;
}