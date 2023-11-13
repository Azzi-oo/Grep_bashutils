#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

struct Flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
};

void parser(int argc, char *argv[], struct Flags *flags, char **pattern,
            char **filename) {
  int opt;
  while ((opt = getopt(argc, argv, "eivcln")) != -1) {
    switch (opt) {
    case 'e':
      flags->e = 1;
      break;
    case 'i':
      flags->i = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    default:
      fprintf(stderr, "Using: %s [-eivcln] <pattern> <name_file>\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  if (optind < argc) {
    *pattern = argv[optind++];
  } else {
    fprintf(stderr, "Don't focus name\n");
    exit(EXIT_FAILURE);
  }
  if (optind < argc) {
    *filename = argv[optind];
  } else {
    fprintf(stderr, "Don't pattern");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  struct Flags flags = {0, 0, 0, 0, 0, 0};
  char *pattern = NULL;
  char *filename = NULL;

  parser(argc, argv, &flags, &pattern, &filename);

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Ошибка открытия файла");
    return EXIT_FAILURE;
  }

  char line[MAX_LINE_LENGTH];
  int line_number = 1;
  int match_lines = 1;

  while (fgets(line, sizeof(line), file) != NULL) {
    int invert = (flags.v) ? 1 : 0;
    if ((invert && ((flags.i && strcasestr(line, pattern) == NULL) ||
                    (!flags.i && strstr(line, pattern) == NULL))) ||
        (!invert && ((flags.i && strcasestr(line, pattern) != NULL) ||
                     (!flags.i && strstr(line, pattern) != NULL)))) {
      match_lines++;
      if (flags.l) {
        printf("%s\n", filename);
        break;
      }
      if (flags.c) {
        printf("%d\n", match_lines);
        break;
      }
      if (flags.n) {
        printf("%d: %s", line_number, line);
      } else {
        printf("%s", line);
      }
    }
    line_number++;
  }

  fclose(file);

  return EXIT_SUCCESS;
}
