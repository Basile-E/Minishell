#include <stdio.h>

void set_int(int *a, int *b, int *c, int *d)
{
	if (a != NULL)
		*a = 0;
	if (b != NULL)
		*b = 0;
	if (c != NULL)
		*c = 0;
	if (d != NULL)
		*d = 0;
}

int find_quote(char *str)
{
	int i;
	int g;
	set_int(&i, &g, NULL, NULL);
	printf("%i\n", i);
	printf("%i\n", g);
	return (i);
}

int main()
{
	char *str = "bonjour les loulou\n";
	find_quote(str);
}