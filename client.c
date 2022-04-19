#define	_GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"
#include "haiku.h"
#include "queue.h"
#include <sys/shm.h>

// clever macro that if defined lets running t_main as main function of file
// if macro is not defined, prevents conflicts of "main"s
#ifdef STANDALONE
#define t_main main
#endif

void error(const char *msg){
    perror(msg);
    exit(2);
}

void client_v1(pid_t t){
    sleep(1);
    printf("Hello from client v1!\n");
    int c;
    for(int i = 0; i < 100; i++){
        usleep(10*1000);
        c = rand()%2;
        if(c) {
            printf("#%d. Sending SIGINT to %d(%d)\n", i, t, c);
            kill(t, SIGINT);
        }
        else {
            printf("#%d. Sending SIGQUIT to %d(%d)\n", i, t, c);
            kill(t, SIGQUIT);
        }
    }
}

void client_v2(){
    sleep(1);
    printf("Hello from client v2!\n");
    haiku h;
    category c;
    for(int i = 0; i < 6; i++){
        printf("#%d. Reading book category ", i);
        if(i%2) {
            c = western;
            puts("western.");
        }
        else {
            c = japanese;
            puts("japanese.");
        }
        if(read_haiku(c, &h) == -1) exit(2);
        print_haiku(h);
    }
}

int t_main(){
    //access shared memory
    key_t k = ftok("/dev/null", '1');
    if(k == -1) error("ftok");
    int id;
    //waiting for server to write data
    for(int i = 0; i < 5; i++){
        id = shmget(k,  sizeof(pid_t), 0644);
        if(id != -1) break;
        else{
            fprintf(stderr, "Failed accessing shared memory. Server may not opened it yet. Attempt %d out of 5. Trying again in 2 seconds...\n", i+1);
            sleep(2);
        }
    }
    id = shmget(k,  sizeof(pid_t), 0644);
    if(id == -1) error("shmget");

    // reading server process id
    pid_t server;
    int *s = shmat(id, NULL, 0);
    server = *s;
    printf("Server process id: %d\n", server);
    client_v1(server);
}

