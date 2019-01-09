#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <windef.h>

#define num_of_files 7
typedef struct file {
    const char *key;
} File;

typedef struct hashTable {
    char *word;
    int documents[num_of_files];
} HashTable;

int compute_table_size(int word_count);

void create_hash_table(char **words, int *doc_word_count, int table_size, int shingle_size);

int find_index(char *shingle, int table_size);

int find_second_index(char *shingle, int table_size);


char **split_line(char *line) {
//    char tokens[20][20];

    char **tokens = malloc(150 * sizeof(char *));
    int i;
    for (i = 0; i != 150; ++i) {
        tokens[i] = malloc(20 * sizeof(char));
    }

    int j, ctr;
    j = 0;
    ctr = 0;
    int flg = 0;
    char *tmp;

    if (line[0] == ' ' || line[0] == '\0' || line[0] == '\n') {
        flg = -1;
    }

    for (i = 0; i <= (strlen(line)); i++) {
        //printf("i: %d = |%c| \n", i, line[i]);
        // if space or NULL found, assign NULL into newString[ctr

        if (line[i] == ' ' || line[i] == '\0' || line[i] == '\n') {
            if (flg == 0) {
                tokens[ctr][j] = '\0';
                //printf("ctr,j: %d,%d = |%c| \n", ctr, j, '\0');
                ctr++;  //for next word
                j = 0;    //for next word, init index to 0
                flg = 1;
            }

        } else {
            if (ispunct(line[i]) == FALSE) {

//                 printf("ctr,j: %d,%d = |%c| \n", ctr, j, tolower(line[i]));
                tokens[ctr][j] = tolower(line[i]);
                j++;
                flg = 0;
            }
        }
    }

//    printf("ctr:%d\n",ctr);
    for (i = ctr; i < 150; i++)
        tokens[i] = NULL;

    return tokens;
}

void main() {
    char *files = "C:\\Users\\Mukaddes\\Desktop\\files.txt";
    FILE *fp;
    char *directory = NULL;
    size_t len = 0;
    int doc_count = 0;
    int word_count[num_of_files + 1];

    int total_word_count = 0;
    int table_size = 0;

    for (int i = 0; i < num_of_files + 1; i++)
        word_count[i] = 0;

    char **words = calloc(num_of_files * 1000, sizeof(char *));
    for (int i = 0; i != 20; ++i) {
        words[i] = calloc(20, sizeof(char));
    }

    if (words == NULL) {
        printf("Allocation Error!\n");
        exit;
    }

    fp = fopen(files, "r");
    if (fp == NULL) {
        printf("File error x!\n");
        exit;
    }

    while ((getline(&directory, &len, fp)) != -1) {
        directory[strcspn(directory, "\n")] = '\0';
//        printf("-----------------Start of doc:\n");
        FILE *fp1 = fopen(directory, "r");
        if (fp1 == NULL) {
            printf("File error y!\n");
            exit;
        }
        doc_count++;

        char *line = NULL;
        while ((getline(&line, &len, fp1)) != -1) {
//            printf("-----------------Start of line:\n");
            char **tokens = split_line(line);
            for (int i = 0; tokens[i] != NULL; i++) {

                if (total_word_count <= 1000 * num_of_files) {
//                    printf("|%s|\n", *(tokens + i));
                    words[total_word_count] = *(tokens + i);
                    word_count[doc_count]++;
                    total_word_count++;
                } else {
                    char **wordsTemp;
                    if ((wordsTemp = (char **) realloc(words, 1000 * num_of_files)) == NULL)
                        printf("ERROR");
                    else
                        words = wordsTemp;

                    for (int k = 0; k < 1000 * num_of_files; k++) {
                        if ((wordsTemp[i] = (char *) realloc(wordsTemp[i], 20)) == NULL)
                            printf("\n\n Error...");
                        else
                            words[i] = wordsTemp[i];
                    }
//                    printf("|%s|\n", *(tokens + i));
                    words[i] = *(tokens + i);
                    word_count[doc_count]++;
                    total_word_count++;
                }
            }
        }
    }

//    for (int t = 0; t < total_word_count; t++)
//        printf("w %d: !%s!\n", t, *(words + t));

    table_size = compute_table_size(total_word_count);

    create_hash_table(words, word_count, table_size, 8);

}


