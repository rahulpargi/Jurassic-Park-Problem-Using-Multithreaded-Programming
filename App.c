#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
sem_t sem_visitor, sem_car; 
//Variable to terminate the program after serving no of passengers after a given amount of time
int CYCLES;

int main (){ 
  printf("\t\t------------Operating System Project------------\n\n");
  printf("\t\tJurrassic Park Problem Using Semaphores\n\n");
  int VISITORS;
  int CARS;
 // int CYCLES;
 
 //Input Validation
 
  printf("Enter the No of passengers:\n");
  scanf("%d",&VISITORS);
  if(VISITORS<0){
  	printf("Enter Positive no passengers only\n Try again\n");
  	exit(0);
  }
  printf("Enter the No of cars:\n");
  scanf("%d",&CARS);
  if(CARS<0){
  	printf("Enter Positive no cars only\n Try again\n");
  	exit(0);
  }
  printf("Enter the No of cycles:\n");
  scanf("%d",&CYCLES);
  if(CYCLES<0){
  	printf("Enter Positive no cycles only\n Try again\n");
  	exit(0);
  }
  
  pthread_t visi[VISITORS], car[CARS]; 
  void *Car (void *), *Visitor (void *); 
  int i, v[VISITORS]; 
  extern sem_t sem_car, sem_visitor; 
 //Initlializing the sem_car unnamed semaphore with pshared value=0
  sem_init (&sem_car, 0, 0); 
 //Initlializing the sem_visitor unnamed semaphore with pshared value=0
  sem_init (&sem_visitor, 0, 0); 
   
  srand (time (NULL));    
 //Threads creation for passengers  
  for (i=0; i<VISITORS; i++) 
  { 
    v[i] = i; 
printf("visitor %d",i) ;   
    if (pthread_create (&visi[i], NULL, Visitor, &v[i])) 
    {
	printf("Thread created Vis %d ",i);
    exit (1);
    } 
  } 
 //Thread creation for cars
  for (i = 0; i < CARS; i++) 
  { 
      v[i] = i; 
	printf("Car %d",i);
      if (pthread_create (&car[i], NULL, Car, &v[i])) 
      {
	printf("Car Thread created %d",i);
	exit (1);
	} 
  } 
   //Waiting for the passenger thread to terminate
  for (i=0; i<VISITORS; i++) 
  {
	printf("visitors waiting %d",i);
	  pthread_join (visi[i], NULL); 
	}
     
  printf("\n\t---- All Passengers are done with all cycles completed---- \n"); 
} 
 
//Passenger Processing
void *Visitor (void *p) 
{ 
  extern sem_t sem_car, sem_visitor; 
  int i, *index; 
   
  index = (int *)p; 
 
  for (i = 0; i < CYCLES; i++) 
    { 
      // Passengers Waiting at the museum. 
      fprintf (stdout, "Passengers Thread No:% d in the museum\n", *index+1); 
      sleep (rand()%4); 
 
      // Waiting for a car
     //Unlocking the sem_car semaphore
      sem_post (&sem_car); 
      
     //Locking the sem_visitor semaphore
      sem_wait (&sem_visitor); 
       
      // Passengers in car 
      fprintf (stdout, "Passengers Thread No:% d is in the car\n", *index+1); 
      sleep (rand()%3); 
 
      /* Free the car. */
    } 
    //For the program termination we are using cycles for eg afer serving passengers in 3 cycles the program will terminate
    
    fprintf (stdout, "Passengers Thread No:% d has finished his% d CYCLES.\n", *index+1, CYCLES); 
} 
 
//Car Processing
void *Car (void *p) 
{ 
  int *index, ride = 0; 
  extern sem_t sem_car, sem_visitor; 
  index = (int *) p; 
   
  printf("cr %d  waiting for passengers %d",sem_car,*index+1);
  /*The cars Waiting for Passengers*/
  while(1) 
    { 
      // Waiting untill the passengers are available 
      //Locking the sem_car variable untill passengers are available
      sem_wait (&sem_car); 
       
      //Take Safari 
      sleep (rand()%3); 
       
      //Go back to museum and turn down the passenger 
      //Unlocking the sem_visitor semaphore
      sem_post (&sem_visitor); 
      
    }  
printf("Car %d  finish with museum %d",sem_car,*index+1);
}
