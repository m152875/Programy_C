#include <stdio.h>
#include <stdbool.h>
#include <string.h>


bool rozdiel(int x, int y, int z)
{
  return (((x>=y) && x-y < z) || ((y>x) && y-x < z)) ? true : false;
}

int main()
{
  char arr[] = "12345";
  char znak;
  int n = strlen(arr);
  int posledne = arr[n-1];
  for (int i = 1; i < n; i++)
  {
    arr[i] = arr[i-1];
  }
  arr[0] = posledne;
  fscanf(stdin,"%c",&znak);
  fprintf(stdout,"%c",znak);
}