// Picior Catalin 312CB
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "queue.h"
#include "utils.h"

// in acest fisier sunt doar functiile de scriere in fisier
// fun de afisare tab deschise
void printOpenTabs(TList tabList, FILE *out) {
  TList p;
  Tab *auxTab;
  unsigned int index = 0;
  // iteram prin lista de taburi
  for (p = tabList; p != NULL; p = p->urm) {
    // verificam daca s-a deschis o pagina
    auxTab = (Tab *)(p->info);
    if (auxTab->crt_webpage) {
      // scriem in fisier numele paginii
      fprintf(out, "(%u: %s)\n", index, auxTab->crt_webpage->url);
    } else {
      // nu exista pagina web deschisa
      fprintf(out, "(%u: empty)\n", index);
    }
    index++;
  }
}

// fun de afisare istoric global
void history(void **globalHistory, FILE *out) {
  // alocam un el aux si initializam o coada
  void *auxQueue = InitQ(sizeof(char) * URL_MAX_LEN);
  char *url = (char *)malloc(sizeof(char) * URL_MAX_LEN);
  // cat timp coada nu este vida
  while (!VidaQ(*globalHistory)) {
    // extragem un el din coada initiala
    // il scriem in fiser si il
    // aduagam in coada aux pentru a ii pastra ordinea
    ExtrQ(*globalHistory, url);
    fprintf(out, "%s\n", url);
    IntrQ(auxQueue, url);
  }
  // eliberam coada istoricului(este vida in acest moment)
  free(*globalHistory);
  // actualizam coada istoricului global
  *globalHistory = auxQueue;
  // eliberam res auxiliara
  free(url);
}

// fun de listare descarcari din pag curenta
void listDownloads(TList crtCel, FILE *out) {
  Tab *crtTab = (Tab *)(crtCel->info);
  // verificam daca pag web exista
  if (crtTab->crt_webpage) {
    Resource *resources = crtTab->crt_webpage->resources;
    int i, resourcesNum = crtTab->crt_webpage->nr_res;
    // scriem in fisier informatii despre toate resursele
    for (i = 0; i < resourcesNum; i++) {
      fprintf(out, "[%d - \"%s\" : %lu]\n", i, resources[i].name,
              resources[i].dimension);
    }
  }
}

// fun de printare a elem din coada de prioritate
// si din lista de resurse descarcate
void PrintDownloadsStatus(void **priorityQueue, TList downloadedRes,
                          FILE *out) {
  // aplicam acelasi procedeu ca la functia history
  void *auxQueue = InitQ(sizeof(Resource));
  Resource *auxRes = (Resource *)malloc(sizeof(Resource));
  while (!VidaQ(*priorityQueue)) {
    ExtrQ(*priorityQueue, auxRes);
    unsigned long remaining = auxRes->dimension - auxRes->currently_downloaded;
    fprintf(out, "[\"%s\" : %lu/%lu]\n", auxRes->name, remaining,
            auxRes->dimension);
    IntrQ(auxQueue, auxRes);
  }
  free(*priorityQueue);
  *priorityQueue = auxQueue;
  free(auxRes);
  // parcurgem lista de descarcari si scriem in fisier
  // informatiile necesare
  TList p = downloadedRes;
  for (; p != NULL; p = p->urm) {
    auxRes = (Resource *)(p->info);
    fprintf(out, "[\"%s\" : completed]\n", auxRes->name);
  }
}