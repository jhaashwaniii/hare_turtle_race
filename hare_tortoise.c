#include "cs_thread.h"

struct Repositioning {
	char player; 		// T for turtle and H for hare
	int time; 		// At what time god interrupt's
	int distance;		// How much does god move any of the player. 
							// distance can be negetive or positive.
							// using this distance if any of players position is less than zero then bring him to start line.
							// If more than finish_distance then make him win.
							// If time is after completion of game than you can ignore it as we will already have winner.
};

struct race {
	
	//	Don't change these variables.
	//	speeds are unit distance per unit time.
	int printing_delay;
	int tortoise_speed;					// speed of Turtle
	int hare_speed;						// speed of hare
	int hare_sleep_time; 				// how much time does hare sleep (in case he decides to sleep)
	int hare_turtle_distance_for_sleep; // minimum lead which hare has on turtle after which hare decides to move
										// Any lead greater than this distance and hare will ignore turtle and go to sleep
	int finish_distance;				// Distance between start and finish line
	struct Repositioning* reposition;	// pointer to array containing Randomizer's decision
	int repositioning_count;			// number of elements in array of repositioning structure
	
	//	Add your custom variables here.

	
	
};

long hare_distance=0;
long turtle_distance=0;
long hare_time=0;
long turtle_time=0;
long current_time=0;
pthread_mutex_t countMutex;
pthread_cond_t reportTurn=PTHREAD_COND_INITIALIZER;
pthread_cond_t turtleTurn=PTHREAD_COND_INITIALIZER;
pthread_cond_t hareTurn=PTHREAD_COND_INITIALIZER;
pthread_cond_t randomTurn=PTHREAD_COND_INITIALIZER;
int reportDone=1;
int turtleDone=0;
int hareDone=0;
int randomDone=0;
int sleeping=0;  
void* Turtle(void *race);
void* Hare(void *race);
void* Randomizer(void *race);
void* Report(void *race);

char init(struct race *race)
{
	pthread_t turtle_thread_id,hare_thread_id,report_thread_id,randomizer_thread_id;
	pthread_create(&randomizer_thread_id,NULL,Randomizer,race);
	pthread_create(&turtle_thread_id,NULL,Turtle,race);
    pthread_create(&hare_thread_id,NULL,Hare,race);
    pthread_create(&report_thread_id,NULL,Report,race);
    
	// pthread_cond_signal(&reportTurn); 
	pthread_join (randomizer_thread_id, NULL);
	pthread_join (turtle_thread_id, NULL);
    pthread_join (hare_thread_id, NULL);
    pthread_join (report_thread_id, NULL);
	// printf("HareTime----%ld\n",hare_distance);
	// printf("TurtleTime------%ld\n",turtle_distance);
	// printf("totalTime--%ld\n",current_time);
	if(hare_distance==turtle_distance){
		return 'T';
	}
	if(hare_distance>=(race->finish_distance)){
		return 'H';
	}
	if(turtle_distance>=(race->finish_distance)){
		return 'T';
	}
	else{
	return '-'; 
	}
}

void* Turtle(void *arg)
{
	  struct race *race = (struct race*)arg;
while(1){
	  if(hare_distance>=(race->finish_distance)||turtle_distance>=(race->finish_distance)){
		  break;
	  }	  
	pthread_mutex_lock (&countMutex);
	 while(!randomDone){
         pthread_cond_wait(&turtleTurn,&countMutex);
	}

	//   printf("i am in turtle\n");
	if((turtle_distance<(race->finish_distance))) {
		turtle_distance=turtle_distance+race->tortoise_speed;
		turtle_time++;
	}
    turtleDone=1;
	randomDone=0;
    pthread_cond_signal (&hareTurn);
    pthread_mutex_unlock (&countMutex);
}
	return NULL;
  
}

void* Hare(void *arg)
{
	struct race *race = (struct race*)arg;
while(1){
	  if(hare_distance>=(race->finish_distance)||turtle_distance>=(race->finish_distance)){
		  break;
	  }	
	pthread_mutex_lock (&countMutex);
	 while(!turtleDone){
         pthread_cond_wait(&hareTurn,&countMutex);
	}

	//   printf("i am in hare\n");

	if(hare_distance<(race->finish_distance)&& turtle_distance<(race->finish_distance)){
        if(sleeping>0 && sleeping<race->hare_sleep_time){
			sleeping++;
			hare_time++;
		}
		else{
		if((hare_distance-turtle_distance)>=race->hare_turtle_distance_for_sleep)
		{
			sleeping++;
			hare_time++;	
		}
		else{
		hare_distance=hare_distance+race->hare_speed;
        hare_time++;
		sleeping=0;
		}
		}
	}
    hareDone=1;
	turtleDone=0;
    pthread_cond_signal (&reportTurn);
    pthread_mutex_unlock (&countMutex);
}
	return NULL;
}


void* Randomizer(void *arg)
{
	 struct race *race = (struct race*)arg;
     while(1){
	  if(hare_distance>=(race->finish_distance)||turtle_distance>=(race->finish_distance)){
		  break;
	  }

	  pthread_mutex_lock (&countMutex);
	  while(!reportDone){
        pthread_cond_wait(&randomTurn,&countMutex);
	}

//    printf("i am in Randomizer\n");
	
	if(turtle_distance<(race->finish_distance)|| hare_distance<(race->finish_distance))
	{
	 	struct Repositioning *temp=race->reposition;
	 	for(int i=0;i<race->repositioning_count;i++){
			
			// printf("timedValue--%d\n",temp->time);
			// printf("playerValue--%c\n",temp->player);
			// printf("distanceValue--%d\n",temp->distance);
	 		if(current_time==temp->time){
				printf("current-time %ld\n",current_time); 
			 if(temp->player=='T'){
				turtle_distance=turtle_distance+temp->distance;
				if(turtle_distance<0){
					turtle_distance=0;
				}
			}
			if(temp->player=='H'){
				hare_distance=hare_distance+temp->distance;
				if(hare_distance<0){
					hare_distance=0;
				}
			}
	 		break;      
	      }
		  temp++;
	 		
		}
	}	
	 randomDone=1;
	 reportDone=0;
     pthread_cond_signal (&turtleTurn);
     pthread_mutex_unlock (&countMutex);
	
}
return NULL;
}

void* Report(void *arg)
{
	 struct race *race = (struct race*)arg;
	
  while(1){
	  if(hare_distance>=(race->finish_distance)||turtle_distance>=(race->finish_distance)){
		  break;
	  }
	 pthread_mutex_lock (&countMutex);
	 while(!hareDone){
         pthread_cond_wait(&reportTurn,&countMutex);
	}
         
	// printf("i am in Report\n");
	    current_time++;
	reportDone=1;
	hareDone=0;
    pthread_cond_signal (&randomTurn);
    pthread_mutex_unlock (&countMutex);
	
}
return NULL;
}  

