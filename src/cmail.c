#include "email.h"
#include <curl/curl.h>
#include <getopt.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* corresponds to: from to subject message cc bcc */
int options_bitmask = 0b0;

/* use an enum for long options with no corresponding short option */
enum {
  FROM_OPTION = CHAR_MAX + 1,
  CC_OPTION,
  BCC_OPTION,
};

static char const short_options[] = "01:2:t:s:m:M:";

static struct option longopts[] = {
    {"help", no_argument, NULL, 1},
    {"from", required_argument, NULL, FROM_OPTION},
    {"too", required_argument, NULL, 't'},
    {"subject", required_argument, NULL, 's'},
    {"cc", required_argument, NULL, CC_OPTION},
    {"bcc", required_argument, NULL, BCC_OPTION},
    {"message", required_argument, NULL, 'm'},      /* message from cli */
    {"message-file", required_argument, NULL, 'M'}, /* message from file */
    {0, 0, 0, 0}};

/* print usage information */
void usage();
void save_text_field(int len, char *optarg, char **dest);

void save_text_field(int len, char *optarg, char **dest) {
  if ((*dest = malloc(len + 1)) == NULL) {
    perror("error allocating space for message");
    exit(EXIT_FAILURE);
  }
  memcpy(*dest, optarg, len + 1);
}

void usage() {
  printf("Usage: cmail [OPTIONS]...\n");
  printf("Send simple emails from your cli!\n");
  printf("\
Example: cmail --from me@example.com -t example@mail.com  -s 'hello' -m 'this is an example email'\n");
  printf("\
    , --from                Email from field (your email)\n\
  -t, --too                 Email too field (destination email, single or comma separated list)\n\
  -s, --subject             Email subject line\n\
    , --cc                  Targets to cc, comma separated list\n\
    , --bcc                 Targets to bcc, comma separated list\n\
  -m, --message             Body of the message field of the email\n\
  -M, --message-file        Absolute path to a file containing the email message body\n");
}

int main(int argc, char *argv[]) {
  CURL *curl;
  curl = curl_easy_init();
  char *emails[MAX_TO]; /* list of emails parsed from an option */
  int num_emails;
  size_t len;
  struct Email *letter = malloc(sizeof(*letter));

  int c; /* argument */
  while (1) {
    int opt_idx = 0;
    c = getopt_long(argc, argv, short_options, longopts, &opt_idx);

    if (c == -1) {
      break;
    }

    switch (c) {
    case FROM_OPTION:
      len = strlen(optarg);
      if ((letter->from = malloc(len + 1)) == NULL) {
        perror("error allocating space for from");
        exit(EXIT_FAILURE);
      }
      memcpy(letter->from, optarg, len + 1);
      options_bitmask |= FROM_FLAG;
      break;
    case 't':
      num_emails = parse_delimited(optarg, emails);
      letter->size_too = num_emails;
      insert_emails_to_Email(letter, emails, TO);
      options_bitmask |= TOO_FLAG;
      break;
    case 's':
      len = strlen(optarg);
      save_text_field(len, optarg, &letter->subject);
      options_bitmask |= SUBJECT_FLAG;
      break;
    case CC_OPTION:
      num_emails = parse_delimited(optarg, emails);
      letter->size_cc = num_emails;
      insert_emails_to_Email(letter, emails, CC);
      options_bitmask |= CC_FLAG;
      break;
    case BCC_OPTION:
      num_emails = parse_delimited(optarg, emails);
      letter->size_bcc = num_emails;
      insert_emails_to_Email(letter, emails, BCC);
      options_bitmask |= BCC_FLAG;
      break;
    case 'm':
      len = strlen(optarg);
      save_text_field(len, optarg, &letter->message);
      options_bitmask |= MESSAGE_FLAG;
      break;
    case 'M':
      printf("path to message: %s\n", optarg);
      options_bitmask |= MESSAGE_FLAG;
      break;
    case '?':
      usage();
      break;
    case 0:
      break;
    default:
      usage();
      break;
    };
  }

  if ((options_bitmask & REQUIRED_BITMASK) != REQUIRED_BITMASK) {
    print_missing_options(options_bitmask);
    exit(EXIT_FAILURE);
  }
  printf("%s\n", letter->subject);

  return 0;
}
