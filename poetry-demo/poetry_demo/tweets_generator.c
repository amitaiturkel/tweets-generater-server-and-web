#include "markov_chain.h"
#include <string.h>
#include <time.h>
#define MAX_SEN 1001
#define MAX_ARGC 5
#define MIN_ARGC 4
#define SEED_INDEX 1
#define NUM_TWEET_INDEX 2
#define PATH_INDEX 3
#define WORDS_INDEX 4
#define ALL_FILE -1
#define MAX_TWEETS 20
#define POS_NUM 5
#define ONE 1
#define BASE 10
#define PATH 100
#define JSON_FILE_NAME "tweets.json" // Define the name of the JSON file


void free_string(void * string){

  if (string == NULL){
    return ;
  }
  char * to_free = (char*)string;
  free(to_free);
}

int string_com(void * first,void *sec){
  const char* new_first = ( const char*) first;
  const char* new_sec = (const char*) sec;
  return strcmp(new_first,new_sec);
}

void print_str_to_file(void *word,void* p_file){
  const char* new_word = (const char*) word;
  FILE * file = (FILE*) p_file;
  if (file == NULL)
  {
    printf ("Error opening the file1.\n");
  }
  fprintf(file,"%s",new_word);
  
}
void*  copy_str(void* src){
  char* from = (char*) src;
  char* dis =  malloc (strlen(from)+1);
  if(dis == NULL){
    return NULL;
  }
  strcpy (dis,from);
  return dis;

}
// get a valid markovNode with data in it
bool last_it_period(void * old_data){
  char* data = (char* )old_data ;
  if(data[strlen((data))-1] == '.'){
    return true;
  }
  return false;
}


int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
  if (markov_chain == NULL) {
    return EXIT_FAILURE;
  }

  char line[MAX_SEN];
  int words_read = 0;
  int endless_read = 0;

  if (words_to_read == -1) {
    words_to_read = POS_NUM;
    endless_read = ONE;
  }

  while (words_read < words_to_read && fgets(line, sizeof(line), fp) != NULL) {
    Node *last_word = NULL;
    char *word = strtok(line, " \t\n\r");

    while (word != NULL) {
      if (words_read == words_to_read) {
        return EXIT_SUCCESS;
      }

      if (last_word == NULL) {
        // First word in a sentence
        last_word = add_to_database(markov_chain, word);
        if (last_word == NULL) {
          return EXIT_FAILURE;
        }

        word = strtok(NULL, " \t\n\r");

        if (endless_read != 1) {
          words_read++;
        }
        continue;
      }

      Node *this_word = add_to_database(markov_chain, word);
      if (this_word == NULL) {
        return EXIT_FAILURE;
      }

      if (add_node_to_frequencies_list(last_word->data, this_word->data, markov_chain) == EXIT_FAILURE) {
        return EXIT_FAILURE;
      }

      if (markov_chain->is_last(this_word->data->data)) {
        last_word = NULL;

        if (endless_read != 1) {
          words_read++;
        }

        word = strtok(NULL, " \t\n\r");
        continue;
      }

      last_word = this_word;

      if (endless_read != 1) {
        words_read++;
      }

      word = strtok(NULL, " \t\n\r");
    }
  }

  return EXIT_SUCCESS;
}



int isvalidpath(char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    perror ("failed");
    return EXIT_FAILURE;
  } else {
    fclose (file);
    return EXIT_SUCCESS;
  }
}

int correct_argv(char *argv[], int argc){
  if(argc != MAX_ARGC && argc != MIN_ARGC ){
    printf("USAGE:Invalid number of arguments.\n");
    return EXIT_FAILURE;
  }
  if(isvalidpath(argv[PATH_INDEX]) == EXIT_FAILURE){
    printf("ERROR:Invalid path.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  int failure = EXIT_SUCCESS;

  // Check command line arguments
  if (correct_argv(argv, argc) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  // Allocate memory for the MarkovChain struct
  MarkovChain *markov_chain = malloc(sizeof(*markov_chain));
  if (markov_chain == NULL) {
    printf("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }

  // Initialize the MarkovChain struct
  markov_chain->database = NULL;
  markov_chain->comp_func = string_com;
  markov_chain->copy_func = copy_str;
  markov_chain->is_last = last_it_period;
  markov_chain->print_func = print_str_to_file;
  markov_chain->free_data = free_string;

  // Set the random seed
  // optional, i cancle it for more random
//  int seed = strtol(argv[SEED_INDEX], NULL, BASE);
//  srand(seed);
  srand(time(NULL));

  // Open the tweets file
  FILE *tweets = fopen(argv[PATH_INDEX], "r");
  if (tweets == NULL) {
    printf("Error: can't read file\n");
    return EXIT_FAILURE;
  }

  // Determine the number of tweets to generate
  int num_tweet = strtol(argv[NUM_TWEET_INDEX], NULL, BASE);

  // Fill the database with words from the tweets file
  if (argc == MIN_ARGC) {
    failure = fill_database(tweets, ALL_FILE, markov_chain);
  } else {
    int word_to_tweet = strtol(argv[WORDS_INDEX], NULL, BASE);
    failure = fill_database(tweets, word_to_tweet, markov_chain);
  }

  // Check if filling the database was successful
  if (failure == EXIT_FAILURE) {
    free_database(&markov_chain);
    printf("%s", ALLOCATION_ERROR_MASSAGE);
    fclose(tweets);
    return EXIT_FAILURE;
  }

  // Generate and print the tweets
  for (int i = 1; i <= num_tweet; i++) {
    MarkovNode *first_word = get_first_random_node(markov_chain);
    if (first_word == NULL) {
      free_database(&markov_chain);
      fclose(tweets);
      printf("%s", ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }
    FILE *file = fopen("output.txt", "a"); // Open the file for writing
    if (file == NULL) {
      printf("Error opening the file.\n");
      return 1; // Exit with an error code
    }

    fprintf(file,"Tweet %d: ", i);
    generate_tweet (markov_chain, first_word, MAX_TWEETS, file);
  }

  // Clean up and free memory
  fclose(tweets);
  free_database(&markov_chain);

  return EXIT_SUCCESS;
}
















