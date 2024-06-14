#define MAX_TO 10

/* flags for seeing which options are set */
#define FROM_FLAG (1 << 7)
#define TOO_FLAG (1 << 6)
#define SUBJECT_FLAG (1 << 5)
#define MESSAGE_FLAG (1 << 4)
#define CC_FLAG (1 << 3)
#define BCC_FLAG (1 << 2)

/* required arguments are represented here */
#define REQUIRED_BITMASK (15 << 4)

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
void generate_email(struct Email *letter, char *letter_buffer);
void print_missing_options(int present_options);
