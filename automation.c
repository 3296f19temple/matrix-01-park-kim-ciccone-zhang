#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>

//running this will use system() to run requested main methods in automated manner
//time outputs will be stored in Data/ folder. 
int main(){

  //list of commands used in automation
  char basic_op[100]; // = "./nParammult ";
  //char *basic_op;
  
  //basic_op = basic;
  printf("starting automated runs\n");
  strcpy(basic_op, "./nParammult ");
  
  
  for(int i = 0; i < 1001; i+= 50){//change increment and max vals for graph changes
     sprintf(basic_op, "time ./nParammult %d > Data/basicOp%d.txt 2>&1", i, i);
     system(basic_op);
  }
  printf("automated runs complete\n");
}
