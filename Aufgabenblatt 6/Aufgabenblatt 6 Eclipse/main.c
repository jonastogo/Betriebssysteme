#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "queue.h"
#include "miniz.h"

int readFolderIntoJobList(int argc, char** argv);
void addJob(char *fullFilePath);
void *comprThread(void* arg);
int comprFile(char *pPath, int ptID);
int EndsWith(const char *str, const char *suffix);

typedef struct job
{
    const char *path;
} Job;

#define maxThreads 2
#define doSleep 0

pthread_mutex_t mutexQueue;
static Queue jobQueue;
int jobCounter = 0;

static pthread_t threadList[maxThreads];

/*
*   Argument 1 ist der relative oder absolute Pfad zum Ordner.
*/
int main(int argc, char** argv)
{
    time_t start_t, end_t;
    double diff_t;

    jobQueue = queue_create();

    // Mindestes 2 Argumente werden benoetigt
    if(argc >= 2){
        // Laufzeitmessung beginnen
        time(&start_t);

        pthread_mutex_lock (&mutexQueue);
        readFolderIntoJobList(argc, argv);
        pthread_mutex_unlock (&mutexQueue);

        for (int i = 0 ; i < maxThreads ; i++ ){
            int rc = pthread_create(&threadList[i], NULL, &comprThread, i);

            if( rc != 0 ) {
                printf("Cannot create thread #%d\n",i);
                return EXIT_FAILURE;
            }
        }

        // Wartet auf alle Threads, die gestartet wurden
        printf("%d threads are working on %d jobs...\n", maxThreads, jobCounter);
        for (int i = 0 ; i < maxThreads ; i++ ) {
            pthread_join( threadList[i], NULL );
        }

        // Berechnet Laufzeit
        time(&end_t);
        diff_t = difftime(end_t, start_t);
        printf("Execution time = %f\n", diff_t);
    }

    // Mutex Objekt zerstoeren
    pthread_mutex_destroy(&mutexQueue);

    return EXIT_SUCCESS;
}

/*
*   Liest alle Dateien im angegebenen Ordner aus.
*/
int readFolderIntoJobList(int argc, char** argv){
    struct dirent *dp;
    DIR *dfd;

    char *dir;
    dir = argv[1];

    if ((dfd = opendir(dir)) == NULL) {
        printf("Cannot open Folder '%s'\n");
        return EXIT_FAILURE;
    }

    char filename[256];

    while ((dp = readdir(dfd)) != NULL) {
        struct stat stbuf;
        sprintf( filename , "%s/%s",dir,dp->d_name);

        if( stat(filename,&stbuf ) == -1 ) {
            printf("Unable to stat file: %s\n",filename);
            continue;
        }

        if ( ( stbuf.st_mode & S_IFMT ) == S_IFDIR ) {
            // Skip directories
            continue;
        } else if (!EndsWith(filename, ".compr")) {
            addJob(filename);
        }
    }

    if(doSleep){
        sleep(1);
    }
    return 0;
}

/*
*   Fuegt die angegebene Datei als Job der Queue hinzu.
*/
void addJob(char *fullFilePath){

    Job *newJob = (Job *) malloc(sizeof(Job));
    char *path = (char *) malloc(sizeof(char) * 265);

    if(newJob || path){
        strcpy(path,fullFilePath);

        newJob->path = path;
        queue_insert(jobQueue, newJob);

        jobCounter++;
    }
}

/*
*   Holt sich einen Job aus der Queue, erstellt sich,
*   eine lokale Kopie und entfernt den Job aus der Queue.
*/
void *comprThread(void* arg){
    int threadID = arg;
    printf("Thread with ID %d started.\n", threadID);

    struct RetValues *ret = NULL;

    while (!queue_empty(jobQueue)) {

        pthread_mutex_lock (&mutexQueue);
        Job *head = queue_head(jobQueue);
        Job current;

        memcpy(&current,head, sizeof(Job));

        queue_delete(jobQueue);

        pthread_mutex_unlock (&mutexQueue);

        int result = comprFile(current.path, threadID);

        // Jobdaten wieder freigeben
        free(head->path);
        free(head);

    }

    printf("Thread with ID %d finished.\n", threadID);
    pthread_exit((void *)ret);
}

/*
*   Liest die angegebene Datei ein, komprimiert diese
*   und schreibt das Ergebnis in eine .compr Datei.
*/
int comprFile(char *pPath, int ptID){

    // Read File
    FILE *f = fopen(pPath, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

    char *buffer = malloc(fsize + 1);
    fread(buffer, fsize, 1, f);
    fclose(f);

    // Compress Content
    Result *res = compress_string(buffer);
    printf("ID: %d\nCompress: %s\nLen: %lu\nContent: '%s'\n", ptID, pPath, res->length, res->data);

    // Write File
    char filepath[256];
    memset(&filepath[0], 0, sizeof(filepath));
    strcat(filepath,pPath);
    strcat(filepath,".compr");

    FILE *file = fopen(filepath, "w");

    int results = fputs(res->data, file);
    if (results == EOF) {
        return 1;
    }
    fclose(file);

    // Komprimierungsergebnis wieder freigeben
    free(res->data);
    free(res);

    if(doSleep){
        sleep(3);
    }

    return 0;
}

/*
*   Ueberprueft, ob der uebergebene String mit dem
*   angegebenen Suffix endet.
*   Quelle: http://stackoverflow.com/a/744822
*/
int EndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}
