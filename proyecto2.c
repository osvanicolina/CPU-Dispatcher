#define _GNU_SOURCE
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Required by for routine
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define DEBUG 1

int printer = 2;
int modem = 3;
int dvd_bluray = 2;
int webcam = 1;
int cornet = 2;


TAILQ_HEAD(tailhead, Proceso) head;
// Real time Queue
TAILQ_HEAD(tailheadrealtime, Proceso) head_real_time;
// Waiting for resources and ready queue priority three 
TAILQ_HEAD(tailheadresourcesthreee, Proceso) head_resources_three;
TAILQ_HEAD(tailheadreadythreee, Proceso) head_ready_three;

struct Proceso{
    int id;
    int time_arrive;
    int priority;
    int processor_time;
    int printer;
    int modem;
    int dvd_blueray;
    int webcam;
    int cornet;
    int counter;
    TAILQ_ENTRY(Proceso) entries;
};


void despachador();
void release_resources(struct Proceso *element);
int assign_resources_if_possible(struct Proceso *element);


void add_to_proccess_queue(int id, int time_arrive, int priority, int processor_time, int printer, int modem, int dvd_blueray, int webcam, int cornet){

    struct Proceso *new_element;
    new_element = malloc(sizeof(struct Proceso));
    
    if (new_element) {
        new_element->id = id;
        new_element->time_arrive = time_arrive;
        new_element->priority = priority;
        new_element->processor_time = processor_time;
        new_element->printer = printer;
        new_element->modem = modem;
        new_element->dvd_blueray = dvd_blueray;
        new_element->webcam = webcam;
        new_element->cornet = cornet;
        new_element->counter = 0;
    }
    TAILQ_INSERT_TAIL(&head, new_element, entries);
}


void add_to_real_time_queue(struct Proceso *element){
    struct Proceso *new_element;
    new_element = malloc(sizeof(struct Proceso));
    
    if (new_element) {
        new_element->id = element->id;
        new_element->time_arrive = element->time_arrive;
        new_element->priority = element->priority;
        new_element->processor_time = element->processor_time;
        new_element->printer = element->printer;
        new_element->modem = element->modem;
        new_element->dvd_blueray = element->dvd_blueray;
        new_element->webcam = element->webcam;
        new_element->cornet = element->cornet;
        new_element->counter = element->counter;
    }

    TAILQ_INSERT_TAIL(&head_real_time, new_element, entries);
}

void add_to_waiting_for_resources_three(struct Proceso *element){
    struct Proceso *new_element;
    new_element = malloc(sizeof(struct Proceso));
    
    if (new_element) {
        new_element->id = element->id;
        new_element->time_arrive = element->time_arrive;
        new_element->priority = element->priority;
        new_element->processor_time = element->processor_time;
        new_element->printer = element->printer;
        new_element->modem = element->modem;
        new_element->dvd_blueray = element->dvd_blueray;
        new_element->webcam = element->webcam;
        new_element->cornet = element->cornet;
        new_element->counter = element->counter;
    }

    TAILQ_INSERT_TAIL(&head_resources_three, new_element, entries);
}

void add_to_ready_three(struct Proceso *element){
    struct Proceso *new_element;
    new_element = malloc(sizeof(struct Proceso));
    
    if (new_element) {
        new_element->id = element->id;
        new_element->time_arrive = element->time_arrive;
        new_element->priority = element->priority;
        new_element->processor_time = element->processor_time;
        new_element->printer = element->printer;
        new_element->modem = element->modem;
        new_element->dvd_blueray = element->dvd_blueray;
        new_element->webcam = element->webcam;
        new_element->cornet = element->cornet;
        new_element->counter = element->counter;
    }

    TAILQ_INSERT_TAIL(&head_ready_three, new_element, entries);
}

int main(void)
{   
    int time_arrive, priority, processor_time, printer, modem, dvd_blueray, webcam, cornet;
    int i, id = 1;
    int number;
    FILE * fp;
    char * line = NULL;
    char *token;
    size_t len = 0;
    ssize_t read;
    struct Proceso *element;
    element = malloc(sizeof(struct Proceso));
    //Leemos el archivo línea por línea, luego llamaremos una función para 
    fp = fopen("archivo.txt", "r");
    if (fp == NULL){
        exit(EXIT_FAILURE);
    }

    TAILQ_INIT(&head);
    TAILQ_INIT(&head_real_time);
    TAILQ_INIT(&head_resources_three);
    TAILQ_INIT(&head_ready_three);

    while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line,",");
        i = 0;
        while(token != NULL){
            sscanf(token, "%d", &number);
            if(i == 0){
                time_arrive = number;
            }else if(i == 1){
                priority = number;
            }else if(i == 2){
                processor_time = number;
            }else if(i == 3){
                printer = number;
            }else if(i == 4){
                modem = number;
            }else if(i == 5){
                dvd_blueray = number;
            }else if(i == 6){
                webcam = number;
            }else if(i == 7){
                cornet = number;
            }
            i++;
            token = strtok(NULL,",");
        }
        add_to_proccess_queue(id,time_arrive, priority, processor_time, printer, modem, dvd_blueray, webcam, cornet);
        id++;
    }

    fclose(fp);
    despachador();
    
    if(DEBUG){
        while (head.tqh_first != NULL){
            element = head.tqh_first;
            printf("ID: %d - %d %d %d %d %d %d %d %d \n",element->id, element->time_arrive, element->priority, element->processor_time, element->printer, element->modem, element->dvd_blueray, element->webcam, element->cornet);
            TAILQ_REMOVE(&head, head.tqh_first, entries);
        }
    }
    
    
    
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int assign_resources_if_possible(struct Proceso *element){
    if(element->printer <= printer && element->modem <= modem && element->dvd_blueray <= dvd_bluray && element->webcam <= webcam && element->cornet <= cornet){
        printer = printer - element->printer;
        modem = modem - element->modem;
        dvd_bluray = dvd_bluray - element->dvd_blueray;
        webcam = webcam - element->webcam;
        cornet = cornet - element->cornet;
        return 1;
    }else{
        return 0;
    }
}

