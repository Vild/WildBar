#include <stdio.h>
#include "wildbar.h"

int main(int argc, char ** argv)
{
	int ret = 0;
	ret = bar_main(argc, argv);
	mem_freeleaks();
	return ret;
}
