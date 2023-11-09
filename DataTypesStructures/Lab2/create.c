#include <stdio.h>

int main(void)
{
	FILE *f = fopen("data.txt", "w");
	for (int i = 1; i < 31; i++)
		fprintf(f, "Surname%d,Name%d,123456,Street%d,personal,%d.%d.2000\n", i, i, i, (i % 28) + 1, (i % 12) + 1);
	for (int i = 31; i < 61; i++)
		fprintf(f, "Surname%d,Name%d,123456,Street%d,work,Position%d,Organization%d\n", i, i, i, i, i);
	fclose(f);
}