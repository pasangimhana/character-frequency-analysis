#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>

int nprocs;
char *TEXT_DIR;
char **FILENAMES;
int NUM_FILES;
int SUBSET_SIZE;
int LAST_SUBSET_SIZE;
int BAR_LENGTH = 60;
int PADDING = 12;

/**
 * @brief Get the filenames of given directory
 *
 */
void get_filenames_of_dir()
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir(TEXT_DIR);
    if (directory == NULL)
    {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    int num_items = 0;

    while ((entry = readdir(directory)) != NULL)
    {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        num_items++;
    }

    NUM_FILES = num_items;

    // Allocate memory for the array of file names
    FILENAMES = malloc(num_items * sizeof(char *));

    // rewind the directory
    rewinddir(directory);

    int currentIndex = 0;
    // Traverse through each entry in the directory
    while ((entry = readdir(directory)) != NULL)
    {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // get size of the file name
        int size = strlen(entry->d_name);

        // Allocate memory for the file name
        FILENAMES[currentIndex] = (char *)malloc((size + 1) * sizeof(char));

        // check if allocation failed
        if (FILENAMES[currentIndex] == NULL)
        {
            perror("Unable to allocate memory for file name");
            exit(EXIT_FAILURE);
        }
        // copy the file name into the array
        strcpy(FILENAMES[currentIndex], entry->d_name);

        currentIndex++;
    }
    // Close the directory
    closedir(directory);
}
/**
 * @brief Function to calculate the subset sizes
 *
 */
void calculate_subset_sizes()
{
    SUBSET_SIZE = NUM_FILES <= nprocs ? 1 : (int)floor(NUM_FILES / nprocs);

    if (SUBSET_SIZE * nprocs < NUM_FILES)
    {
        LAST_SUBSET_SIZE = SUBSET_SIZE + NUM_FILES - (SUBSET_SIZE * nprocs);
    }
    else
    {
        LAST_SUBSET_SIZE = SUBSET_SIZE;
    }
}

int make_trivial_ring(int *fd)
{

    // create a pipe
    if (pipe(fd) == -1)
    {
        // if failed to create pipe, return -1
        return (-1);
    }

    return (0);
}

int add_new_node(int *pid, int *fd)
{

    // create a pipe
    if (pipe(fd) == -1)
        // if failed to create pipe, return -1
        return (-1);
    // try to fork the process
    *pid = fork();
    if ((*pid) == -1)
        // if failed to fork, return -2
        return (-2);

    return (0);
}

/**
 * @brief Function to process the file counts
 *
 * @param inFile input file name
 * @param char_stats character count array to store the counts
 * @return long
 */
long procFileCounts(char inFile[], long char_stats[])
{
    char cur_fname[FILENAME_MAX];
    strcpy(cur_fname, TEXT_DIR);
    strcat(cur_fname, inFile);
    FILE *file = fopen(cur_fname, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(1);
    }
    char c;
    long char_count = 0;
    /* for as long as we can get characters... */
    while ((c = fgetc(file)))
    {
        /* break if end of file */
        if (c == EOF)
        {
            break;
        }
        else if ((tolower(c) - 'a') >= 0 && (tolower(c) - 'a') < 26)
        {
            char_stats[tolower(c) - 'a'] += 1;
        }
        else
        {
            continue;
        }
        /* otherwise add one to the count of that particular character */
        char_count += 1;
    }
    return char_count;
}

/**
 * @brief Function to analyze the subset of files
 *
 * @param start_index start index of the subset
 * @param end_index end index of the subset (exclusive)
 * @param char_stats character count array
 */
void analyze_subset(int start_index, int end_index, int char_stats[])
{
    for (int i = start_index; i < end_index; i++)
    {
        long file_char_stats[26] = {0}; // Array to store character counts for each file

        // Perform analysis for each file in the subset
        long char_count = procFileCounts(FILENAMES[i], file_char_stats);

        // Update the overall character count array (char_stats) using the file_char_stats
        for (int j = 0; j < 26; j++)
        {
            char_stats[j] += file_char_stats[j];
        }
    }
}

/**
 * @brief Function to parse the command line arguments
 *
 * @param argc argument count
 * @param arg argument array
 * @param nprocs pointer to the variable to store the number of processes
 * @return int
 */
int parse_args(int argc, char *arg[], int *nprocs)
{
    // if arg count is not 2, (doesn't have file path and number of processes in the cl arguments)
    if (argc != 3)
    {
        //  show valid usage
        printf("Invalid number of arguments, usage: text-file-analyzer <no_of_processes> <sub_directory_path> \n");
        //  return -1 to indicate error
        return -1;
    }

    // save the number of processes in the nprocs variable
    *nprocs = atoi(arg[1]);
    // save the text directory path in the TEXT_DIR variable
    TEXT_DIR = arg[2];

    // return 0 to indicate success
    return 0;
}

/**
 * @brief Function to print the character statistics as a bar chart
 *
 * @param char_stats
 */
