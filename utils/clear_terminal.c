#include <stdlib.h>

void clearTerminal() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}