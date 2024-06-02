#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define PATH_MAX 100

// Define a structure to store word-count pairs
struct WordCount {
    char word[MAX_WORD_LENGTH];
    int count;
};

// Function to compare WordCount structures for sorting
int compareWordCount(const void *a, const void *b) {
    struct WordCount *wc1 = (struct WordCount *)a;
    struct WordCount *wc2 = (struct WordCount *)b;
    // Sort by decreasing count first, and then lexicographically
    if (wc1->count > wc2->count)
        return -1;
    else if (wc1->count < wc2->count)
        return 1;
    return strcmp(wc1->word, wc2->word);
}

// Function to update word counts in a file
void processFile(const char *filename, struct WordCount *wordCounts, int *count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    char word[MAX_WORD_LENGTH];
    int bytesRead;

    char a[1];
    char b[1];
    char prev;
    char *newword = malloc(21);
    int returnval = 1;
    int wordlength = 0;
    returnval = read(fd,a,1); 
    int returnval2 = read(fd,b,1);
    int last = 3;
    while ((returnval != 0) || (last > 0)){
        if (isalpha(a[0])){
            newword[wordlength] = a[0];
            wordlength++;}
        else if ((a[0] == '-')&&(isalpha(prev))&&(isalpha(b[0]))){
            newword[wordlength] = '-';
            wordlength++;}
        else if ((a[0] == '\'')&&((isalpha(prev))||(isalpha(b[0])))){
            newword[wordlength] = a[0];
            wordlength++;}
        else{
            //printf("%s", newword);
            if (wordlength > 0){
                newword[wordlength] = '\0';
                int found = 0;
                //printf("checking %s\n", newword);
                for (int i = 0; i < *count; i++) {
                    if (strcmp(newword, wordCounts[i].word) == 0) {
                        wordCounts[i].count++;
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                   
                    // Add a new word to the list
                    strcpy(wordCounts[*count].word, newword);
                    wordCounts[*count].count = 1;
                    (*count)++;
                }}
                wordlength = 0;
                free(newword);
                newword = malloc(21);
        }
                prev = a[0];
                a[0] = b[0];
                returnval = read(fd,b,1);
                
                if (returnval <=0){
                    last--;
                    b[0] = '!';
                }
} close(fd);
}


// Function to process files and directories
void processEntry(const char *path, struct WordCount *wordCounts, int *count) {
    struct stat statbuf;
    
    if (stat(path, &statbuf) == -1) {
        perror("stat");
        return;
    }
    if (S_ISDIR(statbuf.st_mode)) {
        // If it's a directory, process its contents
        DIR *dir = opendir(path);
        if (dir == NULL) {
            perror("opendir");
            return;
        }
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char subpath[PATH_MAX];
            snprintf(subpath, PATH_MAX, "%s/%s", path, entry->d_name);

            processEntry(subpath, wordCounts, count);
        }

        closedir(dir);
    } else if (S_ISREG(statbuf.st_mode) && strstr(path, ".txt")) {
        // If it's a text file, process the file
        processFile(path, wordCounts, count);
    }
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file/directory [file/directory ...]\n", argv[0]);
        return 1;
    }

    struct WordCount wordCounts[1000]; // Assuming a maximum of 1000 unique words
    int count = 0;

    for (int i = 1; i < argc; i++) {
        processEntry(argv[i], wordCounts, &count);
    }

    // Sort and print the word counts
    qsort(wordCounts, count, sizeof(struct WordCount), compareWordCount);

    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
    }

    return 0;
}