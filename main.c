#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "structures.h"
#include "queue.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  // setam bandwidth default = 1024 biti
  unsigned int bandwidth = DEFAULT_BWDTH;
  // alocam structurile necesare browserului
  char line[LINE_MAX_LEN];
  Tab* first = alocTab();
  TList tabList = alocListCel();
  tabList->info = first;
  TList last = tabList, crtCel = last;
  void* globalHistory = InitQ(sizeof(char) * URL_MAX_LEN);
  void* priorityQueue = InitQ(sizeof(Resource));
  TList downloadedRes = NULL, lastRes = NULL;
  // deschidem fisierele transmise ca parametru
  FILE* inFile = fopen(argv[1], "r");
  FILE* outFile = fopen(argv[2], "a");
  // citim linie cu line
  while (fgets(line, LINE_MAX_LEN, inFile)) {
    // verficam daca ajungem la sfarsit de fisier
    if (feof(inFile)) {
      break;
    }
    // inlaturam \n provocat de fgets
    // comparam intrarea cu fiecare comanda pt a executa
    line[strlen(line) - 1] = '\0';
    if (strstr(line, "set_band ")) {
      bandwidth = set_bandwidth(line);
    }
    if (strstr(line, "newtab")) {
      crtCel = inserLastTabList(last);
      last = crtCel;
    }
    if (strstr(line, "deltab")) {
      deleteLastTab(&tabList, &last, &crtCel);
    }
    if (strstr(line, "change_tab ")) {
      crtCel = changeTab(line, tabList);
    }
    if (strstr(line, "print_open_tabs")) {
      printOpenTabs(tabList, outFile);
    }
    if (strstr(line, "goto ")) {
      gotoCmd(line, crtCel, globalHistory);
      // deschidere pagina web => o secunda
      wait(1, bandwidth, &priorityQueue, &downloadedRes, &lastRes);
    }
    if (strstr(line, "back")) {
      back(crtCel, outFile);
    }
    if (strstr(line, "forward")) {
      forward(crtCel, outFile);
    }
    if (strstr(line, "del_history ")) {
      delHistoryEntries(line, globalHistory);
    } else {
      if (strstr(line, "history")) {
        history(&globalHistory, outFile);
      }
    }
    if (strstr(line, "list_dl")) {
      listDownloads(crtCel, outFile);
    }
    if (strstr(line, "wait")) {
      int time = atoi(command_param(line));
      wait(time, bandwidth, &priorityQueue, &downloadedRes, &lastRes);
    }
    if (strstr(line, "downloads")) {
      PrintDownloadsStatus(&priorityQueue, downloadedRes, outFile);
    } else {
      if (strstr(line, "download ")) {
        downloadResource(crtCel, &priorityQueue, line);
      }
    }
  }
  // eliberam resursele folosite
  DistrQ(&priorityQueue, elibSimpleArray);
  DistrQ(&globalHistory, elibSimpleArray);
  freeList(&tabList, elibTabs);
  freeList(&downloadedRes, elibSimpleArray);
  // inchidem fisierele date ca parametri
  fclose(outFile);
  fclose(inFile);
  return 0;
}