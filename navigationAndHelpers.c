// Picior Catalin 312CB
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "queue.h"

// in acest fisier am adaugat functiile care nu
// au un rezultat direct, ci doar ajuta la navigarea
// si utilizarea optima a browserului
// functia citeste o linie din fisier, iar in cazul
// in care aceasta are doua cuvinte, returneaza
// adresa inceputului celui de al doilea cuvant
char *command_param(char *command) {
  // lungimea comenzii
  int line_len = strlen(command), i;
  // parcurgem sirul pana intalnim ' '
  for (i = 0; i < line_len; i++) {
    if (command[i] == ' ') {
      // returnam urm pozitie dupa spatiu
      return command + i + 1;
    }
  }
  return NULL;
}

// functie de setare latime de banda descarcare
// returneaza val numerica a comenzii folosind
// functia de conversie atoi
unsigned int set_bandwidth(char *command) {
  return atoi(command_param(command));
}

// functie care schimba tabul curent din lista
// functia returneaza adresa noului tab
TList changeTab(char *command, TList tabList) {
  // determinam val numerica a indexului
  unsigned int index = atoi(command_param(command)), crt_index = 0;
  TList p;
  // parcurgem lista de taburi
  for (p = tabList; p != NULL; p = p->urm) {
    // daca indexul curent este cel cautat returnam celula curenta
    if (crt_index == index) {
      return p;
    }
    // incrementam indexul curent
    crt_index++;
  }
  // caz de baza(desi nereazilabil)
  return NULL;
}

// functia care seteaza pagina web a tabului curent
void gotoCmd(char *command, TList crtCel, void *globalHistory) {
  // determinam numele pag web
  char *url = command_param(command);
  Tab *crtTab = (Tab *)(crtCel->info);
  Webpage *aux = crtTab->crt_webpage;
  // alocam pagina curenta si informatiile necesare o functie
  // custom
  crtTab->crt_webpage = alocWebpage(url);
  // resetam stiva de forward
  ResetS(crtTab->forward_stack, elibWebpage);
  // introducem in numele paginii in istoricul global
  IntrQ(globalHistory, url);
  // daca era deschisa inainte o pagina web
  if (aux) {
    // o introducem in stiva de back
    Push(crtTab->back_stack, aux);
    // eliberam memoria el de tip Webpage
    free(aux);
  }

  // printf("%s\n", url);
}

// functie care actualizeaza pagina curenta la cea anterioara acesteia
void back(TList crtCel, FILE *out) {
  Tab *crtTab = (Tab *)(crtCel->info);
  // verificam ca stiva sa nu fie vida
  if (VidaS(crtTab->back_stack)) {
    fprintf(out, "can’t go back, no pages in stack\n");
  } else {
    // adaugam in sitva de forward pagina curenta
    Push(crtTab->forward_stack, crtTab->crt_webpage);
    // eliminam varful stivei si actualizam pagina curenta
    Pop(crtTab->back_stack, crtTab->crt_webpage);
  }
}

// functie care actualizeaza pagina curenta la cea urmatoare acesteia
void forward(TList crtCel, FILE *out) {
  Tab *crtTab = (Tab *)(crtCel->info);
  // verificam ca stiva sa nu fie vida
  if (VidaS(crtTab->forward_stack)) {
    fprintf(out, "can’t go forward, no pages in stack\n");
  } else {
    // adaugam in stiva de back pagina curenta
    Push(crtTab->back_stack, crtTab->crt_webpage);
    // eliminam varful sitvei forward si actualizam pagina curenta
    Pop(crtTab->forward_stack, crtTab->crt_webpage);
  }
}

