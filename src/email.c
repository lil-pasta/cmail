#include "email.h"
#include <curl/curl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* copy file contents into the message field */
void file_copy(FILE *input, char **dest);

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
  case (1): /* too */
    /* need too initialize the 'too' array */
    if ((letter->too = malloc(sizeof(*letter->too) * letter->size_too)) ==
        NULL) {
      perror("error allocating space for too array");
      exit(EXIT_FAILURE);
    };

    for (int i = 0; i < letter->size_too; i++) {
      len = strlen(emails[i]);
      if ((letter->too[i] = malloc(len + 1)) == NULL) {
        perror("error allocating space for too entry");
        exit(EXIT_FAILURE);
      }
      memcpy(letter->too[i], emails[i], len + 1);
    }
    break;
  case (2): /* CC */
    /* need cc initialize the 'cc' array */
    if ((letter->cc = malloc(sizeof(*letter->cc) * letter->size_cc)) == NULL) {
      perror("error allocating space for cc array");
      exit(EXIT_FAILURE);
    };

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
    if ((letter->bcc = malloc(sizeof(*letter->bcc) * letter->size_bcc)) ==
        NULL) {
      perror("error allocating space for bcc array");
      exit(EXIT_FAILURE);
    };

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

/* according to RFC822 only these two user set fields are required */
void print_missing_options(int present_options) {
  printf("The following required options were missing:\n");
  if ((present_options & TOO_FLAG) != TOO_FLAG) {
    printf("\tA valid recipient (too, set with -t or --too)\n");
  }
  if ((present_options & FROM_FLAG) != FROM_FLAG) {
    printf("\tA valid sending email (from, set with --from)\n");
  }
  printf("For usage instructions call the program again with --help\n");
}

void upload_message_from_file(char *filePath, struct Email *letter) {
  FILE *fp;
  if ((fp = fopen(filePath, "r")) == NULL) {
    fprintf(stderr, "failed to open %s", filePath);
    exit(EXIT_FAILURE);
  } else {
    file_copy(fp, &letter->message);
    fclose(fp);
  };
}

void file_copy(FILE *fp, char **dest) {
  int buffSize, c;
  char *strBuff;
  buffSize = 64; /* starting buffer size */

  /* allocate starting buffer for the message to be loaded */
  if ((strBuff = malloc(sizeof(*strBuff) * buffSize)) == NULL) {
    perror("failed to allocate memory for message");
    exit(EXIT_FAILURE);
  }

  int len = 0;
  while ((c = fgetc(fp)) != EOF) {
    strBuff[len++] = c;
    /* if len of email being read has exceeded the buffer's size
     * we attempt to resize the buffer to accomodate */
    if (len == buffSize) {
      if ((strBuff = realloc(strBuff, sizeof(*strBuff) * (buffSize += 64))) ==
          NULL) {
        perror("failed to expand strBuff while reading file");
        exit(EXIT_FAILURE);
      }
    }
  }

  strBuff[len++] = '\0'; /* null terminate that silly goose */

  if ((*dest = malloc(sizeof(*strBuff) * len)) == NULL) {
    perror("failed to allocate memory for letter->message");
    exit(EXIT_FAILURE);
  }
  memcpy(*dest, strBuff, len);
}

/* generates the email message from the fields set in the Email struct */
void generate_email(struct Email *letter, char *message_buffer) {}
