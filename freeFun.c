// Picior Catalin 312CB
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "queue.h"

// acest fisier contine numai fun de elib elemente
// aceasta fun elibereaza o pag web(se da ca parametru fct DistrS
// si ResetS pt a elibera/ distruge el din stiva de back si cea de forward
void elibWebpage(void *webpage) {
  // verificam daca pag nu e deja nula
  Webpage *aux = (Webpage *)webpage;
  if (!aux) {
    return;
  }
  // verificam daca pag are resurse
  if (aux->resources) {
    // eliberam resursele
    free(aux->resources);
  }
  // eliberam pagina web
  free(aux);
}

// funtie de eliberare tab
// fucntie de elib pagina web + distrugere
// stive de back si forward;
// se elibereaza tabul
void elibTabs(void *crtTab) {
  Tab *tab = (Tab *)crtTab;
  DistrS(&(tab->back_stack), elibWebpage);
  DistrS(&(tab->forward_stack), elibWebpage);
  elibWebpage(tab->crt_webpage);
  free(tab);
}

// functie eliberare memorie vector simplu
// sau al unei structuri fara pointeri in campuri
void elibSimpleArray(void *array) { free(array); }

// eliberare elem unei liste
void freeList(TList *list, funFreeEl fun) {
  // parcurgem lista
  TList p = *list, aux;
  while (p) {
    // retinem el crt si avansam
    aux = p;
    p = p->urm;
    // folosim o functie de elib pt informatia din lista
    fun(aux->info);
    // eliberam celula
    free(aux);
  }
  // setam adresa listei la null
  *list = NULL;
}
