/**
 * @file main.c
 * @author xnovos14
 * @author 
 * @brief Main
 */

#include <stdio.h>
#include "scanner.h"
#include "error.h"




int main(int argc, char const *argv[])
{
	FILE* file = stdin;
	if (argc == 2) {
		file = fopen(argv[1], "r");
	}
	if (file == NULL) {
		file = stdin;
	}

    TOKEN token;
    SpracujZnak(&token);
    fclose(file);
    
	return 0;
}
