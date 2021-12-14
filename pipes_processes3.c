#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>



/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */


        


int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid, pid2, pid3, status;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  if(argc < 1){
    printf("Please enter the grep argument\n");
    exit(1);
  }
  
  grep_args[1]= argv[1]; // set grep arg to the var from command line
  
  pipe(pipefd);
  pipe(pipefd2);

  pid = fork();

  if (pid == 0)
  {
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe
        dup2(pipefd[1], 1);//stdout
        //add dup
        // close unused hald of pipe

        close(pipefd[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // execute cat

        execvp("cat", cat_args);
        
        
   }
   else if(pid < 0){
     perror("cat fork error");
   }
   else{
        
     pid2 = fork();
     if (pid2==0){
        
       dup2(pipefd[0], 0);//stdin
       dup2(pipefd2[1], 1);
          //add dup
          // close unused hald of pipe
  
       close(pipefd[1]);
       close(pipefd2[0]);

          // execute grep

        execvp("grep", grep_args);
     }
      else if(pid2 < 0){
        perror("grep fork error");
      }

      else
      {
        // parent gets here and handles "cat scores"

        // replace standard output with output part of pipe
        pid3=fork();
        if (pid3== 0){
          dup2(pipefd2[0], 0);
          // close unused unput half of pipe

          close(pipefd[0]);
          close(pipefd[1]);
          close(pipefd2[1]);

          // execute sort

          execvp("sort", sort_args);
          
          }
          else if(pid3 < 0){
            perror("sort fork error");
          }
        }
      }
      int i;
      close(pipefd[1]);
      close(pipefd[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);
            
      for(i=0; i < 3; i++ ){
         wait(&status);}
           
  
}