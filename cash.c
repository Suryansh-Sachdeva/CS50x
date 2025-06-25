#include <stdio.h>

int main() {
  int i;
  int j = 0;
  printf("Change owed: ");
  scanf("%d", &i);
  while (i < 0){
    printf("Change owed: ");
    scanf("%d", &i);
  }
  while (i!=0){
    if (i%25 >= 0 && i>=25){
      i = i - 25;
      j++;
    }
    else if (i%10 >= 0 && i>=10){
      i = i - 10;
      j++;
    }
    else if (i%5 >= 0 && i>=5){
      i = i - 5;
      j++;
    }
    else if (i%1 >= 0 && i>=1){
      i = i - 1;
      j++;
    }
  }
  printf("%d\n", j);
  return 0;
}
