// Picior Catalin 312CB
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "queue.h"

// in acest fisier am adaugat functiile ce tin de
// orice tip de alocare de memorie
// alocare tab
Tab* alocTab() {
  // alocam memorie + verificare
  Tab* tab = (Tab*)malloc(sizeof(Tab));
  if (!tab) {
    return NULL;
  }
  tab->crt_webpage = NULL;
  // initializam o stiva + verificare
  tab->forward_stack = InitS(sizeof(Webpage));
  if (!tab->forward_stack) {
    free(tab);
    return NULL;
  }
  // alocam si cea de a doua stiva + verificare
  tab->back_stack = InitS(sizeof(Webpage));
  if (!tab->back_stack) {
    free(tab->forward_stack);
    free(tab);
    return NULL;
  }
  // returnam adresa tab-ului
  return tab;
}

// fun de aloca pag web cu lista de res aferenta
Webpage* alocWebpage(char* url) {
  // aloc + verificare
  Webpage* aux = (Webpage*)malloc(sizeof(Webpage));
  if (!aux) {
    return NULL;
  }
  // copiere nume url in pagina
  strcpy(aux->url, url);
  // folosim functia data in util pentru a completa
  // datele paginii web si lista de resurse
  aux->resources = get_page_resources(url, &(aux->nr_res));
  // returnam adresa paginii alocate
  return aux;
}

// alocam o celula de lista generica
TList alocListCel() {
  // alocare + verificare
  TList aux = (TList)malloc(sizeof(TCel));
  if (!aux) {
    return NULL;
  }
  aux->info = NULL;
  aux->urm = NULL;
  aux->prev = NULL;
  // returnam adresa celulei
  return aux;
}

// fun de inserare la sfarsit in lista de taburi
// primeste ca param ultimul tab si intoarce ultimul
// tab actualizat
TList inserLastTabList(TList last) {
  // alocare + verificare tab(info celula)
  Tab* auxTab = alocTab();
  if (!auxTab) {
    return NULL;
  }
  // alocare celula + verificare
  TList auxListCell = alocListCel();
  if (!auxListCell) {
    free(auxTab->back_stack);
    free(auxTab->forward_stack);
    free(auxTab);
    return NULL;
  }
  // adaugam in lista ultimul tab
  auxListCell->info = auxTab;
  last->urm = auxListCell;
  auxListCell->prev = last;
  last = auxListCell;
  // returnam adresa ultimului tab
  return last;
}