void star_visualization(int *char_stats)
{

    int max_char_count = 0;
    for (int i = 0; i < 26; i++)
    {
        if (char_stats[i] > max_char_count)
        {
            max_char_count = char_stats[i];
        }
    }

    printf("\n\n");

    for (int i = 0; i < 26; i++)
    {
        int num_of_digits = floor(log10(char_stats[i])) + 1;
        printf("Got char %c: %d", 'a' + i, char_stats[i]);
        for (int j = 0; j < PADDING - num_of_digits; j++)
        {
            printf(" ");
        }

        printf("| ");

        int num_stars = (int)floor((float)char_stats[i] / (float)max_char_count * BAR_LENGTH);

        for (int j = 0; j < num_stars; j++)
        {
            printf("*");
        }
        printf("\n");
    }

    printf("\n\n");
}

int main(int argc, char *argv[])
{
    int i;                    /* number of this process (starting with 1) */
    int childpid;             /* indicates process should spawn another */
    int char_stats[26] = {0}; /* array to store the character counts */

    if (parse_args(argc, argv, &nprocs) < 0)
        exit(EXIT_FAILURE);

    get_filenames_of_dir();

    // if nprocs is greater than the num of files, set nprocs to num of files
    if (nprocs > NUM_FILES)
    {
        printf("Too much processes (%d) to analyse %d files, setting to %d\n", nprocs, NUM_FILES, NUM_FILES);
        nprocs = NUM_FILES;
    }

    int pipes[nprocs][2];
    int main_pid = getpid();

    // a flag to be notified when the last process is finished processing the text files
    int finished[2];
    if (pipe(finished) == -1)
    {
        perror("Could not create pipe");
        exit(EXIT_FAILURE);
    }

    calculate_subset_sizes();

    if (make_trivial_ring(pipes[0]) < 0)
    {
        perror("Could not make trivial ring");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < nprocs; i++)
    {
        if (add_new_node(&childpid, pipes[i]) < 0)
        {
            perror("Could not add new node to ring");
            exit(EXIT_FAILURE);
        };
        if (childpid)
            break;
    }

    // calculate the correct start index and end index for the subset to be analysed by this process
    int start_index = SUBSET_SIZE * (i - 1);
    int end_index = i == nprocs ? start_index + LAST_SUBSET_SIZE : start_index + SUBSET_SIZE;

    if (i == 1)
    {

        // analyze the first subset
        analyze_subset(0, SUBSET_SIZE, char_stats);

        write(pipes[0][1], char_stats, sizeof(char_stats));
        close(pipes[0][1]);

        if (nprocs == 1)
        {
            // if there's only a single process, set finished flag to indicate that the analysis is finished
            close(finished[0]);
            close(finished[1]);
        }
    }
    else if (i < nprocs)
    {
        // for intermediate processes when there is more than 2
        if (i > 2)
        {
            // wait for the parent process to finish, unless it's the first child process
            // (because then main process will be finshed and that shouldn't happen)
            waitpid(getppid(), NULL, 0);
        }

        // close the write end of the previous pipe
        close(pipes[i - 2][1]);
        // close the read end of the current pipe
        close(pipes[i - 1][0]);

        int intermediate_char_stats[26] = {0};

        // read the character counts from the previous pipe
        read(pipes[i - 2][0], intermediate_char_stats, sizeof(intermediate_char_stats));
        // close the read end of the previous pipe
        close(pipes[i - 2][0]);

        // analyze the correct subset
        analyze_subset(start_index, end_index, intermediate_char_stats);

        // write to this process's pipe
        write(pipes[i - 1][1], intermediate_char_stats, sizeof(intermediate_char_stats));
        // close the write end of this process's pipe
        close(pipes[i - 1][1]);
    }
    else
    {
        if (i > 3)
        {
            // wait for the previous process to finish
            waitpid(getppid(), NULL, 0);
            // close the write end of the previous pipe
            close(pipes[i - 2][1]);
        }

        // close the read end of this process's pipe
        close(pipes[i - 1][0]);

        // array to hold the intermediate character statistics
        int intermediate_char_stats[26] = {0};

        // read the character counts from the previous pipe
        read(pipes[i - 2][0], intermediate_char_stats, sizeof(intermediate_char_stats));
        // close the read end of the previous pipe
        close(pipes[i - 2][0]);

        // analyze the subset
        analyze_subset(start_index, end_index, intermediate_char_stats);

        // close the read end of final pipe
        close(pipes[0][0]);
        // write to the final pipe
        write(pipes[0][1], intermediate_char_stats, sizeof(intermediate_char_stats));
        // close the write end of the final pipe
        close(pipes[0][1]);

        // close the read end of finished pipe
        close(finished[0]);
        // write 1 to the finished pipe
        int flag = 1;
        write(finished[1], &flag, sizeof(int));
        // close the write end of the finished pipe
        close(finished[1]);
    }

    if (getpid() == main_pid)
    {

        // to block the process until the finished flag is set
        close(finished[1]);
        int flag;
        read(finished[0], &flag, sizeof(int));
        close(finished[0]);

        // get the final character statistics from the first pipe
        int final_stats[26] = {0};
        read(pipes[0][0], final_stats, sizeof(final_stats));
        close(pipes[0][0]);
        close(pipes[0][1]);

        star_visualization(final_stats);
    }

    exit(EXIT_SUCCESS);
}