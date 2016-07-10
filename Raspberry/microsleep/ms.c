#include <stdio.h>
#include <time.h>

#define MYCLOCK CLOCK_MONOTONIC

/* Results CLOCK_MONOTONIC

  Compiled with -O2, no debugging, run as root
  Note: requested delay set to 90000 ns
  
  
  Empty delay: 1250 (two calls to clock_gettime)
  
  By default: delay rounds up to 165 us.

  Set scheduling to FIFO: chrt -f -p 10 PID
	delay down to 110 us
  Set scheduling to RR, chrt -r -p 10 PID
	delay down to 110 us
  Increase prio to 20 in FIFO
	delay down to 110 us
  	
  Change program: delay set to 5000 ns

  By default: delay rounds up to 77us.
  Increase prio to 20 in FIFO
	delay down to 27 us

  Locking application to core 1:  taskset  -p 1 PID
	No effect.
	 
  Changing commandline such that core 3 is isolated from scheduler
  Rebooting, starting application, setting FIFO scheduling
  Binding task to core 3, ps -aF shows that this core
  exclusively runs our task.
  
  	No effect, delay still in the order of 27 us
  
  Inspect whether core 3 runs interupts: cat /proc/interrupts
  
  	Core 3 only runs rescheduling and timer interrupts  
  

   Results CLOCK_THREAD_CPUTIME_ID, still with cpu 3 isolated
   
   Running immediately leads to delays in the order of 5 us!
   
   This is the result of CLOCK_THREAD_CPUTIME_ID not being
   supported by clock_nanosleep.
   
   Result: CLOCK_REALTIME, similar to CLOCK_MONOTONIC

   Result: CLOCK_PROCESS_CPUTIME_ID: process seems to hang
   
   Result: using nanosleep i.s.o. clock_nanosleep
   	
	Similar to clock_nanosleep.

   Results: using mhysleep (busy wait on time)
   
   	 Setting requested delay to 5 us leads to 7.5us
	 
   Making it realtime: hardly any change
   
   	Setting requested delay to 1ns leads to 3.3 us delay
	
   So 3.3 us delay seems to be minimal amount of delay we can
   get on this computer.

*/

void mysleep(int ndelay);


int main(int argc, char* argv[])
{
	const struct timespec delta = {
		0, 1500UL 
	
	};
	struct timespec start;
	struct timespec end;
	int r;
	
	printf("Starting.\n");
	
	clock_getres(MYCLOCK, &start);
	printf("clock resolution: %d ms, %d ns\n", start.tv_sec, start.tv_nsec);
	
	clock_gettime(MYCLOCK, &start);	
	clock_gettime(MYCLOCK, &end);
	printf("empty delay: %d\n", end.tv_nsec - start.tv_nsec);
	
	while (1) {
		clock_gettime(MYCLOCK, &start);
		// r = clock_nanosleep(MYCLOCK, 0, &delta, NULL);
		// r = nanosleep(&delta, NULL);
		mysleep(delta.tv_nsec);
		clock_gettime(MYCLOCK, &end);
		printf("result: %d, delay: %d\n", r, end.tv_nsec - start.tv_nsec);
		usleep(500000UL);
	}


}

void mysleep(int ndelay)
{
	struct timespec now;
	struct timespec end;

	clock_gettime(MYCLOCK, &end);
	end.tv_nsec+=ndelay;
	if (end.tv_nsec>1000000000UL) {
		end.tv_nsec -= 1000000000UL;
		end.tv_sec++;
	}		
	do {
		clock_gettime(MYCLOCK, &now);
	} while ((end.tv_sec > now.tv_sec) || 
		 ((end.tv_sec == now.tv_sec) &&
		  (end.tv_nsec > now.tv_nsec)));
}
