/*
    Author: Lerynnx (GitHub)
    Do not remove this Attribution if you use this code.
    Nottify the author if you want to use this code before using it.
*/

/*Master*/
#include <stdio.h>
#include <stdlib.h>
#include <pvm3.h>

int main() {
	//Declare variables
	int tid, tid1, tid2, min1, min2, escl1, escl2, i, halfSize, size = 1;

	//Get the size of the array
	printf("Enter the size (even) of the array: ");
	scanf("%d", &size);

	//Declare variables
	halfSize = size/2;
	int array[size], subArray1[halfSize], subArray2[halfSize];

	//Get the values of the array
	printf("Enter the numbers of the array:\n");
	for(i=0;i<size;i++){
		scanf("%d", &array[i]);
	}

	//Enroll in PVM
	tid = pvm_mytid();
	printf("The master's id is %d\n", tid);

	//Create slave processes
	escl1 = pvm_spawn("slave", NULL, 0, "", 1, &tid1);
	escl2 = pvm_spawn("slave", NULL, 0, "", 1, &tid2);

	//Check if the slaves were started
	if (escl1!=1 || escl2!=1 ) {
		printf("Failed to start one or more slave processes.\n");
		pvm_exit();
		exit(1);
	}

	//Divide the array into two subarrays
	for(i = 0; i < halfSize; i++){
		subArray1[i] = array[i];
		subArray2[i] = array[i+halfSize];
	}

	//Send data to the slaves
	//Initialize a buffer to send data to the first slave
	pvm_initsend(PvmDataDefault);
	//Pack data and send to the first slave
	pvm_pkint(&halfSize,1,0);
	pvm_pkint(subArray1,halfSize,1);
	pvm_send(tid1, 1);

	//Initialize a buffer to send data to the second slave
	pvm_initsend(PvmDataDefault);
	//Pack data and send to the second slave
	pvm_pkint(&halfSize,1,0);
	pvm_pkint(subArray2,halfSize,1);
	pvm_send(tid2, 1);

	//Wait for the responses from the slaves
	min1 = pvm_recv(-1, -1);
	//Unpack the first response
	pvm_upkint(&min1,1,0);

	min2 = pvm_recv(-1, -1);
	//Unpack the second response
	pvm_upkint(&min2,1,0);

	//Calculate the final result
	printf("The result is: %d\n", min1 < min2 ? min1 : min2);

	pvm_exit();
	exit(0);
}
