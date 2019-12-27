#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 16384

#define ERROR_NO_FILE 1
#define ERROR_OPEN_FILE 2
#define ERROR_INVALID_FILE 3
#define ERROR_OUT_OF_MEMORY 4

const char *error_msg[] = {
    "",
    "usage: %s filename\n",
    "ERROR: cannot open file %s\n",
    "ERROR: file %s invalid\n",
    "ERROR: Out of Memory\n"
};

typedef struct _List_Element_
{
    char *word;
    char *word_guess;
    int wrong_guesses;
    struct _List_Element_ *next_element;
} ListElement;

void appendList(ListElement **list, ListElement *element)
{
  if (*list == NULL)
  {
    // New list
    *list = element;
  } else {
    // Append to end of existing list
    ListElement *last = *list;
    while (last->next_element != NULL)
      last = last->next_element;

    last->next_element = element;
  }
}

void deleteList(ListElement *list)
{
  ListElement *cur;
  while (list != NULL)
  {
    cur = list;
    list = list->next_element;
    free(cur->word);
    free(cur->word_guess);
    free(cur);
  }
}

int readWordList(ListElement **list, char *filename)
{
  FILE *wordfile = fopen(filename, "r");
  if (wordfile == NULL)
  {
    printf(error_msg[ERROR_OPEN_FILE], filename);
    exit(ERROR_OPEN_FILE);
  }

  char *line = malloc(BLOCK_SIZE);
  if (!line)
  {
    printf("%s", error_msg[ERROR_OUT_OF_MEMORY]);
    fclose(wordfile);
    exit(ERROR_OUT_OF_MEMORY);
  }

  size_t line_chars_read = 0;
  size_t line_chars_max = BLOCK_SIZE;

  while (fgets(line, (int)line_chars_max, wordfile) != NULL)
  {
    line_chars_read = strlen(line);

    while (line_chars_read == line_chars_max - 1 && line[line_chars_read - 1] != '\n')
    {
      line_chars_max *= 2;

      // Reallocate the line buffer with 2x the previous size.
      char *tmp_line = realloc(line, line_chars_max);
      if (!tmp_line)
      {
        printf("%s", error_msg[ERROR_OUT_OF_MEMORY]);
        free(line);
        fclose(wordfile);
        deleteList(*list);
        exit(ERROR_OUT_OF_MEMORY);
      }
      line = tmp_line;

      if (fgets(line + line_chars_read,
                (int)(line_chars_max - line_chars_read), wordfile) == NULL)
        break;

      // Update the total amount of bytes read for the next while iteration
      line_chars_read += strlen(line + line_chars_read);
    }

    char *line_ptr = line;

    while (isspace(*line_ptr)) line_ptr++;
    //printf("NO WHITESPACE LEFT : %s\n", line_ptr);

    char *p = line_ptr + strlen(line_ptr);
    while (p-- > line_ptr && isspace(*p)) {
      *p = '\0';
    }

    bool input_valid = true;

    if (*line_ptr == '\0')
      input_valid = false;

    char *line_start = line_ptr; 
    while (*line_ptr) {
      if (!((*line_ptr >= 'A' && *line_ptr <= 'Z')
            || (*line_ptr >= 'a' && *line_ptr <= 'z')))
      {
        input_valid = false;
        break;
      }

      line_ptr++;
    }

    if (!input_valid) {
      printf(error_msg[ERROR_INVALID_FILE], filename);
      free(line);
      fclose(wordfile);
      deleteList(*list);
      exit(ERROR_INVALID_FILE);
    }

    line_ptr = line_start;

    ListElement *element = calloc(1, sizeof(ListElement));
    if (element == NULL) {
      printf("%s", error_msg[ERROR_OUT_OF_MEMORY]);
      free(line);
      fclose(wordfile);
      deleteList(*list);
      exit(ERROR_OUT_OF_MEMORY);
    }

    size_t word_len = strlen(line_ptr);
    
    element->word = calloc(1, word_len + 1);
    element->word_guess = calloc(1, word_len + 1);
    if (element->word == NULL || element->word_guess == NULL) {
      printf("%s", error_msg[ERROR_OUT_OF_MEMORY]);
      free(line);
      if (element->word)
        free(element->word);
      if (element->word_guess)
        free(element->word_guess);
      free(element);
      fclose(wordfile);
      deleteList(*list);
      exit(ERROR_OUT_OF_MEMORY);
    }

    strncpy(element->word, line_ptr, word_len);

    memset(element->word_guess, '_', word_len);
    element->word_guess[0] = element->word[0]; // first letter is always shown
    element->word_guess[word_len] = '\0';
    element->next_element = NULL;

    appendList(list, element);
  }

  free(line);
  fclose(wordfile);

  return 0;
}

void play(ListElement *list) {
  /*ListElement *cur = list;
  while (cur) {
    printf("WORD: %s\n", cur->word);
    printf("WORD2: %s\n", cur->word_guess);
    cur = cur->next_element;
  }
  printf("\n");*/

  int words_total = 0;
  int words_correct = 0;
  while (list) {
    printf("%s (%d)\n", list->word_guess, list->wrong_guesses);
    printf("Your guess: ");
    char guess = 0;
    scanf(" %c", &guess);

    bool guess_correct = false;
    for (size_t i = 0; i < strlen(list->word); i++) {
      // Case insensitive comparison
      if (tolower(list->word[i]) == tolower(guess)) {
        guess_correct = true;
        list->word_guess[i] = list->word[i];
      }
    }

    if (!guess_correct) {
      list->wrong_guesses++;
    }

    bool next_word = false;
    if (list->wrong_guesses == 11) {
      printf("%s (x_x)\n", list->word);
      next_word = true;
    } else if (strcmp(list->word_guess, list->word) == 0) {
      printf("%s (%d)\n", list->word, list->wrong_guesses);
      list->wrong_guesses = 0;
      words_correct++;
      next_word = true;
    }

    if (next_word) {
      list = list->next_element;
      list->wrong_guesses = 0;
      words_total++;
    }
  }

  printf("won (%d/%d)\n", words_correct, words_total);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf(error_msg[ERROR_NO_FILE], argv[0]);
    exit(ERROR_NO_FILE);
  }

  ListElement *list = NULL;
  readWordList(&list, argv[1]);
  play(list);
  deleteList(list);

  return 0;
}
