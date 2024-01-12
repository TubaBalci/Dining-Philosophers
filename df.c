#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define THINKING 0
#define THIRSTY 1
#define POURING 2
#define DRINKING 3

const char *state_dict[4];
int *state;
int N;
int K;
sem_t* cup_and_plate;
sem_t sugar;
pthread_mutex_t tea_pot;
int* cay_counts;
int total_cay;

int f = 0;

void print_status(){
	for(int j=0; j<f; j++){
		printf("\033[F");
	}

	printf("Eklenen cay %d     \n", K);
	for(int i=0; i<N; i++){
		printf("P%d: %s%d)    \n", i+1, state_dict[state[i]], cay_counts[i]);
	}
	printf("kalan cay: %d     \n", total_cay);
	printf("-----\n");
}

void* drink_cay(void* param){
    int index = *(int *)param;
    int ph_id = index + 1;
    cay_counts [index] = 0;
    srand(time(NULL));
    state[index] = 0; // thinking
    sleep(1);
    if(ph_id % 2 == 0)// cift ise cayi sekersiz iciyor.
    {
        do {          
            state[index] = 1; // thirsty
            sem_wait(&cup_and_plate[index % N]);
            sem_wait(&cup_and_plate[(index+ 1) % N]);
            pthread_mutex_lock(&tea_pot);            
            state[index] = 2; // pouring
            cay_counts [index] += 1;
            total_cay--;
            print_status();
            if(total_cay <= 0){
                K = rand() % (5*N) +1;
                total_cay += K;
                for(int i = 0; i<N; i++)
                    cay_counts [i] = 0 ;
                sleep (5);    
            }
            pthread_mutex_unlock(&tea_pot);
            state[index] = 3; // drinking
            sleep(1);
            sem_post(&cup_and_plate[index % N]);
            sem_post(&cup_and_plate[(index+ 1) % N]);
            state[index] = 0; // thinking
            sleep(1);
        }while(1);
    } else{ // tekse cayi sekerli iciyor
        do {
            state[index] = 1; // thirsty
            sem_wait(&cup_and_plate[(index+ 1) % N]);
            sem_wait(&cup_and_plate[index % N]);
            sem_wait(&sugar);
            pthread_mutex_lock(&tea_pot);            
            state[index] = 2; // pouring
            cay_counts [index] += 1;
            total_cay--;
            print_status();
            if(total_cay <= 0){
                K = rand() % (5*N) +1;
                total_cay += K;
                for(int i = 0; i<N; i++)
                    cay_counts [i] = 0 ;
                sleep (5);    
            }
            pthread_mutex_unlock(&tea_pot);
            state[index] = 3; // drinking
            sleep(1);
            sem_post(&cup_and_plate[(index+ 1) % N]);
            sem_post(&cup_and_plate[index % N]);  
            sem_post(&sugar);          
            state[index] = 0; // thinking
            sleep(1);
        }while(1);

    }
}

 
int main(int argc, char ** argv)
{

    N = atoi(argv[1]);
    total_cay = 2*N;
    K = 2* N;
    cup_and_plate = (sem_t*)(malloc(sizeof(sem_t)*N));
    state = (int*)(malloc(sizeof(int)*N));
    cay_counts = (int*)(malloc(sizeof(int)*N));
    pthread_t t_id[N];

    
    state_dict[0] = "THINKING  (";
    state_dict[1] = "THIRSTY   (";
    state_dict[2] = "POURING   (";
    state_dict[3] = "DRINKING  (";
    
    
    // do smthng
    sem_init(&sugar, 0, 1);
    pthread_mutex_init(&tea_pot, NULL);
   
    int num [N];
    for(int i=0;i<N;i++)
        num[i] = i;  

    for(int i=0; i < N; i++) {
        sem_init(&cup_and_plate[i], 0, 1);
        pthread_create(&t_id[i], NULL, drink_cay, &num[i]); 
    }

    f = 0;
    print_status();
    f=N+3;
    
    for(int i=0;i<N;i++)
        pthread_join(t_id[i], NULL);    
    
    free(cup_and_plate);
    free(state);
    free(cay_counts);

}
