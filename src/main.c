#include <stdio.h>

extern int bar_main(int argc, char ** argv);

int main(int argc, char ** argv)
{
	return bar_main(argc, argv);
}
