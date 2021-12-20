#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool **alloc_table(int N) {
  bool **table = (bool **) calloc(N, sizeof(bool *));
  for (int i = 0; i < N; i++) {
    *(table + i) = (bool *) calloc(N, sizeof(bool));
  }
  
  return table;
}

bool can_place(bool **table, int N, int i, int j) {
  for (int k = 0; k < N; k++) {
    if (table[i][k]) return false;
    if (table[k][j]) return false;
  }

  int m = i - 1;
  int n = j - 1;
  while (m >= 0 && n >= 0) {
    if (table[m][n]) return false;
    m--;
    n--;
  }
  m = i - 1;
  n = j + 1;
  while (m >= 0 && n < N) {
    if (table[m][n]) return false;
    m--;
    n++;
  }

  return true;
}

bool fill_table(bool **table, int N, int currentRow) {
  if (currentRow == N) return true;
  for (int i = 0; i < N; i++) {
    if (can_place(table, N, currentRow, i)) {
      table[currentRow][i] = true;
      if (fill_table(table, N, currentRow + 1)) return true;
      else table[currentRow][i] = false;
    }
  }

  return false;
}

int main(void) {
  FILE *in, *out;
  in = fopen("sah.in", "r");
  out = fopen("sah.out", "w");
  int N;
  fscanf(in, "%d", &N);
  bool **table = alloc_table(N);
  bool is_possible = fill_table(table, N, 0);

  if (!is_possible) {
    printf("Impossible\n");
    return 0;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if(table[i][j]) fprintf(out, "(%d, %d)\n", i, j);
    }
    free(table[i]);
  }
  free(table);

  fclose(in);
  fclose(out);

  return 0;
}