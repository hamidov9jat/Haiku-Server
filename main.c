#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "server.h"
#include "client.h"
#include "haiku.h"

// function that runs both server and client of the first version
void v1(){
    puts("Running V1");
    pid_t proc = fork();
    printf("fork res: %d\n", proc);
    if(proc > 0){
        // father
        client_v1(proc);
    } else if(proc == 0){
        // son
        server_v1();
    } else { // proc < 0
        // error_s
        perror("fork");
        exit(2);
    }
}

// function that turns server v2 into appropriate form for threads
void *server_v2_wrapper(void *arg){
    server_v2();
    pthread_exit(NULL);
}

// function that turns client v2 into appropriate form for threads
void *client_v2_wrapper(void *arg){
    client_v2();
    pthread_exit(NULL);
}

// function that creates two threads: one for client, other for server
void v2(){
    puts("Running V2");
    pthread_t s,c;
    if(pthread_create(&s, NULL, server_v2_wrapper, NULL) != 0) {
        perror("pthread_create");
        exit(2);
    }
    if(pthread_create(&c, NULL, client_v2_wrapper, NULL) != 0) {
        perror("pthread_create");
        exit(2);
    }
    pthread_join(s,NULL);
    pthread_join(c, NULL);
}

// main function allows running any version of haiku project depending on argument passed
int main(int argc, char **argv) {
    srand(time(NULL));
    printf("Haiku project\n");
    assert(argc > 1);
    for(int i = 1; i < argc; i++){
        printf("%d\n", i);
        switch (argv[i][0]) {
            case '1':
                v1();
                break;
            case '2':
                v2();
                break;
            default:
                puts("Not implemented.");
                break;
        }
        puts("");
        sleep(1);
    }
    getchar();
    return 0;
}
