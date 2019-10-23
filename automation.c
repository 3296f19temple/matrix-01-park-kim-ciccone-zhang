#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>

//running this will use system() to run requested main methods in automated manner
//time outputs will be stored in Data/ folder.
int main(){

  FILE *dataNodeBasic;
  FILE *dataNodeSIMD;
  FILE *graphData;
  
  //list of commands used in automation
  char pull_data[100];//full line pulled with fgets()
  char* time_dataBasic;//tokenized string of just real time
  char* time_dataSIMD;//tokenized string of just real time
  char time_dataBasic1[100];
  char insertCMD[100];
  char fileName[100];
  char graphMakeCMD[100];//command to generate graph
  char basic_op[100];
  char simd_op[100];

  printf("starting automated runs\n");
  system("mkdir Data");
  system("touch Data/GraphData.txt");

  sprintf(graphMakeCMD, "gnuplot graph.gnu");
  graphData = fopen("Data/GraphData.txt", "a");

  for(int i = 0; i <501 ; i+= 100){//change increment and max vals for graph changes
     sprintf(basic_op, "time -p ./nParammult %d > Data/basicOp%d.txt 2>&1", i, i);
     printf("running basicOp n = %d\n", i);
     system(basic_op);
     
     //extracting data from operation
     sprintf(fileName, "Data/basicOp%d.txt", i);
     dataNodeBasic = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeBasic);
     fclose(dataNodeBasic);
     //printf("%s\n", pull_data);
     time_dataBasic = strtok(pull_data, " "); //gets token "real"
     time_dataBasic = strtok(NULL, " "); //gets token time value
     printf("%s\n", time_dataBasic);
     sprintf(insertCMD, "%d,     %s,     ", i, time_dataBasic);
     fprintf(graphData, insertCMD);

     sprintf(simd_op, "time -p ./SIMDmult %d > Data/simdOp%d.txt 2>&1", i, i);
     printf("running simdOp n = %d\n", i);
     system(simd_op);

     //extracting data from operation
     sprintf(fileName, "Data/simdOp%d.txt", i);
     dataNodeSIMD = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeSIMD);
     fclose(dataNodeSIMD);
     time_dataSIMD = strtok(pull_data, " "); //gets token "real"
     time_dataSIMD = strtok(NULL, " "); //gets token time value
     printf("%s\n", time_dataSIMD);
     printf("%s\n", time_dataBasic);

     //creating insertCMD
     sprintf(insertCMD, "%s\n", time_dataSIMD);
     fprintf(graphData, insertCMD);
  }
  fclose(graphData);
  system(graphMakeCMD);
  printf("automated runs complete\n");
}
