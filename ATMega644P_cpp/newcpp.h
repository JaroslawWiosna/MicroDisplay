/*
 * newcpp.h
 *
 *  Created on: Feb 9, 2016
 *      Author: michal
 */

#ifndef NEWCPP_H_
#define NEWCPP_H_

#include <stdlib.h>

void * operator new(size_t size);
void operator delete(void * ptr);

void * operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	free(ptr);
}

void * operator new[](size_t size)
{
	return malloc(size);
}

void operator delete[](void * ptr)
{
	free(ptr);
}


#endif /* NEWCPP_H_ */
