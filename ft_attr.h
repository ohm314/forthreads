#ifndef _FT_ATTR_H_
#define _FT_ATTR_H_

#include <pthread.h>
#include "ft_consts.h"
#include "ft_data.h"

/*****************************************/
/*      attribute object routines        */
/*****************************************/

void forthread_attr_destroy(int *attr, int *info);


void forthread_attr_init(int *attr, int *info);

/*****************************************/
/*       mutex attribute routines        */
/*****************************************/

void forthread_mutexattr_destroy(int *attr,int *info);

void forthread_mutexattr_init(int *attr,int *info);

/*****************************************************/
/*    condition attriubute variable routines         */
/*****************************************************/


void forthread_condattr_destroy(int *attr,int *info);


void forthread_condattr_init(int *attr,int *info);

/**************************************************/
/*    barrier attribute variable routines         */
/**************************************************/


void forthread_barrierattr_destroy(int *attr,int *info);

void forthread_barrierattr_init(int *attr,int *info);


#endif //_FT_ATTR_H
