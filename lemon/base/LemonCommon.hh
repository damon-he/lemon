/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      LemonCommon.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */

#ifndef LEMON_COMMON_HH
#define LEMON_COMMON_HH

#include <stdio.h>

const int LEMON_SUCCESS  =  0;
const int LEMON_FAILURE  = -1;

#define LEMON_CHECK(X)							\
do {											\
    if (!(X)) {									\
        printf("LEMON_CHECK failed, return\n");	\
		return;									\
    }											\
} while(0)

#define CHECK_BOOL 	   LEMON_CHECK
#define CHECK_FUN(FUN) LEMON_CHECK((FUN) == LEMON_SUCCESS)		
#define CHECK_PTR(PTR) LEMON_CHECK((PTR) != NULL)	
#define CHECK_EQ(A, B) LEMON_CHECK((A) == (B))
#define CHECK_NE(A, B) LEMON_CHECK((A) != (B))
#define CHECK_LT(A, B) LEMON_CHECK((A) <  (B))
#define CHECK_LE(A, B) LEMON_CHECK((A) <= (B))
#define CHECK_GT(A, B) LEMON_CHECK((A) >  (B))
#define CHECK_GE(A, B) LEMON_CHECK((A) >= (B))

#define LEMON_CHECK_VAL(X, Y)								\
do {														\
    if (!(X)) {												\
        printf("LEMON_CHECK_VAL failed, return"#Y"\n");	\
		return (Y);											\
    }														\
} while(0)

#define CHECK_BOOL_VAL 	      LEMON_CHECK_VAL
#define CHECK_FUN_VAL(FUN, C) LEMON_CHECK_VAL((FUN) == LEMON_SUCCESS, C)		
#define CHECK_PTR_VAL(PTR, C) LEMON_CHECK_VAL((PTR) != NULL, C)	
#define CHECK_EQ_VAL(A, B, C) LEMON_CHECK_VAL((A) == (B), C)
#define CHECK_NE_VAL(A, B, C) LEMON_CHECK_VAL((A) != (B), C)
#define CHECK_LT_VAL(A, B, C) LEMON_CHECK_VAL((A) <  (B), C)
#define CHECK_LE_VAL(A, B, C) LEMON_CHECK_VAL((A) <= (B), C)
#define CHECK_GT_VAL(A, B, C) LEMON_CHECK_VAL((A) >  (B), C)
#define CHECK_GE_VAL(A, B, C) LEMON_CHECK_VAL((A) >= (B), C)

#define CHECK_ASSERT(X)								\
do {												\
    if (X) {										\
        printf("CHECK_ASSERT "#X" failed\n");		\
		abort();									\
    }												\
} while(0)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) 			\
TypeName(const TypeName &);                         \
void operator=(const TypeName &)


class DataBuff {
public:
	unsigned char *data;
	unsigned int len;
	unsigned int size;

};


#endif
