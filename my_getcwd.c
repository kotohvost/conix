#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
/****************************************************************************/
			my_getcwd(buf, size)
/*              Получение полного пути до текущего каталога
*****************************************************************************/
	char    *buf;
	int     size;
{
	int     fd[2];
	int     status;
	pid_t   pid;
	int     i;
	int     readed;
/*==========================================================================*/
if(pipe(fd)){
  perror("Can't make pipe");
  exit(-1);
}

switch(pid=fork())
  {
    case 0: /* порожденный процесс */
      close(fd[0]);
      dup2(fd[1],1); /* переназначение стандартного вывода*/
      close(fd[1]);
      for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
      execl("/bin/pwd","pwd",(char*)0);
      perror("Can't exec /bin/pwd");
      exit(-1);
    break;

    case -1:
      perror("Can't fork");
      exit(-1);
    break;

    default: /* процесс-родитель*/
      close(fd[1]);

      while(wait(&status) != pid);
      if(status) return(-1);

      readed=read(fd[0], buf,size);
      close(fd[0]);

      if(readed<=0) return(-2);
      if(readed==size) return(-3);
      for(i=0;i<size && buf[i]!='\n'; i++);
      if(i<size)
	{
	  buf[i]='\0';
	  return(0);
	}else{
	  return(-4);
	}
    break;
  }
perror("Programmer's error in my_getcwd");
exit(-1);
}
