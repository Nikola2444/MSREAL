// Copyright [2018] <nikola>
#include <stdio.h>
#include <stdlib.h>
struct s_example {
  int a;
  int b;
  int (*s_add_fp)(int, int);
};

int add(int a, int b) {
  return a + b;
}

int main() {
  int a, b;
  int(*add_fp)(int, int) = add;
  struct s_example s = {
    .a = 5,
    .b = 6,
    .s_add_fp = add,
  };
  a = 10;
  b = 15;
  int c = add(a, b);
  printf("\n%d", c);
  c = add_fp(a, b);
  printf("\n%d", c);
  c = s.s_add_fp(s.a, s.b);
  printf("\n%d", c);
  return 0;
}
