
#include <stdio.h>

int angle1;

int main() {

  angle1 = 0;

  cicle:
    angle1 = angle1 + 1;
    if (angle1 == 1) {
        printf("First\n");
    }
    else if(angle1 == 2) {
        printf("Second\n");
    }
    printf("Circle\n");

  if (angle1 != 10) {
    printf("Trying again\n");
    goto cicle;
  }

  return 0;
}