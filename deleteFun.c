// Picior Catalin 312CB
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "queue.h"

// am adaugat functii care se refera la stergerea unor el
// din browser
// fun de stergere ult tab
void deleteLastTab(TList *tabList, TList *last, TList *crtTab) {
  // daca in lista este un sg element, actualizam tabul curent la NULL
  if (*crtTab == *last) {
    *crtTab = (*last)->prev;
  }
  // retinem adresa ultimului tab
  TList aux = *last;
  Tab *auxTab = (Tab *)(aux->info);
  // daca in lista e doar un elem, lista devine vida
  if (*tabList == aux) {
    *tabList = (*tabList)->prev;
  }
  // actualizam ultimul tab
  *last = (*last)->prev;
  (*last)->urm = NULL;
  // eliberam datele paginii
  elibWebpage(auxTab->crt_webpage);
  DistrS(&(auxTab->back_stack), elibWebpage);
  DistrS(&(auxTab->forward_stack), elibWebpage);
  // eliberam tabul si celula din lista
  free(auxTab);
  free(aux);
}

// fun  care sterge un anumit nr de intrari din istoric
void delHistoryEntries(char *command, void *globalHistory) {
  // nr intrarilor
  int entriesNum = atoi(command_param(command)), crtEntries = 0;
  // alocam un sir de caractere pt a stoca elem din coada
  char *url = (char *)malloc(sizeof(char) * URL_MAX_LEN);
  // conditie resetare coada istoricului
  if (entriesNum == 0) {
    ResetQ(globalHistory, elibSimpleArray);
  } else {
    // extragem cate elemente este necesar
    // sau pana coada devine vida
    while (entriesNum != crtEntries) {
      if (VidaQ(globalHistory)) {
        break;
      }
      crtEntries++;
      ExtrQ(globalHistory, url);
    }
  }
  // eliberam resursa aux
  free(url);
}