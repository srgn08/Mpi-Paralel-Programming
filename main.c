#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>


int main(int argc, char **argv) {

    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int count = 0;

    if (my_rank == 0){

        char fileChars[5000];

        FILE *file = fopen("encodedfile.txt", "r");
        int c;

        while ((c = fgetc(file)) != EOF) {
            fileChars[count] = (char) c;
            count++;
        }

        fileChars[count] = '\0';

        fclose(file);

        int allLocalBuffer = count / comm_sz;

        int i;
        int memCounter = 0;
        for (i = 0; i < comm_sz; i++){
            char local_lines[count / comm_sz + 5];
            strncpy(local_lines,fileChars + memCounter, (size_t) allLocalBuffer);
            memCounter+= allLocalBuffer;
            MPI_Send(local_lines,allLocalBuffer,MPI_CHAR,i,0,MPI_COMM_WORLD);
        }

    }

    MPI_Bcast(&count,1,MPI_INT,0,MPI_COMM_WORLD);

    int allLocalBuffer = count / comm_sz;
    char local_line[allLocalBuffer + 5];
    MPI_Recv(local_line,allLocalBuffer,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    char myMessage[10];
    int i,len = strlen(local_line);
    int counter = 0;
    for (i = 0; i < len; i++){
        if (local_line[i] == ','){
            myMessage[counter] = local_line[i+1];
            counter++;
        }
    }
    myMessage[counter] = '\0';

    char allMessage[counter * (comm_sz + 1)];

    MPI_Allgather(myMessage,counter,MPI_CHAR,allMessage,counter,MPI_CHAR,MPI_COMM_WORLD);


    printf("\nMy rank is: %d,    All Message is: %s\n",my_rank,allMessage);

    MPI_Finalize();

    return 0;
}