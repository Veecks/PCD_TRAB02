#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
using namespace std;
#define maxThread 4 // thread servidor + n threads clientes
long int critical_Region = 0;
int request = 0, respond = 0;
int end_sum = 0;
pthread_barrier_t barrier;
void *clientAcess(void *arg)
{
    int aux = *(int *)(arg)+1;
    int ctrl = 0;
    int auxSoma  =0;
    cout << "sou a thread: " << aux << endl;
    while (ctrl <= 1000000)
    {
        
        while (respond != aux)
        {
            request = aux;
        }
        
        
        // usleep(500);
        // critical region
        auxSoma=critical_Region;
        auxSoma++;
        critical_Region = auxSoma;

        // sleep(1);
        // pos protocol
        respond = 0;
        ctrl++;
    }
    pthread_barrier_wait(&barrier);
 
    if (aux == 1)end_sum = 1;
    
    free(arg);
    cout << "Thread: " << aux << " acabou "<<endl;
    return NULL;
}

void *serverProvider(void *arg)
{

    while (1)
    {
        // usleep(500);
       if (end_sum==1) break;
       while(request==0){ // request nula, ninguem recebeu
           // if(t1==1 || t2==1)break;
           if (end_sum==1) break;
            //printf("preso aqui %d %d\n", t1,t2);
        }
        respond = request;
        while(respond!=0){
            //if(t1==1 || t2==1)break;
            if (end_sum==1) break;
        }
        request=0;
        
    }

    return NULL;
}

int main()
{
    pthread_t vThread[maxThread];
    pthread_barrier_init(&barrier, NULL, maxThread-1);
    for (int i = 0; i < maxThread; i++)
    {
        int *index;
        index = (int *)malloc(sizeof(int) * 1);
        *index = i;
        if (i == maxThread-1)
        {
            printf("valor de i %d\n", i);
            pthread_create(&vThread[i], NULL, serverProvider, index);
        }
        else
        {
            printf("valor de i %d\n", i);
            pthread_create(&vThread[i], NULL, clientAcess, index);
        }
    }
    for (int i = 0; i < maxThread; i++)
    {
        pthread_join(vThread[i], NULL);
    }
    printf("valor final %ld\n", critical_Region-maxThread+1);

    cout << "teste" << endl;
}