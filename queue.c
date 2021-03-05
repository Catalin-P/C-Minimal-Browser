// Picior Catalin 312CB
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// Functie de initializare coada
void* InitQ(size_t d) {
  // alocam coada + verificare
  AQ s = (AQ)malloc(sizeof(TQueue));
  if (!s) return NULL;
  // se seteaza dimiensiunea elem din coada
  s->dime = d;
  // coada vida
  s->ic = NULL;
  s->sc = NULL;
  // returnam pointerul cozii
  return (void*)s;
}

// verificare daca este coada vida
// intoarce 1 daca este vida si 0 daca nu este
int VidaQ(void* a) {
  // conditia de verificare coada goala
  if (IC(a) == NULL && SC(a) == NULL) {
    return 1;
  } else {
    return 0;
  }
}

// functie de inserare in coada
// intoarce 1 daca elemntul s-a inserat cu succes
int IntrQ(void* a, void* ae) {
  // alocare celula coada + verficare
  ACelQ aux = (ACelQ)malloc(sizeof(TCelQ));
  if (!aux) {
    return 0;
  }
  // alocare informatii coada + verificare
  aux->info = malloc(DIMEQ(a));
  if (!aux->info) {
    free(aux);
    return 0;
  }
  // conditie ultim el coada
  aux->urm = NULL;
  // copiam continutul de al parametrului la adresa celulei
  memcpy(aux->info, ae, DIMEQ(a));
  // inserare in coada vida
  if (VidaQ(a) == 1) {
    IC(a) = aux;
    SC(a) = aux;
  } else {
    // insesare in coada nevida
    SC(a)->urm = aux;
    SC(a) = aux;
  }
  return 1;
}

// functie de extragere el din coada
// intoare 1 daca s-a extras cu succes
// si 0 daca coada era vida
int ExtrQ(void* a, void* ae) {
  // verificam daca coada este vida
  if (VidaQ(a) == 1) {
    return 0;
  } else {
    // copiam continutul celulei de extras la adresa parametrului ae
    ACelQ aux = IC(a);
    memcpy(ae, aux->info, DIMEQ(a));
    IC(a) = aux->urm;
    // verificam daca coada a ramas vida
    if (IC(a) == NULL) {
      SC(a) = NULL;
    }
    // eliberam memoria aferenta elementului extras din coada
    free(aux->info);
    free(aux);
  }
  return 1;
}

// fun de resetare coada: transforma o coada intr-una vida
void ResetQ(void* a, funElibElQ fun) {
  // retinem prima celula din lista de celule a cozii
  ACelQ p = IC(a), aux;
  // cat timp sunt el in lista
  while (p) {
    // memoram adresa el curent
    aux = p;
    // avansam in lista
    p = p->urm;
    // distrugem informatie celulei cu ajutorul
    // unei functii custom
    fun(aux->info);
    free(aux);
  }
  // conditie coada nula
  IC(a) = NULL;
  SC(a) = NULL;
}

// distrugere coada = reset coada + eliberare el de tip TQueue
void DistrQ(void** aa, funElibElQ fun) {
  ResetQ(*aa, fun);
  free(*aa);
  *aa = NULL;
}

// functie de concatenare a doua cozi
// intoarce 1 daca s-a concat cu succes si 0 daca nu
int ConcatQ(void* ad, void* as) {
  // caz coada destinatie e vida
  if (VidaQ(ad)) {
    // ultima si prima celula a cozii destinatie
    // devin ultima si prima cel a cozii sursa
    IC(ad) = IC(as);
    SC(ad) = SC(as);
    // sursa devine vida
    IC(as) = NULL;
    SC(as) = NULL;
    return 1;
  }
  // daca sursa este vida -> stop
  if (VidaQ(as)) {
    return 1;
  }
  // daca ambele cozi sunt nevide
  if (!VidaQ(ad) && !VidaQ(as)) {
    // legam ultima cel a cozii destinatie
    // cu inceputul sursei
    SC(ad)->urm = IC(as);
    // actualizam sfarsitul cozii destinatie
    SC(ad) = SC(as);
    // sursa -> vida
    IC(as) = NULL;
    SC(as) = NULL;
    return 1;
  }
  return 0;
}