void create_hash_table(char **words, int *doc_word_count, int table_size, int shingle_size) {

    HashTable *hashtable = calloc(table_size, sizeof(HashTable));

    if (hashtable == NULL) {
        printf("Allocation Error 5!\n");
        exit;
    }

    for (int i = 0; i < table_size; i++) {
        (hashtable[i].word) = NULL;
        for (int j = 0; j < num_of_files; j++)
            hashtable[i].documents[j] = 0;
    }

    for (int i = 0; i < num_of_files + 1; i++) {
//        printf("doc count: %d\n", doc_word_count[i]);
    }
    int start = 0;
    int end = 0;
    int tmpc = 0;
    {
        for (int i = 0; i < num_of_files; i++) {
            start += doc_word_count[i];
            end = start + doc_word_count[i + 1];
            for (int j = start; j < end - (shingle_size - 1); j++) {
                char shingle[160];
                if (shingle_size == 4) {
                    snprintf(shingle, 160, "%s %s %s %s", words[j], words[j + 1], words[j + 2], words[j + 3]);
                } else if (shingle_size == 5) {
                    snprintf(shingle, 160, "%s %s %s %s %s", words[j], words[j + 1], words[j + 2], words[j + 3],
                             words[j + 4]);
                } else if (shingle_size == 8) {
                    snprintf(shingle, 160, "%s %s %s %s %s %s %s %s", words[j], words[j + 1], words[j + 2],
                             words[j + 3], words[j + 4], words[j + 5], words[j + 6], words[j + 7]);
                }

//                printf("tmpc: %d\n", tmpc);
                //snprintf(shingle, 80, "%s %s %s %s", words[j], words[j + 1], words[j + 2], words[j + 3]);
//                printf("%d |%s\n", i, shingle);
                tmpc++;
                char *sptr = shingle;
                int h1 = find_index(shingle, table_size);
//                printf("h1: %d\n", h1);
                // place is empty, put the shingle to that index
                if (hashtable[h1].word == NULL) {
//                    printf("girdi1\n");
                    (hashtable[h1].word) = strdup(shingle);
                    hashtable[h1].documents[i] = 1;
//                    printf("|h1:%d|---|%s|\n", h1, hashtable[h1].word);
                }
                    //shingle is existing in that index, already
                else if (strcmp(shingle, hashtable[h1].word) == 0) {
//                    printf("girdi2\n");
                    hashtable[h1].documents[i]++;
//                    printf("|h1:%d|---|%s|    |doc:%d|---|num:%d|\n", h1, shingle, i, hashtable[h1].documents[i]);
                }
                    // index of shingle is already filled
                else {
//                    printf("girdi3\n");
                    int shift = 1;
                    int h2 = find_second_index(shingle, table_size);
                    int double_ind = (h1 + shift * h2) % table_size;
                    while (hashtable[double_ind].word != NULL && strcmp(sptr, hashtable[double_ind].word) != 0) {
//                        printf("girdi4\n");
                        shift++;
                        if (h2 == 0)
                            h2++;
                        double_ind = (h1 + shift * h2) % table_size;

//                        printf("%d %d %d\n", h1, h2, double_ind);
                    }
                    if (hashtable[double_ind].word == NULL) {
//                        printf("girdi5\n");
                        (hashtable[double_ind].word) = strdup(shingle);
                        hashtable[double_ind].documents[i] = 1;
//                        printf("|double_ind:%d|---|%s|\n", h2, shingle);
                    } else {
//                        printf("girdi6\n");
                        hashtable[double_ind].documents[i]++;
//                        printf("|double_ind:%d|---|%s|    |doc:%d|---|num:%d|\n", double_ind, shingle, i,
//                               hashtable[h1].documents[i]);
                    }

                }


            }

        }

    }

    int *num_of_shingles = calloc(num_of_files, sizeof(int));
    int total_shingle = 0;

    int *doc_shingles = calloc(num_of_files, sizeof(int));

    printf("Hash Table:\n");
    printf("%s%-50s", "Index|","Shingle");
    for(int i = 0; i < num_of_files; i++)
        printf("%6d",i+1);
    printf("\n");

    for (int i = 0; i < table_size; i++) {
        printf("%2d| %-50s | ", i, hashtable[i].word);
        if(hashtable[i].word != NULL)
            total_shingle++;
        for (int j = 0; j < num_of_files; j++) {
            printf("%5d ", hashtable[i].documents[j]);
            if(hashtable[i].documents[j] != 0){
                doc_shingles[j]++;
            }
            num_of_shingles[j] += hashtable[i].documents[j];
        }
        printf("\n");

    }
    printf("Total number of %d shingles in %d documents: %d\n",shingle_size, num_of_files, total_shingle);
    printf("Number of unique shingles in documents:\n");
    for (int i = 0; i < num_of_files; i++) {
        printf("%d |%d\n", i+1, doc_shingles[i]);
    }

    double **jaccard_table = calloc(num_of_files, sizeof(double *));
    for (int i = 0; i != num_of_files; ++i) {
        jaccard_table[i] = calloc(num_of_files, sizeof(double));
    }

    if (jaccard_table == NULL) {
        printf("Allocation Error 8!\n");
        exit;
    }
 // Compute Jaccard Similarity
    for (int i = 0; i < num_of_files ; i++)
        for (int j = i; j < num_of_files; j++) {
            //printf("%d %d\n",i,j);
            int intersection = 0;
            int join = 0;
            for (int k = 0; k < table_size; k++) {
                //printf("basta %d %d\n",hashtable[k].documents[i], hashtable[k].documents[j]);
                if (hashtable[k].documents[i] && hashtable[k].documents[j]) {
                    //printf("gırdı %d %d\n",hashtable[k].documents[i], hashtable[k].documents[j]);
                    intersection++;
                    join++;
                } else if (hashtable[k].documents[i] || hashtable[k].documents[j]) {
                    //printf("GIRDI %d %d\n",hashtable[k].documents[i], hashtable[k].documents[j]);
                    join++;
                }


            }
            double tmp = (double) intersection;
            //printf("js: %lf\n", (tmp) / join);
            jaccard_table[i][j] = ((tmp) / join);
        }

    printf("Jaccard Similarity Table for %d-Shingle:\n",shingle_size);
    for (int i = 0; i < num_of_files; i++)
        printf("%9d ",i+1);
    printf("\n");
    for (int i = 0; i < num_of_files; i++) {
        printf("%2d: ",i+1);
        for (int j = 0; j < num_of_files; j++) {
            printf("%lf |", jaccard_table[i][j]);
        }
        printf("\n");
    }
 // Create signature matrix
    int signature[100][num_of_files];

    for (int k = 0; k < 100; k++) {
        int hashfunc[table_size];
        int a = rand() % table_size;
        for (int t = 0; t < table_size; t++){
            hashfunc[t] = (a * t + 1) % table_size;
        }

        int min[num_of_files];
        for (int t = 0; t < num_of_files; t++)
            min[t] = 10000;

        for (int i = 0; i < table_size; i++) {
            //printf("i %d \n",i);
            for (int j = 0; j < num_of_files; j++) {
                //printf("i j %d %d\n",i,j);
                //printf("%d %d %d\n", hashtable[hashfunc[i]].documents[j], min[j], hashfunc[i]);
                if (hashtable[hashfunc[i]].documents[j] == 1 && min[j] == 10000) {
                    min[j] = hashfunc[i];
                    //printf("min %d\n",min[j]);
                }

            }
        }
        for (int t = 0; t < num_of_files; t++)
            signature[k][t] = min[t];
    }
    // Compute signature ratio
    double **signature_ratio = calloc(num_of_files, sizeof(double *));
    for (int i = 0; i != num_of_files; ++i) {
        signature_ratio[i] = calloc(num_of_files, sizeof(double));
    }

    if (signature_ratio == NULL) {
        printf("Allocation Error 8!\n");
        exit;
    }

    int intersection = 0;
    for (int i = 0; i < num_of_files ; i++) {
        for (int j = i; j < num_of_files; j++) {
            for (int k = 0; k < 100; k++) {
                if (signature[k][i] == signature[k][j])
                    intersection++;
            }
            double tmp = (double)intersection;
            signature_ratio[i][j] = tmp / 100;
            intersection = 0;
        }
    }
    printf("Signature Ratio Table for %d-Shingle:\n",shingle_size);
    for (int i = 0; i < num_of_files; i++)
        printf("%9d ",i+1);
    printf("\n");
    for (int i = 0; i < num_of_files; i++) {
        printf("%2d: ",i+1);
        for (int j = 0; j < num_of_files; j++) {
            printf("%lf |", signature_ratio[i][j]);
        }
        printf("\n");
    }


    printf("Signature Matrix for %d-Shingle:\n",shingle_size);
    printf("%s","# of hash function|");
    for (int j = 0; j < num_of_files; j++)
        printf("%6d",j+1);
    printf("\n");
    for (int i = 0; i < 100; i++) {
        printf("%18d| ", i+1);
        for (int j = 0; j < num_of_files; j++) {
            printf("%5d ", signature[i][j]);
        }
        printf("\n");
    }
}

