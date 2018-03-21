#include "InverseCosine.h"

float32_t ArcCos(float32_t acos){
	return REC_ArcCos(acos, 0, PRECESION);
}

float32_t REC_ArcCos(float32_t acos, int val1, int val2){
	if(val2 - val1 == 2){
		return (PI * (val1 + val2) / (float32_t)(2*PRECESION));
	}
	float32_t temp = BaseCoef[val1+val2];
	if(acos > temp){
		return REC_ArcCos(acos, val1, (val1 + val2)/2);
	}else if(acos < temp){
		return REC_ArcCos(acos, (val1 + val2)/2, val2);
	}else{
		return (PI * (val1 + val2) / (float32_t)(2*PRECESION));
	}
}
