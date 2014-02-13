#include <stdio.h>
#include <stdlib.h>

//getValue: a function to retrieve values from the provided array in 
//accordance with the lab assignment. In cases where the value was not 
//user-defined, no changes are made to the int referenced by value.
int getValue(int *array, int numElements, int pos, int *value) {
    if(pos + 1 > numElements) {
        return 2;
    }
    else if(array[pos] < 0) {
        return 1;
    }
    else {
        *value = array[pos];
        return 0;
    }
}

int main(int argc, char *argv[]) {
    int *array = NULL; //This pointer is initialized to NULL in order to make realloc behave like malloc the first time it's called
    int arraySize = 0;
    int value;
    int position;
    char cont; 

    do {
        printf("Enter a position: ");
        scanf("%d", &position);
        printf("Enter a value: ");
        scanf("%d", &value);

        //If the specified position fits in the current array, change the 
        //value to the specified one. If not, resize the array using realloc
        //and fill in the expanded space with -1s to indicate that they 
        //haven't been assigned.
        if(position + 1 <= arraySize) {
            array[position] = value;
        }
        else {
            array = realloc(array, (position + 1)*sizeof(int));
            for(int i = arraySize; i < position; i++) {
                array[i] = -1;
            }
            array[position] = value;
            arraySize = position + 1;
        }

        printf("Enter another assignment? ");
        getchar(); //this clears the newline from the input buffer
        scanf("%c", &cont);
    } while(cont == 'y');

    for(int i=0; i < arraySize; i++) {
        //Uses the fact that unassigned positions contain -1 to decide which
        //positions are assigned.
        if(array[i] < 0) {
            printf("array[%d] = [No value assigned]\n", i); 
        }
        else {
            printf("array[%d] = %d\n", i, array[i]);
        }
    }

    printf("Specify a position to access: ");
    scanf("%d", &position);
    //Reusing the position/value variables because there's no further need
    //for them
    switch(getValue(array, arraySize, position, &value)) {
        case 0:
            printf("array[%d] is %d\n", position, value);
            break;
        case 1: 
            printf("No value at that position\n");
            break;
        case 2:
            printf("Not a valid position\n");
            break;
    }
    exit(0);
}
