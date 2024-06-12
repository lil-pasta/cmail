#define MAX_TO 10

enum Too {
  TO = 1,
  CC,
  BCC,
};

/* parse options are serialized into the email struct which is used to
 *  generate the email message eventually sent using libcurl */
struct Email {
  char **too;
  int size_too; /* size of the too array */
  char **cc;
  int size_cc;
  char **bcc;
  int size_bcc;
  char *from;
  char *message;
  char *subject;
};

int parse_delimited(char email[],
                    char *sorted_emails[]); /* parse comma separated list */

void insert_emails_to_Email(struct Email *letter, char **emails,
                            int email_type);
