/*
    Author: Lerynnx (GitHub)
    Do not remove this Attribution if you use this code.
    Nottify the author if you want to use this code before using it.
*/

/*Slaves*/
#include <stdio.h>
#include <stdlib.h>
#include <pvm3.h>

int main() {
    //Declare variables
    int ptid, bufid, size, min, i, tid;

    //Enroll in PVM with the master
    ptid = pvm_parent();

    //Receive the message
    bufid = pvm_recv(ptid, -1);

    //Unpack the size
    pvm_bufinfo(bufid, (int*)0, (int*)0, &tid);
    pvm_upkint(&size, 1, 0);

    //Declare array
    int sArray[size];

    //Unpack the array
    pvm_upkint(sArray, size, 1);

    // Open a file in write mode
    FILE *file = fopen("/home/user/pvm3/bin/LINUX/log.txt", "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening log file");
        exit(1);
    }

    // Write the array elements to the file
    fprintf(file, "Array values\n");
    for (i = 0; i < size; i++) {
        fprintf(file, "%d ", sArray[i]);
    }
    
    fprintf(file, "\nArray size: %d ", size);

    //Get the minimum value
    min = sArray[0];
    for (i = 1; i < size; i++) {
        if (sArray[i] < min) {
            min = sArray[i];
        }
    }
	
    fprintf(file, "\nMinimum value: %d", min);

    // Close the file
    fclose(file);

    //Send the data to the master
    //Initialize a buffer to send data to the first slave
    pvm_initsend(PvmDataDefault);
    //Pack data and send to the master
    pvm_pkint(&min, 1, 0);
    pvm_send(ptid, 1);

    pvm_exit();
    exit(0);
}
