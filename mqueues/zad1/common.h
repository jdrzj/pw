#ifndef COMMON_H
#define COMMON_H

#define NAME kolejka
#define BUF_SIZE  4
#define TEXT_SIZE 100

typedef struct
{
	int typ;
	int nr_prod;
	char text[TEXT_SIZE];
} Message;

#endif
