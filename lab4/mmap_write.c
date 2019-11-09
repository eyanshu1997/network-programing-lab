 #include <unistd.h>
#include <fcntl.h>
#include<stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h> 
#include<stdlib.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <time.h> 
    #define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)
int writ(int fd1)
{
	int count=0;
	char x[1025]="wilccccThis will will will will be output to standard This will be output to standard This will be output to standard This will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard This will be output to standardThis will be output to standardThis will be output to standardThis will be output to standard";

	while(count<1048575)
	 {
		write(fd1, x, 1024) ;
		count=count+1;
    }
	write(fd1, x, 1023) ;
}

int mwrite(int fd,struct stat sb)
{
	char *addr;
    addr = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE,MAP_PRIVATE, fd, 0);
   if (addr == MAP_FAILED)
	   handle_error("mmap");

   for(int i=0;i<sb.st_size;i++)
		addr[i]='x';
	 // int s = write(STDOUT_FILENO, addr , 10);
	// if (s != 10) {
		// if (s == -1)
		   // handle_error("write");

	   // fprintf(stderr, "partial write");
	   // exit(EXIT_FAILURE);
	// }
	msync(addr, sb.st_size, MS_SYNC);
   munmap(addr,sb.st_size);
   close(fd);
 }

int main() 
{ 
	int fd;
	struct stat sb;
    int fd1 = open("hello.txt", O_WRONLY|O_CREAT,0777); 
    if (fd1 < 0)  
    { 
        perror("c1"); 
        exit(1); 
    } 
    //printf("opened the fd = % d\n", fd1); 
	clock_t t; 
    t = clock(); 
	writ(fd1);
	t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; 
    printf("Write() took %f seconds to execute \n", time_taken); 
    sync();
	if (close(fd1) < 0)  
    { 
        perror("c1"); 
        exit(1); 
    }  
    // printf("closed the fd.\n"); 
	sleep(2);
	// printf("starting nmap\n");
	
   fd = open("hello.txt", O_RDWR);
   if (fd == -1)
	   handle_error("open");

   if (fstat(fd, &sb) == -1)           
	   handle_error("fstat");
    t = clock(); 
	mwrite(fd,sb);
	t = clock() - t; 
	time_taken = ((double)t)/CLOCKS_PER_SEC; 
  
    printf("Mmap() took %f seconds to execute \n", time_taken); 
   	
	  close(fd);
	  int status = remove("hello.txt");
 
 
  return 0;
	  return 0;
}