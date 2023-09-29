#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <papi.h>

#define MAX_FILES 100
#define NUM_RUNS 10

bool L1Misses, cycle, instruction;
int retval, eventset = PAPI_NULL;

// Function to read the TSC (Time Stamp Counter)
unsigned long long rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

void papiEvents (){
	
	retval=PAPI_create_eventset(&eventset);
	if (retval != PAPI_OK) {
	   fprintf(stderr,"Error creating eventset! %s\n",
		PAPI_strerror(retval));
	}

	if (L1Misses){
	retval=PAPI_add_named_event(eventset,"PAPI_L1_DCM");
	if (retval!=PAPI_OK) {
		fprintf(stderr,"Error adding PAPI_L1_DCM: %s\n",
		PAPI_strerror(retval));
	}
	else {
		printf("\n PAPI_L1_DCM ADDED OKAY\n");
	}
	}

	if (instruction){
	retval=PAPI_add_named_event(eventset,"PAPI_TOT_INS");
	if (retval!=PAPI_OK) {
		fprintf(stderr,"Error adding PAPI_TOT_INS: %s\n",
		PAPI_strerror(retval));
	}
	}
}


void compileAndRunWithTiming(char *filename, int numLoops) {
    char compileCommand[256];
    char executeCommand[256];


    int i;
    int totalTSC = 0;

    long_long values[3] = {(long_long) 0, (long_long) 0, (long_long) 0};
	int long long totalCycles = 0;
	int long long totalInstructions = 0;
    int long long totalMisses = 0;

    // Compile the C code
    snprintf(compileCommand, sizeof(compileCommand), "gcc -g -Wall -I /home/zemor/papi-7.0.1/src %s /home/zemor/papi-7.0.1/src/libpapi.a -o temp", filename);
    if (system(compileCommand) != 0) {
        printf("Compilation failed.\n");
        return;
    }

    // Measure execution time
    unsigned long long start_cycles, end_cycles;

    for(i=0;i<NUM_RUNS;i++){
        //Start PAPI counting
        
        PAPI_reset(eventset);
        retval=PAPI_start(eventset);
        if (retval!=PAPI_OK) {
            fprintf(stderr,"Error starting COUNTING: %s\n",
            PAPI_strerror(retval));
        } else {
            //printf("\n PAPI RESET OKAY\n");
        } 
        start_cycles = rdtsc();

        // Execute the compiled code with the provided number of loops as an argument
        snprintf(executeCommand, sizeof(executeCommand), "./temp %d", numLoops);
        if (system(executeCommand) != 0) {
            printf("Execution failed.\n");
            return;
        }

        end_cycles = rdtsc();

        //Stop PAPI counting dont forget to disable
        retval=PAPI_stop(eventset,values);
        if (retval!=PAPI_OK) {
            printf("ARDEU ARDEU ARDEU");
            fprintf(stderr,"Error stopping:  %s\n",
            PAPI_strerror(retval));
        }
        else {
            //printf("HHELLO HELLO");
            if (L1Misses && !instruction){
            //printf("Measured %lld LOADS | ",values[0]);
            totalMisses += values[0];
            }

            if (L1Misses && instruction){
            //printf("Measured %lld STORES | ",values[1]);
            totalMisses += values[0];
            totalInstructions += values[1];
            }
        }

        

        // Calculate and print the number of clock cycles
        totalTSC += end_cycles - start_cycles;
        printf("Clock cycles: %llu\n", end_cycles - start_cycles);

    }

    if (L1Misses && !instruction){
	printf("TOTAL L1 MISSES: %lld | ", totalMisses);
	printf("L1 MISSES PER RUN (%d iterations) AVERAGE: %.6f \n", numLoops, (float)totalMisses/(NUM_RUNS));
	}

	if (L1Misses && instruction){
    printf("TOTAL L1 MISSES: %lld | ", totalMisses);
	printf("L1 MISSES PER RUN (%d iterations) AVERAGE: %.6f \n", numLoops, (float)totalMisses/(NUM_RUNS));
	printf("TOTAL INSTRUCTIONS: %lld | ", totalInstructions);
	printf("INSTRUCTIONS PER RUN (%d iterations) AVERAGE: %.6f \n", numLoops, (float)totalInstructions/(NUM_RUNS));
	}

    printf("TOTAL TSC CYCLES: %d | ", totalTSC);
	printf("TSC CYCLES PER RUN (%d iterations) AVERAGE: %.6f \n", numLoops, (float)totalTSC/(NUM_RUNS));

    // Clean up the temporary executable
    system("rm temp");
}

int main() {
    char filenames[MAX_FILES][256];
    int numFiles = 0;

    // Automatically scan and list C code files in the current directory
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (numFiles < MAX_FILES && strstr(entry->d_name, ".c") != NULL && strcmp(entry->d_name, "run.c") != 0) {
            strncpy(filenames[numFiles], entry->d_name, sizeof(filenames[numFiles]));
            numFiles++;
        }
    }

    closedir(dir);

    retval=PAPI_library_init(PAPI_VER_CURRENT);
        if (retval!=PAPI_VER_CURRENT) {
                fprintf(stderr,"Error initializing PAPI! %s\n",
                        PAPI_strerror(retval));
                return 0;
        }

    L1Misses = true;
    cycle = true;
    instruction = true;

    papiEvents ();

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. List available C code files\n");
        printf("2. Run C code\n");
        printf("3. Quit\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (numFiles == 0) {
                    printf("No C code files available in the directory.\n");
                } else {
                    printf("-----------------------------------\n");
                    printf("Available C code files:\n");
                    for (int i = 0; i < numFiles; i++) {
                        printf("%d. %s\n", i + 1, filenames[i]);
                    }
                    printf("-----------------------------------");
                }
                break;
            case 2:
                if (numFiles == 0) {
                    printf("No C code files available. Please add some files.\n");
                } else {
                    printf("Enter the number corresponding to the C code file to run (or '0' to cancel): ");
                    int fileNumber;
                    scanf("%d", &fileNumber);

                    if (fileNumber >= 1 && fileNumber <= numFiles) {
                        printf("Enter the number of loops for the selected file: ");
                        int numLoops;
                        scanf("%d", &numLoops);

                        // Pass the number of loops to the selected file
                        compileAndRunWithTiming(filenames[fileNumber - 1], numLoops);
                    } else if (fileNumber != 0) {
                        printf("Invalid file number. Please enter a valid option.\n");
                    }
                }
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
        
        // Clear the input buffer
        while (getchar() != '\n');
    }

    return 0;
}


