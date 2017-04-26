/* strtok example */
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#define FILENAME "example.config"
#define MAXBUF 1024 
#define DELIM " ="

typedef struct config_var 
{
   char before[256];
   char after[256];
}SPVar;

SPVar get_var(char* line){
  SPVar v;
  sscanf(line,"%[^'='^' ']=%s",v.before,v.after);

  return v;
}
void print_var_arr(SPVar* array,int size){
  for(int i=0;i<size;i++){
    printf("(%s),(%s)\n",array[i].before,array[i].after);
  }
}
char* trim_line(char *s) {
  while (isspace((unsigned char) *s)) s++;
  if (*s) {
    char *p = s;
    while (*p) p++;
    while (isspace((unsigned char) *(--p)));
    p[1] = '\0';
  }
  return s;
}

int main ()
{
  FILE *file = fopen (FILENAME, "r");
  SPVar var_array[15];
  int var_num=0;
  if (file != NULL){ 
    char line[MAXBUF];
    char* p = line;
    while(fgets(line, sizeof(line), file) != NULL){
          p=line;
//           p= trim_line(p);
          if(p[0]!='#') var_array[var_num++]=get_var(p);
                } // End while
                fclose(file);
        }
        print_var_arr(var_array,var_num);
}




