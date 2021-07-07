#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const char *DB = "directory.db";

struct entry0
{
    char name[20];
    char phone[20];
    struct entry0 *next;
};

typedef struct entry0 entry;

void add(char *,char *);
int search(FILE *,char *);
void list(FILE *);
int delete(FILE *,char *);

FILE * open_db_file();

void print_usage(char *, char *);

entry *load_entries(FILE *);

entry *create_entry_node(char *, char *);

void free_entries(entry *);

void write_all_entries(entry *);


int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        print_usage("Insufficient arguments", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1], "add") == 0)
    {
        if(argc != 4)
        {
            print_usage("Improper arguments for add", argv[0]);
            exit(1);
        }
    
        char *name = argv[2];
        char *phone = argv[3];
        add(name, phone);
        exit(0);
    }
    else if(strcmp(argv[1], "list") == 0)
    {
        if(argc != 2)
        {
            print_usage("Improper arguments for list", argv[0]);
            exit(1);
        }
    
        FILE *fp = open_db_file();
        list(fp);
        fclose(fp);
        exit(0);
    }
    else if(strcmp(argv[1], "search") == 0)
    {
        if(argc != 3)
        {
            print_usage("Improper arguments for search",argv[0]);
            exit(1);
        }

        FILE *fp = open_db_file();
        char *name = argv[2];
        if(search(fp, name) == 0)
        {
            printf("no match\n");
            fclose(fp);
            exit(1);
        }
        fclose(fp);
        exit(0);
    }
    else if(strcmp(argv[1], "delete") == 0)
    {
        if(argc != 3)
        {
            print_usage("Improper arguments for delete", argv[0]);
            exit(1);
        }
        FILE *fp = open_db_file();
        char *name = argv[2];
        if(delete(fp, name) == 0)
        {
            printf("no match\n");
            fclose(fp);
            exit(1);
        }
        fclose(fp);
        exit(0);
    }
    else
    {
        print_usage("Invalid command", argv[0]);
        exit(1);
    }
}

FILE *open_db_file()
{
    FILE *fp=fopen(DB, "r");
    if(fp == NULL)
    {
        perror("Couldn't open database file");
        exit(1);
    }
    return fp;
}
  
void free_entries(entry *p)
{
    entry *del = NULL;

    while(p != NULL)
    {
        del = p;
        p = p->next;
        free(del);
    }
}

void print_usage(char *message, char *progname)
{
    printf("Error : %s\n", message);
    printf("Usage: %s command arguments\n", progname);
    printf("\nAvailable commands: \n-------------------\n");
    printf("add name phonenumber\n");
    printf("    Used to add a new name to the database.\n    name and phone number should not have any\n    spaces in them\n");
    printf("search name\n");
    printf("    Prints the phone number of the given name\n    if it's there in the database. Prints\n    'no match' if not found\n");
    printf("list\n");
    printf("    Lists all names and phone numbers in the\n    system along with a total count at the end.\n");
    printf("delete name\n");
    printf("    Deletes the entry for the name in the database.\n    Prints 'no match' if there's no such name.\n");
}

entry * create_entry_node(char *name, char *phone)
{
    entry *ret;
    ret = malloc(sizeof(entry));
    strcpy(ret->name, name);
    strcpy(ret->phone, phone);
    ret->next = NULL;
    return ret;
}


entry *load_entries(FILE *fp)
{
    char name[20], phone[20];
    memset(name, '\0', 20);
    memset(phone, '\0', 20);
    entry *ret = NULL;
    entry *current = NULL;
    entry *tmp = NULL;
   
  
    while(fscanf(fp, "%20[^,\n],%20[^,\n]\n", name, phone) != EOF)
    {
        tmp = create_entry_node(name, phone);
        if (ret == NULL)
            ret = tmp;
        else
            current->next = tmp;
        current = tmp;
    }
    return ret;
}

void write_all_entries(entry * p)
{
    FILE *fp = fopen(DB, "w");
    while (p != NULL)
    {
        fprintf(fp, "%s,%s\n", p->name, p->phone);
        p = p->next;
    }
    fclose(fp);
}


void add(char *name, char *phone)
{
    FILE *fp = fopen(DB, "a");
    fprintf(fp, "%s,%s\n", name, phone);
    fclose(fp);
}

void list(FILE *db_file)
{
    int count = 0;

    entry *p = load_entries(db_file);
    entry *base = p;
    while(p!=NULL)
    {
        printf("%-20s : %10s\n", p->name, p->phone);
        p=p->next;
        count++;
    }

    printf("Total entries :  %d\n",count);

    free_entries(base);
}


int delete(FILE *db_file, char *name)
{
    entry *p = load_entries(db_file);
    entry *base = p;
    entry *prev = NULL;
    entry *del = NULL ;
    int deleted = 0;
    while(p!=NULL)
    {
        if(strcmp(p->name, name) == 0)
        {
            if(p == base)
            {
                base = p->next;
                del = p;
                free(del);
                deleted = 1;
                break;
            }
            else
            {
                del = p;
                prev->next = p->next;
                free(del);
                deleted = 1;
                break;
            }
        }
        
        prev = p;
        p = p->next;
    }

    write_all_entries(base);
    free_entries(base);
    return deleted;
}

int search(FILE *db_file, char *name)
{
    entry *p = load_entries(db_file);
    entry *base = p;
    int search = 0;

    while(p!=NULL)
    {
        if(strcmp(p->name, name) == 0)
        {
            printf("%s\n",p->phone);
            search = 1;
        }
        
        p = p->next;
    }

    free_entries(base);
    return search;
}