int compute_table_size(int word_count) {
    int table_size = 2 * word_count;

    int flag = 1, index, count = 0;

    while (flag == 1) {
        flag = 0;
        for (int i = 2; i <= table_size / 2; i++) {
            if (table_size % i == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            table_size++;
    }
//    printf("table size:%d\n", table_size);

    return table_size;
}

int find_index(char *shingle, int table_size) {
    long horner_sum = 0;
    long power;
    int h1 = 0;

    for (int m = 0; (shingle[m] != '\0'); m++) {
        power = (long) ((pow(2, m)));
//        printf("power: %lu\n",power);
        power = power % 1021;
        horner_sum += ((int) shingle[m]) * power;
//        printf("hornersum: %lu\n",horner_sum);
//        printf("m1 = %d | pow1 = %d \n",m,horner_sum);
    }
    h1 = (int) (horner_sum % table_size);

//    printf("h1: %d| tablesize:%d\n",h1,table_size);
//    printf("horner1:%s  h1: %d \n", horner_sum, h1);

    return h1;
}

int find_second_index(char *shingle, int table_size) {
    long horner_sum = 0;
    long power;
    int h2 = 0;

    for (int m = 0; (shingle[m] != '\0'); m++) {
        power = (long) ((pow(2, m)));
        power = power % 107;
        horner_sum += ((int) shingle[m]) * power;
//        printf("m2 = %d | pow2 = %d \n",m,horner_sum);
    }
    h2 = horner_sum % 37;
//    printf("horner2:%s  h2: %d \n", horner_sum, h2);

    return h2;
}
