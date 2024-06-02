# Word-Occurrence-Count

words takes one or more arguments, which may be text files or directories. words will open and
process each named text file. For directories, words will recursively seek and process files in the
directory whose names end with “.txt”.
words must use the functions open(), close(), and read() to read from files and write() to
print to standard output. Use of sprintf() to format decimal numbers is permitted.
words must use the functions opendir(), closedir(), and readdir() to read the contents of
directories. It may use stat() to determine if a file is a directory.
words will process one or more files by keeping maintaining a count of how many times each
word was encountered. It maintains a mapping from words to counts, adding new words as they are
encountered, and increasing the count of words that have been seen earlier. When processing all
files, the count will indicate the number of times a word has been seen in all files. (That is, counts
are not kept per-file.)
Once all files have been processed, words will print the list of words and counts sorted by
decreasing appearance count. Words with the same count will be ordered lexicographically. Each
word will be printed on a separate line, followed by its number of occurrences.
words should not assume a maximum file size or a maximum word length.
words should assume that all files and directories in its arguments are distinct. That is, words
does not need to check whether a file has been listed more than once.
