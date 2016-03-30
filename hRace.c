#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include "oterm.h"

#define SHM_NAME "/shm1"
#define MAX_HORSES 32
#define MAX_RIGHT 120

struct s_race
{
	int i_winner;
	int i_ypos[MAX_HORSES];
};

int do_shm_open(const char *name, int oflag, mode_t mode)
{
	int i;

	errno=0;
	i=shm_open(name,oflag,mode);
	if (errno != 0)
	{
		fprintf(stderr,"ERR: do_shm_open %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}

int do_shm_unlink(const char *name)
{
	int i;

	errno=0;
	i=shm_unlink(name);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_shm_unlink %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}


int do_ftruncate(int fd, off_t length)
{
	int i;

	errno=0;
	i=ftruncate(fd,length);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_ftruncate %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}


void *do_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)

{
	void *vp;

	errno=0;
	vp=mmap(addr,length,prot,flags,fd,offset);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_ftruncate %s/n",strerror(errno));
		exit(0);
	}

	return(vp);
}


int do_munmap(void *addr, size_t length)
{
	int i;
	errno=0;
	i=munmap(addr,length);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_munmap %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}


int do_close(int i_fd)
{
	int i;
	errno=0;
	i=close(i_fd);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_close %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}


sem_t *do_sem_open(const char * semname, int oflag,mode_t mode,unsigned int value)
{

	sem_t * semaddy;
	errno=0;
	semaddy = sem_open(semname,oflag,mode,value);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_sem_open %s\n",strerror(errno));
		exit(0);
	}

	return(semaddy);
}


int do_sem_close(sem_t *semname)
{
	int i;
	errno=0;
	i=sem_close(semname);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_sem_close %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}

int do_sem_unlink(const char *semname)
{
	int i;
	errno=0;
	i=sem_unlink(semname);
	if(errno != 0)
	{
		fprintf(stderr,"ERR: do_sem_unlink %s\n",strerror(errno));
		exit(0);
	}

	return(i);
}


int max_array(int a[], int num_elements)
{
	int i, max=-32000;
	for (i=0; i<num_elements; i++)
	{
		if (a[i]>max)
		{
			max=a[i];
		}
	}
	return(max);
}


int maximumIndex(int a[], int size){


	int largest;
	int l=0;

	largest = a[0];
	int largestIndex = 0;

	for(l=0; l<size; l++)

	{

		if (a[l]>largest){
			largest = a[l];
			largestIndex=l;
		}

	}

	return largestIndex;	 

}

void child(int i_count,int i_fline,int i_rand,int i_yline)
{
	struct s_race *sp_s_race;
	int i_fd;

	sem_t *mutex = do_sem_open("/DUSTY",O_CREAT,0666,1);
	i_fd=do_shm_open(SHM_NAME,O_RDWR,0666);
	do_ftruncate(i_fd,sizeof(struct s_race));
	sp_s_race=do_mmap(NULL,sizeof(struct s_race),PROT_READ | PROT_WRITE, MAP_SHARED,i_fd,0);

	//fprintf(stderr,"i_winner=%i\n",sp_s_race->i_winner);
	srand(time(NULL)+getpid());

	while (i_count < i_fline)
	{ 
		i_rand = rand() % 100 * 8000;
		usleep(i_rand);


		sem_wait(mutex);
		o_term_gotoxy(1,i_count,i_yline);
		o_term_clear_line_from_cursor_left(1);
		printf("H\n");
		sp_s_race->i_ypos[i_yline]=i_count;

		i_count++;

		sem_post(mutex);

	}
	if (sp_s_race->i_winner == -1)
	{
		sp_s_race->i_winner = i_yline;
	}
	do_munmap(sp_s_race,sizeof(struct s_race));
	do_close(i_fd);
	do_sem_close(mutex);
	exit(0);
}


int main (int argc, char* argv[])
{

	o_term_hide_cursor(1); //Hide the cursor so it looks better

	o_term_clear(1); //Clean up the screen before we start

	struct s_race *sp_s_race;

	int i_status;
	int i_count = 0;
	int i_rand = 0;
	int i_fline = MAX_RIGHT; //How far right you want the horses to go
	int i_fd;

	int i_hcount = 30; //Declare 30 horses

	if (argv[1] > 0) //If a valid number less than 32 is recieved from CLI, replace previous number of horses
	{
		if (atoi(argv[1]) > MAX_HORSES)
		{
			perror("Cannot have more than MAX_HORSES");
			exit(1);
		}
		i_hcount = atoi(argv[1]);
	}


	int i_yline = i_hcount;
	int i_fcount = i_hcount;

	pid_t i_pid;


	sem_t *mutex = do_sem_open("/DUSTY",O_CREAT | O_EXCL,S_IRUSR | S_IWUSR,1);
	i_fd=do_shm_open(SHM_NAME, O_CREAT | O_EXCL | O_TRUNC | O_RDWR,0666);

	do_ftruncate(i_fd,sizeof(struct s_race));
	sp_s_race=do_mmap(NULL, sizeof(struct s_race), PROT_READ | PROT_WRITE, MAP_SHARED, i_fd,0);
	sp_s_race->i_winner = -1;



	while(i_fcount > 0) //Draw finish line
	{

		o_term_gotoxy(1,i_fline,i_fcount);
		printf("|%i\n",i_fcount);
		i_fcount--;

	}	

	while(i_yline > 0) //Fork loop
	{

		if ((i_pid = fork()) < 0) {
			perror("fork error");
			exit(1);
		}
		if (i_pid == 0)
			break;

		i_yline--;
	}

	if(i_pid==0)	//Am I a parent or child?
	{
		//Child Stuff
		child(i_count,i_fline,i_rand,i_yline); 


	}
	else
	{

		//Parent Stuff
		while ((waitpid(-1, &i_status, WNOHANG)) != -1 ) 
		{

			sem_wait(mutex);

			o_term_gotoxy(1, 0, (i_hcount + 2));
			o_term_clear_entire_line(1);

			if (sp_s_race->i_winner > 0)
			{
				printf("The winner is horse number %2d!\n",sp_s_race->i_winner); //Print winner
			}
			else
			{
				printf("Number %02i is in the lead!\n",maximumIndex(sp_s_race->i_ypos,i_hcount));
			}			
			sem_post(mutex);

			usleep(10); //Give up time slice

		}


		//Childred are done and cleaned, perform parent cleaning and release resources
		o_term_show_cursor(1);
		do_close(i_fd);
		do_munmap(sp_s_race,sizeof(struct s_race));
		do_sem_close(mutex);
		do_sem_unlink("/DUSTY");
		do_shm_unlink(SHM_NAME);

	}




	return (0);	
}