void release_resources(struct Proceso *element){
    printer = printer + element->printer;
    modem = modem + element->modem;
    dvd_bluray = dvd_bluray + element->dvd_blueray;
    webcam = webcam + element->webcam;
    cornet = cornet + element->cornet;
}

void despachador(){
    
    char *child_time;
    struct Proceso *element = malloc(sizeof(struct Proceso));;
    pid_t pID;
    int time_program = head.tqh_first->time_arrive;
    while(head.tqh_first != NULL){
        

        //While para tomar y asignar todos los procesos a su respectiva cola y asignar recursos en caso de que estén disponibles
        while(head.tqh_first != NULL && head.tqh_first->time_arrive <= time_program){
            if(head.tqh_first -> priority == 0){
                add_to_real_time_queue(head.tqh_first);
            }else if (head.tqh_first -> priority == 3){
                if(assign_resources_if_possible(head.tqh_first)){
                    add_to_ready_three(head.tqh_first);
                }else{
                    add_to_waiting_for_resources_three(head.tqh_first);
                }
            }
            TAILQ_REMOVE(&head, head.tqh_first, entries);
        }


        while( head_real_time.tqh_first != NULL){
            element = head_real_time.tqh_first;
            pID = fork();
            if(pID == 0){

                printf("Iniciando proceso ID: %d - %d %d %d %d %d %d %d %d \n",element->id, element->time_arrive, element->priority, element->processor_time, element->printer, element->modem, element->dvd_blueray, element->webcam, element->cornet);
                sprintf(child_time, "%d", element->processor_time);
                if(execlp("./child", "child", child_time, NULL) == -1){
                    perror("Error in exec;");
                }
                
                return;
            }else{
                wait(0);
                time_program += element -> processor_time;  
                TAILQ_REMOVE( &head_real_time, head_real_time.tqh_first, entries);
                printf("Terminado proceso con ID: %d \n", element->id);
                while(head.tqh_first != NULL && head.tqh_first->time_arrive < time_program){
                    element = head.tqh_first;
                    if(element -> priority == 0){
                        add_to_real_time_queue(element);
                    }else if(element -> priority == 3){
                        if(assign_resources_if_possible(element)){
                            add_to_ready_three(element);
                        }else{
                            add_to_waiting_for_resources_three(element);
                        }
                    }
                    TAILQ_REMOVE(&head, head.tqh_first, entries);
                }    
            }
        } // Fin del while de ejecutar procesos en tiempo real
        if(head_ready_three.tqh_first != NULL){
            element = head_ready_three.tqh_first;
            pID = fork();
            if(pID == 0){

                printf("Iniciando QUANTUM prioridad 3 del proceso ID: %d - %d %d %d %d %d %d %d %d \n",element->id, element->time_arrive, element->priority, element->processor_time, element->printer, element->modem, element->dvd_blueray, element->webcam, element->cornet);
                sprintf(child_time, "%d", 1);
                if(execlp("./child", "child", child_time, NULL) == -1){
                    perror("Error in exec;");
                }
                
                return;
            }else{
                wait(0);
                element->processor_time--;
                TAILQ_REMOVE(&head_ready_three, head_ready_three.tqh_first, entries);
                if(element->processor_time <= 0){
                    printf("Proceso con ID %d culminado.\n", element->id);
                    printf("hola");
                    release_resources(element);
                    if(head_resources_three.tqh_first != NULL){
                        for (element = head_resources_three.tqh_first; element != NULL && assign_resources_if_possible(element) == 0; element = element->entries.tqe_next){
                            continue;
                        }
                        if(element != NULL){
                            add_to_ready_three(element);
                            printf("ELEMENT ID: %d\n", element->id);
                            TAILQ_REMOVE(&head_resources_three, element, entries);
                        }
                    }
                    
                }else{
                    printf("QUANTUM de prioridad 3 terminado para proceso con id %d \n", element->id);
                    add_to_ready_three(element);
                } 
            }
        }
        printf("Time: %d \n", time_program);
        time_program++;
    }


    if(DEBUG){
        printf("Recursos: %d %d %d %d %d \n", printer, modem, dvd_bluray, webcam, cornet);
        while (head_ready_three.tqh_first != NULL){
            element = head_ready_three.tqh_first;
            printf("READY3 - ID: %d - %d %d %d %d %d %d %d %d \n",element->id, element->time_arrive, element->priority, element->processor_time, element->printer, element->modem, element->dvd_blueray, element->webcam, element->cornet);
            TAILQ_REMOVE(&head_ready_three, head_ready_three.tqh_first, entries);
        }
        while (head_resources_three.tqh_first != NULL){
            element = head_resources_three.tqh_first;
            printf("WAITING3 - ID: %d - %d %d %d %d %d %d %d %d \n",element->id, element->time_arrive, element->priority, element->processor_time, element->printer, element->modem, element->dvd_blueray, element->webcam, element->cornet);
            TAILQ_REMOVE(&head_resources_three, head_resources_three.tqh_first, entries);
        }
    }
}