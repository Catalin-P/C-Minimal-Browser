// Picior Catalin 312CB
// Fisierul contine declaratiile structurilor
// si functiilor necesare implementarii unei cozi
#include <stdlib.h>
#include <string.h>
#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct celq {
  struct celq* urm;
  void* info;
} TCelQ, *ACelQ;

typedef struct queue {
  size_t dime;
  ACelQ ic, sc;
} TQueue, *AQ;

#define IC(a) (((AQ)(a))->ic)
#define SC(a) (((AQ)(a))->sc)
#define DIMEQ(a) (((AQ)(a))->dime)

typedef void (*funElibElQ)(void*);

void* InitQ(size_t d);
int VidaQ(void* a);
int IntrQ(void* a, void* ae);
int ExtrQ(void* a, void* ae);
void ResetQ(void* a, funElibElQ fun);
void DistrQ(void** aa, funElibElQ fun);
int ConcatQ(void* ad, void* as);

#endif /* _QUEUE_H_ */