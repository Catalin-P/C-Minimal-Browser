// Picior Catalin 312CB
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "structures.h"

// functie initializare stiva
// aproape identica cu functia aferenta cozii
void* InitS(size_t d) {
  ASt s;
  s = (ASt)malloc(sizeof(TStack));
  if (!s) return NULL;
  s->dime = d;
  s->vf = NULL;
  return (void*)s;
}

// functie de inserare in sitva
// intoarce 1 daca s-a inserat cu succes si 0 daca nu
// aproape identica cu IntrQ
int Push(void* a, void* ae) {
  ACelSt new_top = (ACelSt)malloc(sizeof(TCelSt));
  if (!new_top) {
    return 0;
  }
  new_top->info = malloc(DIME(a));
  if (!new_top->info) {
    free(new_top);
    return 0;
  }
  // elementul este pus in "varful" stivei
  new_top->urm = VF(a);
  memcpy(new_top->info, ae, DIME(a));
  VF(a) = new_top;
  return 1;
}

// functie de verificare coada vida
// identica cu VidaQ
int VidaS(void* a) {
  if (VF(a)) {
    return 0;
  } else {
    return 1;
  }
}

// functie de extragere din stiva
// intoarce 1 daca s-a inserat cu succes si 0 daca nu
int Pop(void* a, void* ae) {
  // verificare stiva vida
  if (VidaS(a) == 1) {
    return 0;
  }
  ACelSt aux = VF(a);
  memcpy(ae, VF(a)->info, DIME(a));
  VF(a) = VF(a)->urm;
  free(aux->info);
  free(aux);
  return 1;
}

// functie resetare stiva = stiva vida
void ResetS(void* a, funElibEl fun) {
  // iteram prin lista de celule
  ACelSt cells = VF(a), aux;
  while (cells) {
    // retinem cel curenta si continuam parcurgerea
    aux = cells;
    cells = cells->urm;
    // eliberam info listei cu o functie custom generica
    fun(aux->info);
    // eliberam celula
    free(aux);
  }
  // stiva a devenit vida
  VF(a) = NULL;
}
// distrugere = reset + eliberarea elem TStack
void DistrS(void** aa, funElibEl fun) {
  ResetS(*aa, fun);
  free(*aa);
  *aa = NULL;
}