#include <iostream>
#include <cstdio>
#include <algorithm>

int Partition(int a[], int sp, int ep) {
  int p = (ep-sp)/2;
    int v = a[p];

    while (true) {
        while (a[sp] < v) ++sp;
        while (a[ep] >= v) --ep;

        if (sp < ep) std::swap(a[sp], a[ep]);
        else break;
    }

    std::swap(a[p], a[ep]);

    return ep;
}
void print(int a[], int n) {
  for(int i = 0; i < n; ++i)
    std::cout << a[i];
  std::cout << std::endl;
}

int QuickSelect(int a[], int sp, int ep, int k) {
  int p = Partition(a, sp, ep);
  print(a, 8);

  if (k < p+1) return QuickSelect(a, sp, p, k);
  else if (k > p+1) return QuickSelect(a, p, ep, k-p);
  else return a[p];
}


int main() {
    std::cout << "Hello World!" << std::endl;

    int a[] = {0, 1, 2, 3, 4, 5, 6, 7};
    std::cout << QuickSelect(a, 0, 7, 0);
    return 0;
}
