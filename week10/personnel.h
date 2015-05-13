#define NAMESTRLEN      40
#define MAXSUBJECTS     8

typedef char namestr [NAMESTRLEN + 1];

typedef struct {
    namestr         given, others, family;
} fullname_t;

typedef struct {
    int             yy, mm, dd;
} date_t;

typedef struct {
    int             subjectcode;
    date_t          enrolled;
    int             status;
    int             finalmark;
} subject_t;

typedef struct {
    fullname_t      name;
    int             employeenumber;
    date_t          dob;
    date_t          datecommenced;
    int             status;
    int             annualsalary;
} staff_t;

typedef struct {
    fullname_t      name;
    int             studentnumber;
    date_t          dob;
    int             nsubjects;
    subject_t       subjects [MAXSUBJECTS];
} student_t;

staff_t     jane;
student_t   bill;

/** vim: set ft=c ts=4 sw=4 et : */
