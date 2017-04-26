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

typedef struct config_var 
{
   char before[256];
   char after[256];
}SPVar;

SPVar get_var(char* line){
  SPVar v;
  sscanf(line,"%[^= ] = %s",v.before,v.after);
  return v;
}
void print_var_arr(SPVar* array,int size){
  for(int i=0;i<size;i++){
    printf("(%s),(%s)\n",array[i].before,array[i].after);
  }
}
int first_nonwhitespace(char* s){
    size_t i = 0;
    while(s[i] == ' ' || s[i] == '\t'|| s[i] == '\n' || s[i] == '\r' || s[i] == '\f' || s[i] == '\v')
    {
        ++i;
    }
    return i;

}


int main ()
{
  FILE *file = fopen (FILENAME, "r");
  SPVar var_array[14];
  int var_num=0;
  if (file != NULL){ 
    char line[MAXBUF];
    while(fgets(line, sizeof(line), file) != NULL){
          if(line[first_nonwhitespace(line)]!='#') 
            var_array[var_num++]=get_var(line);
                } // End while
                fclose(file);
        }
        print_var_arr(var_array,var_num);
}




