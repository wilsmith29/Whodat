#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <limits.h>
#include <string.h>

#define OPTIONS "hpPlL:vY"

#define COWSAY "cowsay -W60"
#define CRESET "\033[0m"
#define COLOR  "\033[34;47m"

#ifndef FALSE
# define FALSE 0
#endif // FALSE
#ifndef TRUE
# define TRUE 1
#endif // TRUE

#define LOGON_INFO "last | grep %s"
#define LOGON_LINES_DEFAULT 2
#define LOGIN_LINE_LEN 200

int show_logon = 1;
int logon_records = 2;
int show_plan = 1;
int show_project = 1;
int verbose_level = 0;

void get_user_by_name(const char *);
void print_user_info(const struct passwd *);
void show_logon_info(const char *);
void display_help(const char *);
void display_plan(const char *);
void display_project(const char *);

int main(int argc, char *argv[])
{
    {
        int opt;

        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
            switch (opt) {
            case 'h':
                display_help(argv[0]);
		exit(EXIT_SUCCESS);
                break;
            
            case 'p':
		show_plan = 0;
		break;

            case 'P':
		show_project = 0;
		break;

            case 'l':
		show_logon = 0;
		break;

            case 'L':
		logon_records = atoi(optarg);
		if (logon_records < 0 || logon_records > 1000)
		{  
		   fprintf(stderr, "Invalid argument for -L. Using default.\n");
		   logon_records = LOGON_LINES_DEFAULT;
		}
		break;

	    case 'v':
		verbose_level++;
		fprintf(stderr, "verbose enabled %d\n", verbose_level);
		break;
                
            case 'Y':
            {
                char cowsay[LOGIN_LINE_LEN] = {'\0'};

                // This is just for fun.
                sprintf(cowsay, COWSAY " \"%s %s %s\"", COLOR, "You got this!!!", CRESET);
                system(cowsay);
                return EXIT_SUCCESS;
            }
                break;
            default: /* '?' */
                if (opt != '?') {
                  fprintf(stderr, "./whodat: invalid option -- '%c'\n", opt);
                }
		fprintf(stderr, "unknown option... Exiting.\n");
		exit(EXIT_FAILURE);
                break;
            }
        }
    }

    // Iterate through all the logon names on the command line.
    if (optind < argc)
    {
      for (int i = optind; i < argc; i++) {
        get_user_by_name(argv[i]);
      }
    }

    
    return EXIT_SUCCESS;
}

void get_user_by_name(const char *logname)
{
    struct passwd *pwd = getpwnam(logname);
    if (pwd == NULL) {
       fprintf(stderr, "User entry not found for logname = %s\n\n", logname);
       return;
    }

    if (verbose_level > 0)
       fprintf(stderr, ">> Fetching information for user %s <<\n", logname);

    print_user_info(pwd);

    if (show_logon)
      show_logon_info(logname);

    if (verbose_level > 0 && !show_logon)
       fprintf(stderr, ">> Logon information not requested for %s <<\n", logname);

    if (show_project)
      display_project(pwd->pw_dir);

    if (verbose_level > 0 && !show_project)
       fprintf(stderr, ">> Project information not requested for %s <<\n", logname);

    if (show_plan)
      display_plan(pwd->pw_dir);

    if(verbose_level > 0 && !show_plan)
      fprintf(stderr, ">> Plan information not requested for %s <<\n", logname);
 

   printf("\n");
}

void print_user_info(const struct passwd *pwd)
{
     printf("Logname: %s\n", pwd->pw_name);
     printf("\tName:  %s\n", pwd->pw_gecos);
     printf("\tUID:   %d\n", pwd->pw_uid);
     printf("\tGID:   %d\n", pwd->pw_gid);
     printf("\tHome:  %s\n", pwd->pw_dir);
     printf("\tShell: %s\n", pwd->pw_shell);
}

// Since the use of popen()/pclose() are kind of far in our future, I'm
// providing this code. It sure makes doing this part easy.
void show_logon_info(const char *logname)
{
    char cmd[LOGIN_LINE_LEN] = {'\0'};
    char logon[LOGIN_LINE_LEN] = {'\0'};
    FILE *logon_info = NULL;
    int logon_count = 0;

    fprintf(stdout, "Recent logon information:\n");
    sprintf(cmd, LOGON_INFO, logname);
    logon_info = popen(cmd, "r");
    if (logon_info != NULL) {
       while (fgets(logon, LOGIN_LINE_LEN, logon_info) != NULL) {
         printf("%s", logon);
         logon_count++;
         if (logon_count >= logon_records) {
           break;
         }
      }
        pclose(logon_info);
        if (logon_count == 0) {
            fprintf(stdout, "** No recent logon information. **\n");
        }
    }
    else {
        perror("could not popen() connection");
    }
}

void display_plan(const char *home_dir)
{
  char filename[256];
  FILE *file;
  char line[256];
  int line_count = 0;

  sprintf(filename, "%s/.plan", home_dir);

  file = fopen(filename, "r");
  if (file == NULL) {
    printf("Plan: N/A\n");
    return;
  }

  printf("Plan:\n");
  while (fgets(line, sizeof(line), file)) {
    printf("%s", line);
    line_count++;
  }

  if (line_count == 0 || line[strlen(line) - 1] != '\n') {
      printf("\n");
  }
  fclose(file);
}


void display_project(const char *home_dir)
{
  char filename[256];
  FILE *file;
  char line[256];
  int line_count = 0;

  sprintf(filename, "%s/.project", home_dir);

  file = fopen(filename, "r");
  if (file == NULL) {
    printf("Project: N/A\n");
    return;
  }

  printf("Project: \n");
  while (fgets(line, sizeof(line), file)) {
    printf("%s", line);
    line_count++;
  }

  if (line_count == 0 || line[strlen(line) - 1] != '\n') {
     printf("\n");
  }
  fclose(file);
}


void display_help(const char * program)
{
  printf("%s [-hpPlL:vY] [logname1] [logname2] ... [lognameN]\n", program);
  printf("\t-l\tDo not show recent logon information\n");
  printf("\t-L #\tNumber of recent logon records to show\n");
  printf("\t-p\tDo not show .plan\n");
  printf("\t-P\tDo not show .project\n");
  printf("\t-v\tVerbose mode\n");
  printf("\t-h\tShow help text and exit\n");
  printf("\t-Y\tYou got this!!!\n");
}
