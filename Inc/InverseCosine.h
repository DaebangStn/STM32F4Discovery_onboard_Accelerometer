#ifndef INVERSECOSINE_H__
#define INVERSECOSINE_H__

#include "arm_common_tables.h"
#include "arm_math.h"
#define PRECESION 512

#if (PRECESION == 8)
#define BaseCoef twiddleCoef_16
#elif (PRECESION == 16)
#define BaseCoef twiddleCoef_32
#elif (PRECESION == 32)
#define BaseCoef twiddleCoef_64
#elif (PRECESION == 64)
#define BaseCoef twiddleCoef_128
#elif (PRECESION == 128)
#define BaseCoef twiddleCoef_256
#elif (PRECESION == 256)
#define BaseCoef twiddleCoef_512
#elif (PRECESION == 512)
#define BaseCoef twiddleCoef_1024
#elif (PRECESION == 1024)
#define BaseCoef twiddleCoef_2048
#endif

float32_t ArcCos(float32_t acos);
float32_t REC_ArcCos(float32_t acos, int val1, int val2);

#endif
