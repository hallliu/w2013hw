#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

int to_diff(char *diff) {
    switch(diff[0]) {
        case 'e':
            return 8;
        case 'm':
            return 6;
        case 'h':
            return 4;
        case 'u':
            return INT_MAX;
        default:
            printf("Invalid difficulty, %s\n", diff);
            exit(0);
    }
    return 0;
}

void game_loop(int guesses, unsigned int number) {
    int usr_guess;
    for(int i=guesses; i>0; i--) {
        printf("Enter your guess: ");
        scanf("%d", &usr_guess);
        if(usr_guess > number) {
            printf("Guess too high\n");
        }
        else if(usr_guess < number) {
            printf("Guess too low\n");
        }
        else {
            printf("Guess correct\n");
            return;
        }
    }
    printf("Out of guesses\n");
    return;
}

int main(int argc, char *argv[]) {
    int range = 100; //default range
    int difficulty = INT_MAX; //default difficulty unlimited
    int guesses;
    char c;
    //randomness
    unsigned int number;
    FILE *random;

    random = fopen("/dev/urandom", "r"); //open /dev/urandom
    fread(&number, sizeof(unsigned int), 1, random); //read a int of arbitrary size

    while((c = getopt(argc, argv, "n:d:")) != -1) {
        switch(c) {
            case 'n':
                range = strtol(optarg, NULL, 10);
                break;
            case 'd':
                difficulty = to_diff(optarg);
                break;
            case '?':
                printf("Argument error\n");
                return 255;
            default:
                abort();
        }
    }
    guesses = (int) ceil(0.1*difficulty*log(range)/log(2));
    number = number % range + 1; //determine the number to be guessed
    game_loop(guesses, number);
    return 0;
}
