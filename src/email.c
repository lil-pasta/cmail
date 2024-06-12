#include "email.h"
#include <curl/curl.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

int parse_delimited(char email_list[], char *parsed_emails[]) {
  char delim[] = ",";
  char *ptr = strtok(email_list, delim);

  int i = 0;
  while (ptr != NULL) {
    parsed_emails[i++] = ptr;
    ptr = strtok(NULL, delim);
  }
  return i;
}

void insert_emails_to_Email(struct Email *letter, char **emails,
                            int email_type) {
  int len;

  switch (email_type) {
  case (1): /* too  */
    /* need too initialize the 'too' array */
    letter->too = malloc(sizeof(*letter->too) * letter->size_too);

    for (int i = 0; i < letter->size_too; i++) {
      len = strlen(emails[i]);
      if ((letter->too[i] = malloc(len + 1)) == NULL) {
        perror("error allocating space for too entry");
        exit(EXIT_FAILURE);
      }
      memcpy(letter->too[i], emails[i], len + 1);
    }
    break;
  case (2): /* CC  */
    /* need cc initialize the 'cc' array */
    letter->cc = malloc(sizeof(*letter->cc) * letter->size_cc);

    for (int i = 0; i < letter->size_cc; i++) {
      len = strlen(emails[i]);
      if ((letter->cc[i] = malloc(len + 1)) == NULL) {
        perror("error allocating space for cc entry");
        exit(EXIT_FAILURE);
      }
      memcpy(letter->cc[i], emails[i], len + 1);
    }
  case (3): /* BCC */
    /* need bcc initialize the 'bcc' array */
    letter->bcc = malloc(sizeof(*letter->bcc) * letter->size_bcc);

    for (int i = 0; i < letter->size_bcc; i++) {
      len = strlen(emails[i]);
      if ((letter->bcc[i] = malloc(len + 1)) == NULL) {
        perror("error allocating space for bcc entry");
        exit(EXIT_FAILURE);
      }
      memcpy(letter->bcc[i], emails[i], len + 1);
    }
  default:
    break;
  }
}
