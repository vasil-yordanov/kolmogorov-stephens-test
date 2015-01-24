#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, char *argv[])
{
  int i, j, k, m, n, count, cnt;
  double dev;
  FILE *fl = NULL;
  FILE *fr = NULL;
  FILE *fw = NULL;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char *file_list;
  char *source_dir;
  char *target_dir;
  char filename[80];
  char target_filename[80];
  char *source_fileprefix;
  char fmt[80];
  bool is_first = true;
  if (argc != 6) {
     fprintf(stderr, "Error: missing parameters: <file_list> <source_dir> <source_fileprefix> <target_dir> <count>\n");
     fprintf(stderr, "Example: ./gather_procentiles exp/norm_unknown/list.txt exp/norm_unknown/ norm_unknown_ exp/norm_unknown/procentiles/ 1000000\n");
     exit(-1);
   }
  file_list = argv[1];
  source_dir = argv[2];
  source_fileprefix = argv[3];
  target_dir = argv[4];
  count = atoi(argv[5]);

  fl = fopen(file_list, "r");
  if (fl == NULL) {
     fprintf(stderr, "Can not open file: %s\n",file_list);
     exit(-1);
  }
  snprintf(fmt, sizeof fmt, "%s%%d_%%d_%%d.txt", source_fileprefix);
  printf("%s", fmt);
  
  while ((read = getline(&line, &len, fl)) != -1) {
    printf("Processing file: %s", line);
    sscanf(line,fmt, &cnt, &n, &k);
    printf("count=%d n=%d k=%d\n", cnt, n, k); 
    if (cnt != count) {
      fprintf(stderr, "Invalid file name: count (%d) is not equal to %d\n",cnt, count);
      exit(-1);
    }
    snprintf(filename, sizeof filename, "%s%s%d_%d_%d.txt", source_dir, source_fileprefix, cnt, n, k);
    fr = fopen(filename, "r");
    if (fr == NULL) {
       fprintf(stderr, "Can not open file: %s\n",filename);
       exit(-1);
    }
    i = 0; // line number
    j = 0; // procentile
    while ((read = getline(&line, &len, fr)) != -1) {
       if (i == j) {
          snprintf(target_filename, sizeof target_filename, "%s/percentile_%d.txt", target_dir, j/1000);
          if (is_first) fw = fopen(target_filename, "w");
          else fw = fopen(target_filename, "a");
          sscanf(line,"%lf\n", &dev);
//          fprintf(fw,"%d, %s",n,line);          
          if (n>=16) {
            fprintf(fw,"%f, %f\n",1.0/sqrt(n),1.0/dev);          
	  }
          fclose(fw);
          j+=1000;
       }
       i++;      
    }
    if (i<count) {
     fprintf(stderr, "There are few lines %d < %d in the file: %s\n",i, count, filename);
     exit(-1);
    }
    fclose(fr);
    is_first = false;
  }

  fclose(fl);
  return 0;
}