// functie care adauga resurse in coada de prioritate
// algoritm: daca coada este vida , el se adauga in coada;
// altfel, initializam o coada aux si extragem el din coada de prioritate:
// *daca prioritatea este >= val el curent, el crt intra in coada de pr,
// la fel si el extras din coada de prioritate si concat cele doua cozi;
// *daca prioritatea < val el curent, inseamna ca el extras din coada de
// prioritati se insereaza in coada aux, iar daca coda de pr este vida,
// se insereaza si el crt in coada aux;
// la sfarsit, de concat coada aux cu cea de prioritate, si se modifica
// adresa cozii de pr;
void downloadResource(TList crtCel, void **priorityQueue, char *command) {
  Tab *crtTab = (Tab *)(crtCel->info);
  int IndexToDownload = atoi(command_param(command));
  // verificam daca exista o pagina web
  if (crtTab->crt_webpage) {
    // nr de resurse
    Resource *resources = crtTab->crt_webpage->resources;
    int resNum = crtTab->crt_webpage->nr_res;
    // daca pagina are resurse
    if (resNum) {
      // daca coada e vida
      if (VidaQ(*priorityQueue)) {
        // introd primul el in coada
        IntrQ(*priorityQueue, &resources[IndexToDownload]);
      } else {
        // alocam mem pt o resursa si init o coada aux
        Resource *auxRes = (Resource *)malloc(sizeof(Resource));
        // setam prioritatea cozii(mai mica este mai buna)
        unsigned long priority =
            resources[IndexToDownload].dimension -
            resources[IndexToDownload].currently_downloaded;
        void *auxQueue = InitQ(sizeof(Resource));
        // extragem repetat din pr queue
        while (!VidaQ(*priorityQueue)) {
          ExtrQ(*priorityQueue, auxRes);
          // calc prioritatea el curent
          unsigned long value =
              auxRes->dimension - auxRes->currently_downloaded;
          // se aplica algoritmul comm la definitia functiei
          if (priority <= value) {
            IntrQ(auxQueue, &resources[IndexToDownload]);
            IntrQ(auxQueue, auxRes);
            ConcatQ(auxQueue, *priorityQueue);
          } else {
            IntrQ(auxQueue, auxRes);
            if (VidaQ(*priorityQueue)) {
              IntrQ(auxQueue, &resources[IndexToDownload]);
            }
          }
        }
        // se elibereaza resursele alocate
        free(auxRes);
        free(*priorityQueue);
        // se actualizeaza adresa pr queue
        *priorityQueue = auxQueue;
      }
    }
  }
}

// functie ce simuleaza trecerea timpului si cum sunt afectate descarcarile
void wait(int time, unsigned int bandwidth, void **priorityQueue,
          TList *downloadedRes, TList *lastRes) {
  // setam nr total de biti ce vor fi descarcati
  unsigned long remainingData = time * (unsigned long)bandwidth;
  // alocam un elem resursa aux
  Resource *auxRes = (Resource *)malloc(sizeof(Resource));
  void *auxQueue = InitQ(sizeof(Resource));
  // extragem repetat din pr queue
  while (!VidaQ(*priorityQueue) && remainingData) {
    ExtrQ(*priorityQueue, auxRes);
    // retinem datele resursei curente
    unsigned long dim = auxRes->dimension;
    unsigned long crtDownload = auxRes->currently_downloaded;
    // daca resursa nu se descarca complet
    if (remainingData + crtDownload < dim) {
      // actualizam cat s-a descarcat
      auxRes->currently_downloaded += remainingData;
      // conditie de oprire while
      remainingData = 0;
      // se introduce res intr-o coada aux
      IntrQ(auxQueue, auxRes);
    } else {
      // se recalculeaza bitii ramasi
      remainingData -= (dim - crtDownload);
      // resursa s-a descarcat complet
      auxRes->currently_downloaded = dim;
      // retinem rezultatul intr-o lista de resurse
      TList auxListCell = alocListCel();
      Resource *auxResList = (Resource *)malloc(sizeof(Resource));
      memcpy(auxResList, auxRes, sizeof(Resource));
      auxListCell->info = auxResList;
      // daca lista de res este vida
      if (!(*downloadedRes)) {
        // modific adresa listei
        *downloadedRes = auxListCell;
        // actualizez ultimul elem
        *lastRes = auxListCell;
      } else {
        // adaugam el + actual maxim
        (*lastRes)->urm = auxListCell;
        *lastRes = auxListCell;
      }
    }
  }
  // concatenam listele pt a obtine resursele ramane la descarcat
  // in ordine
  ConcatQ(auxQueue, *priorityQueue);
  // eliberam resursele auxiliare + modificam adresa pr queue
  free(*priorityQueue);
  *priorityQueue = auxQueue;
  free(auxRes);
}