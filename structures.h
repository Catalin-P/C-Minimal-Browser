// Picior Catalin 312CB
#include "utils.h"
#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_
#define URL_MAX_LEN 26
#define LINE_MAX_LEN 257
#define DEFAULT_BWDTH 1024

// declararea structurilor si a functiilor necesare
// implementarii unui browser
typedef struct webpage {
  char url[URL_MAX_LEN];
  int nr_res;
  Resource *resources;
} Webpage;

typedef struct tab {
  Webpage *crt_webpage;
  void *back_stack;
  void *forward_stack;
} Tab;

typedef struct cel {
  void *info;
  struct cel *urm;
  struct cel *prev;
} TCel, *TList;

typedef void (*funFreeEl)(void *);

void elibWebpage(void *webpage);
void console(char *input, char *output);
char *command_param(char *command);
unsigned int set_bandwidth(char *command);
Tab *alocTab();
TList alocListCel();
TList inserLastTabList(TList last);
void deleteLastTab(TList *tabList, TList *last, TList *crtTab);
TList changeTab(char *command, TList tabList);
void printOpenTabs(TList tabList, FILE *out);
void gotoCmd(char *command, TList crtCel, void *globalHistory);
void back(TList crtCel, FILE *out);
void forward(TList crtCel, FILE *out);
void elibString(void *string);
void delHistoryEntries(char *command, void *globalHistory);
void listDownloads(TList crtCel, FILE *out);
void downloadResource(TList crtCel, void **priorityQueue, char *command);
void wait(int time, unsigned int bandwidth, void **priorityQueue,
          TList *downloadedRes, TList *lastRes);
void PrintDownloadsStatus(void **priorityQueue,
	TList downloadedRes, FILE *out);
void elibTabs(void *crtTab);
void freeList(TList *list, funFreeEl fun);
void history(void **globalHistory, FILE *out);
void elibSimpleArray(void *array);
Webpage *alocWebpage(char *url);

#endif /* _STRUCTURES_H_ */