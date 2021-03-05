// Picior Catalin 312CB
// Fisierul contine declaratiile structurilor
// si functiilor necesare implementarii unei stive

#include <stdlib.h>
#include <string.h>
#ifndef _STACK_H_
#define _STACK_H_

typedef struct celst {
  struct celst* urm;
  void* info;
} TCelSt, *ACelSt;

typedef struct stack {
  size_t dime;
  ACelSt vf;
} TStack, *ASt;

#define VF(a) (((ASt)(a))->vf)
#define DIME(a) (((ASt)(a))->dime)

typedef void (*funElibEl)(void*);

void* InitS(size_t d);
int VidaS(void* a);
int Push(void* a, void* ae);
int Pop(void* a, void* ae);
void ResetS(void* a, funElibEl fun);
void DistrS(void** aa, funElibEl fun);

#endif /* _STACK_H_ */