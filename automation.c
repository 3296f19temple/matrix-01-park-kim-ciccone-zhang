#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<math.h>

//running this will use system() to run requested main methods in automated manner
//time outputs will be stored in Data/ folder.
int main(){

  FILE *dataNodeBasic;
  FILE *dataNodeSIMD;
  FILE *dataNodeOMP;
  FILE *graphData;
  
  //list of commands used in automation
  char pull_data[100];//full line pulled with fgets()
  char* time_dataBasic;//tokenized string of just real time
  char* time_dataSIMD;//tokenized string of just real time
  char* time_dataOmp;
  char time_dataBasic1[100];
  char insertCMD[100];
  char fileName[100];
  char graphMakeCMD[100];//command to generate graph
  char basic_op[100];
  char simd_op[100];
  char omp_op[100];
  int rand_size, entry_size;

  printf("starting automated runs\n");
  system("mkdir Data");
  system("touch Data/GraphData.txt");

  sprintf(graphMakeCMD, "gnuplot graph.gnu");
  graphData = fopen("Data/GraphData.txt", "a");

  for(int i = 100; i <501 ; i+= 100){//change increment and max vals for graph changes

     //state entry_size
     entry_size = i*i;
     //run BASIC op
     sprintf(basic_op, "time -p ./nParammult %d > Data/basicOp%d.txt 2>&1", i, i);
     printf("running basicOp n = %d\n", i);
     system(basic_op);
     
     //extracting data from operation BASIC Square
     sprintf(fileName, "Data/basicOp%d.txt", i);
     dataNodeBasic = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeBasic);
     fclose(dataNodeBasic);
     //printf("%s\n", pull_data);
     time_dataBasic = strtok(pull_data, " "); //gets token "real"
     time_dataBasic = strtok(NULL, " "); //gets token time value
     //printf("%s\n", time_dataBasic);

     //create insertCMD
     sprintf(insertCMD, "%d,     %s,     ", entry_size, time_dataBasic);
     fprintf(graphData, insertCMD);// adds i and BASIC Square time data into file
     
     //run SIMD op
     sprintf(simd_op, "time -p ./SIMDmult %d > Data/simdOp%d.txt 2>&1", i, i);
     printf("running simdOp n = %d\n", i);
     system(simd_op);

     //extracting data from operation SIMD Square
     sprintf(fileName, "Data/simdOp%d.txt", i);
     dataNodeSIMD = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeSIMD);
     fclose(dataNodeSIMD);
     time_dataSIMD = strtok(pull_data, " "); //gets token "real"
     time_dataSIMD = strtok(NULL, " "); //gets token time value
     //printf("%s\n", time_dataSIMD);
     //printf("%s\n", time_dataBasic);

     //creating insertCMD
     sprintf(insertCMD, "%s     ", time_dataSIMD);
     fprintf(graphData, insertCMD);// adds SIMD Square time data into file

     //printf("Starting Non-Sqaure runs\n");
     //create second size for non_square
     rand_size = (int)rand() % 100 + i - 100; //have %value be equal to increment value in for lloop
     //printf("rand_size made\n");
     entry_size = rand_size * rand_size;

     //run OMP op
     sprintf(omp_op, "time -p ./OMPmmult %d > Data/ompOp%d.txt 2>&1", i, i);
     printf("running ompOp n = %d\n", i);
     system(omp_op);
     
     //extracting data from operation OMP Square
     sprintf(fileName, "Data/ompOp%d.txt", i);
     dataNodeOMP = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeOMP);
     fclose(dataNodeOMP);
     //printf("%s\n", pull_data);
     time_dataOmp = strtok(pull_data, " "); //gets token "real"
     time_dataOmp = strtok(NULL, " "); //gets token time value
     //printf("%s\n", time_dataOmp);

     //create insertCMD
     sprintf(insertCMD, "%s     ", time_dataOmp);
     fprintf(graphData, insertCMD);// adds i and OMP Square time data into file

     //run BASIC op non-Square
     sprintf(basic_op, "time -p ./nParammultNS %d %d > Data/basicOpNS%d.txt 2>&1",rand_size, i, i);
     printf("running basicOp non-square n = %d, m = %d\n", i, rand_size);
     system(basic_op);
     //extracting data from operation BASIC non-Square
     sprintf(fileName, "Data/basicOpNS%d.txt", i);
     dataNodeBasic = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeBasic);
     fclose(dataNodeBasic);
     printf("%s\n", pull_data);
     time_dataBasic = strtok(pull_data, " ");//gets token "real"
     time_dataBasic = strtok(NULL, " ");//gets token time value
     printf("%s\n", time_dataBasic);

     //create insertCMD
     sprintf(insertCMD, "%d     %s     ", entry_size, time_dataBasic);
     fprintf(graphData, insertCMD);// adds BASIC non-Square time data into file

     //run SIMD op non-Square
     sprintf(simd_op, "time -p ./SIMDmultNS %d %d > Data/simdOpNS%d.txt 2>&1",rand_size, i, i);
     printf("running simdOp non-square n = %d, m = %d\n",i, rand_size);
     system(simd_op);

     //extracting data from operation SIMD non-Square
     sprintf(fileName, "Data/simdOpNS%d.txt", i);
     dataNodeSIMD = fopen(fileName, "r");
     fgets(pull_data, 10, dataNodeSIMD);
     fclose(dataNodeSIMD);
     time_dataSIMD = strtok(pull_data, " "); //gets token "real"
     time_dataSIMD = strtok(NULL, " ");//gets token time value
     printf("%s\n", time_dataSIMD);

     //create insertCMD
     sprintf(insertCMD, "%s\n", time_dataSIMD);
     fprintf(graphData, insertCMD);//adds SIMD non-Square time data into file
  }
  fclose(graphData);
  system(graphMakeCMD);
  printf("automated runs complete\n");
}
