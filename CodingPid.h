#ifndef __CodingPid_H
#define __CodingPid_H

void Speed_read(void);
void Coding_Init(void);
void CarUpstandInit(void);
void PID(void);

typedef struct _PID_t
{
  float P;
	float I;
	float D;
}PID_t;
#endif

