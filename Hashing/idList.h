typedef struct idList *IDList;

#define MIN_ID (0)
#define MAX_ID (999999999)

IDList IDListCreate(int n, int unsortedList[]);

void IDListDestroy(IDList list);

int IDListContains(IDList list, int id);