#ifndef __AUTO_PID_H
#define __AUTO_PID_H

#include "stdbool.h"
// 
// C Binding for C++ Compilers
// 
#ifdef __cplusplus
extern "C"
{
#endif

void PID_Init(float* mInput, float* mOutput, double* mSetpoint, int* mTime, bool *mstartPID);
void PID_Run(void);
int getTime(void);
void AutoTune(void);
float abs(float number);

static struct{
	int (*getTime)(void);
} PID = 
{
	getTime,
};


#ifdef __cplusplus
}
#endif

#endif /* __FILTER_H */
