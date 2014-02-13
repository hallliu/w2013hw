#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct state {
    char *name;
    struct state **transitionTo; //indices 0-25 represent a-z, 26 is *
} stateDat, *state;

char* remove_white_read(FILE *f) { //reads a line and returns it stripped of whitespace
    int outLen = 100;
    char *output = malloc(outLen*sizeof(char));
    char c; 
    int out_index=0;
    while((c = fgetc(f)) != EOF && c != '\n') {
        if(isgraph(c)) {
            if(out_index + 2 > outLen) {
                outLen *= 2;
                output = realloc(output, outLen*sizeof(char));
            }
            output[out_index] = c;
            ++out_index;
        }
    }
    output[out_index] = '\0';
    return output;
}

char **split(char *orig, char sep) { //this will fuck up if there is a trailing separator, but that doesn't happen here.
    int maxParts = 6;
    char **result = malloc(maxParts*sizeof(char *));

    int partInd = 0;
    int innerInd = 0;

    result[0] = malloc(strlen(orig)*sizeof(char)); //waste of space, yes, but insignificant here
    //idea is to iterate through and start a new string when we hit a separator
    for(int i=0; orig[i] != '\0'; ++i) {
        if(orig[i] == sep) {
            result[partInd][innerInd] = '\0';
            result[++partInd] = malloc(strlen(orig)*sizeof(char));
            innerInd = 0;
            continue;
        }
        result[partInd][innerInd++] = orig[i];
    }
    result[partInd][innerInd] = '\0';
    result[partInd+1] = "";
    return result;
}

state get_state_from_name(state *list_of_states, char *name) {
    for(int i=0;;++i) { //we're guaranteed that name will be somewhere in there, so this actually isn't infinite
        if(!strcmp(list_of_states[i] -> name, name))
            return list_of_states[i];
    }
}

//populates the transition array given a trans_str formatted like a:name,b:name,c:name
void make_transitions(int index, char *trans_str, state *list_of_states) {
    state current = list_of_states[index];
    char **transitions = split(trans_str,',');
    for(int i=0; transitions[i][0] != '\0'; ++i) { 
        //the pointer arithematic here drops the character and the colon, leaving the name of the new state.
        if(transitions[i][0] == '*') 
            current -> transitionTo[26] = get_state_from_name(list_of_states, transitions[i]+2);
        else
            current -> transitionTo[transitions[i][0] - 97] = get_state_from_name(list_of_states, transitions[i]+2);
    }
    return;
}

state transit(state s, char c) {
    //if(c == '*') return s -> transitionTo[26];
    if(s -> transitionTo[c - 97] == NULL) return s -> transitionTo[26];
    return s -> transitionTo[c - 97];
}

int main(int argc, char *argv[]) {
    FILE *fsmFile;
    char *line;
    char **names;
    int nStates = 0;
    char *start;
    char *end;
    state *list_of_states; 
    state curr_state;

    fsmFile = fopen(argv[1],"r");
    if(fsmFile == NULL) {
        printf("File not found\n");
        exit(-1);
    }
   
    //get the initial information into the program
    while(strcmp((line = remove_white_read(fsmFile)), "[transitions]")) {
        if(!strncmp(line,"names",5)) 
            names = split(line+6,',');
        else if(!strncmp(line,"start",5))
            start = line + 6;
        else if(!strncmp(line,"end",3))
            end = line + 4;
    }
    //there's a minor memory leak here with each line that's read in, but it's not really worth fixing.

    for(;strcmp(names[nStates],""); ++nStates); //count up the number of names
    ++nStates;

    list_of_states = malloc(nStates*sizeof(state)); //allocate space for pointers

    //make the structs now that we have the names
    for(int i=0; strcmp(names[i],""); ++i) { 
        list_of_states[i] = malloc(sizeof(stateDat));
        list_of_states[i] -> name = names[i];
        list_of_states[i] -> transitionTo = calloc(27,sizeof(state));
    }
   
    //read through the rest of it and get the transitions
    while(!feof(fsmFile)) {
        line = remove_white_read(fsmFile);
        for(int i=0; strcmp(names[i],""); ++i) {//check which name this one matches
            if(!strncmp(line,names[i],strlen(names[i]))) {
                make_transitions(i,line+strlen(names[i])+1, list_of_states); //make the struct with the given transits
                if(!strcmp(names[i],start))
                    curr_state = list_of_states[i]; //start here at the beginning of the run
            }
        }
    }
    
    //Finally, start reading from the second argument
    for(int i=0; argv[2][i] != '\0'; i++) {
        curr_state = transit(curr_state, argv[2][i]);
    }

    if(strcmp(curr_state -> name, end)) {
        printf("Reject. Ended in state %s\n", curr_state -> name);
    }
    else
        printf("Accept.\n");

    exit(0);
}
