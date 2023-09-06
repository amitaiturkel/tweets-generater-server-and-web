#include "markov_chain.h"
#include <string.h>


/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
 Node *add_to_database(MarkovChain *markov_chain, void *data_ptr) {
  if (markov_chain == NULL) {
    return NULL;
  }

  int failure = 1, empty_database = 0;

  if (markov_chain->database == NULL) {  // If there is no database, create one
    markov_chain->database = malloc(sizeof(LinkedList));
    if (markov_chain->database == NULL) {
      return NULL;
    }
    empty_database = 1;
    markov_chain->database->first = NULL;
    markov_chain->database->last = NULL;
    markov_chain->database->size = 0;
  }

  MarkovNode *markovnode = malloc(sizeof(MarkovNode));
  if (!(markovnode)) {
    if (empty_database) {
      free(markov_chain->database);
      markov_chain->database = NULL;
    }
    markov_chain->free_data(data_ptr);
    return NULL;
  }

  void *word_to_put = markov_chain->copy_func(data_ptr);
  if (word_to_put == NULL) {
    return NULL;
  }

  markovnode->data = word_to_put;  // Put data
  markovnode->frequencies_list = NULL;  // Not a word yet
  markovnode->words_in_frequencies = 0;
  markovnode->array_frequencies_size = 0;

  LinkedList *linked_list = markov_chain->database;

  if (linked_list->first == NULL) {  // The database is empty
    failure = add(linked_list, markovnode);
    if (failure) {
      free(markovnode);
      markovnode = NULL;
      markov_chain->free_data(word_to_put);
      if (empty_database) {
        free(markov_chain->database);
        markov_chain->database = NULL;
      }
      return NULL;
    }
    return linked_list->first;
  }

  Node *temp = linked_list->first;
  while (temp != NULL) {
    if (markov_chain->comp_func((temp->data)->data, data_ptr) == 0) {
      markov_chain->free_data(word_to_put);  // If word already exists, no need for markovnode and data_ptr
      free(markovnode);
      return temp;
    }
    temp = temp->next;
  }

  failure = add(linked_list, markovnode);
  if (failure) {
    free(markovnode);
    if (empty_database) {
      free(markov_chain->database);
      markov_chain->database = NULL;
    }
    return NULL;
  }
  return linked_list->last;
}

/**
* Check if data_ptr is in database. If so,
 * return the markov_node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database(MarkovChain *markov_chain, void *data_ptr) {
  if (!markov_chain->database) {
    return NULL;
  }

  LinkedList *linked_list = markov_chain->database;
  Node *temp = linked_list->first;

  while (temp != NULL) {
    if (markov_chain->comp_func((temp->data)->data, data_ptr) == 0) {
      return temp;
    }
    temp = temp->next;
  }

  return NULL;
}

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_frequencies_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain) {
  int i = 0;

  if (first_node->frequencies_list == NULL) {
    // If the frequencies list is empty, create an array of size 1
    MarkovNodeFrequency *new_node_freq_array = malloc(sizeof(MarkovNodeFrequency));
    if (!new_node_freq_array) {
      return EXIT_FAILURE; // Check allocation
    }
    first_node->frequencies_list = new_node_freq_array;
    first_node->frequencies_list[0].markov_node = second_node;
    first_node->frequencies_list[0].frequency = 1;
    first_node->array_frequencies_size = 1;
    first_node->words_in_frequencies = 1;
    return EXIT_SUCCESS;
  }

  if (first_node->array_frequencies_size == first_node->words_in_frequencies) {
    // If the array is full, check if the word is already there and update its frequency
    for (i = 0; i < first_node->words_in_frequencies; i++) {
      MarkovNodeFrequency *temp_next_word = &(first_node->frequencies_list[i]);
      if (markov_chain->comp_func(temp_next_word->markov_node->data, second_node->data) == 0) {
        temp_next_word->frequency++;
        return EXIT_SUCCESS;
      }
    }

    // If needed to add a word, reallocate the array with double the size
    MarkovNodeFrequency *temp_frequencies_list = first_node->frequencies_list;
    temp_frequencies_list = realloc(first_node->frequencies_list, (first_node->array_frequencies_size) * 2 * sizeof(MarkovNodeFrequency));
    if (!temp_frequencies_list) {
      return EXIT_FAILURE;
    }

    first_node->frequencies_list = temp_frequencies_list;
    first_node->frequencies_list[first_node->words_in_frequencies].markov_node = second_node;
    first_node->frequencies_list[first_node->words_in_frequencies].frequency = 1;
    first_node->words_in_frequencies++;
    first_node->array_frequencies_size = (first_node->array_frequencies_size) * 2;

    return EXIT_SUCCESS;
  }

  // If there are already words, check if the word is already in the list and update its frequency
  if (first_node->words_in_frequencies > 1) {
    for (i = 0; i < first_node->words_in_frequencies; i++) {
      MarkovNodeFrequency *temp_next_word = &(first_node->frequencies_list[i]);
      if (markov_chain->comp_func(temp_next_word->markov_node->data, second_node->data) == 0) {
        temp_next_word->frequency++;
        return EXIT_SUCCESS;
      }
    }
  }

  // If the word is not in the list, add it to the frequencies list
  first_node->frequencies_list[first_node->words_in_frequencies].frequency = 1;
  first_node->frequencies_list[first_node->words_in_frequencies].markov_node = second_node;
  first_node->words_in_frequencies++;

  return EXIT_SUCCESS;
}


/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database(MarkovChain **ptr_chain) {
  if ((*ptr_chain)->database == NULL) {
    return; // If the database is NULL, nothing to free
  }

  if ((*ptr_chain)->database->first == NULL) {
    return; // If the database is empty, nothing to free
  }

  Node *temp_node = (*ptr_chain)->database->first;
  while (temp_node != NULL) {
    if (temp_node->data != NULL) {
      if (temp_node->data->data != NULL) {
        (*ptr_chain)->free_data(temp_node->data->data); // Free the char pointer
      }
    }

    if (temp_node->data->frequencies_list != NULL) {
      free(temp_node->data->frequencies_list); // Free the frequencies list
      temp_node->data->frequencies_list = NULL;
    }

    Node *last_temp_node = temp_node;
    free(temp_node->data); // Free the data
    temp_node = temp_node->next;
    free(last_temp_node); // Free the node itself
  }

  free((*ptr_chain)->database); // Free the LinkedList
  (*ptr_chain)->database = NULL;
  free(*ptr_chain); // Free the MarkovChain
  *ptr_chain = NULL;
}


int get_random_number(int max_number){
  return rand() % max_number;
}

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  if (markov_chain == NULL)
  {
    return NULL;
  }
  if (markov_chain->database == NULL)
  {
    return NULL;
  }
  int size = markov_chain->database->size;
  if (size == 0)
  {
    return NULL;
  }
  int is_good = 1;
  Node *temp = markov_chain->database->first;
  while (is_good == 1)
  {
    temp = markov_chain->database->first;
    int random_num = get_random_number (size);
    for (int j = 0; j < random_num; j++)
    {
      temp = temp->next;
    }
    if (true != markov_chain->is_last(temp->data->data))
    {
      is_good = 0;
    }
  }
  return temp->data;}


/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  if(state_struct_ptr == NULL){
    return NULL;}

  int words_in_freq = state_struct_ptr ->words_in_frequencies;
  if(words_in_freq == 0)
  {
    return NULL;
  }
  int sum_of_freq = 0;
  for(int j = 0; j< words_in_freq;j++){
    sum_of_freq += (state_struct_ptr->frequencies_list)[j].frequency;
  }
  int random = get_random_number(sum_of_freq);
  MarkovNode * temp = state_struct_ptr;
  for (int i = 0; i < words_in_freq; ++i)
  {
    random -= temp->frequencies_list[i].frequency;
    if (random < 0)
    {
      return temp->frequencies_list[i].markov_node;
    }
  }
  return NULL;}




/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(MarkovChain *markov_chain, MarkovNode *first_node, int
max_length,FILE *p_file) {
  if (first_node == NULL) {
    first_node = get_first_random_node(markov_chain); // Get a random starting node if first_node is NULL
  }

  int is_period = 0;
  int words_in_tweet = 1;

  markov_chain->print_func(first_node->data,p_file); // Print the first word of
  // the tweet
  fprintf(p_file," ");

  MarkovNode *temp = get_next_random_node(first_node); // Get a random next node

  while (is_period == 0 && words_in_tweet < max_length - 1) {
    if (markov_chain->is_last(temp->data)) { // Check if the current node is the end of a tweet
      is_period = 1; // Set the flag to indicate the end of the tweet
      continue;
    }

    markov_chain->print_func(temp->data,p_file); // Print the word
    fprintf(p_file," ");

    temp = get_next_random_node(temp); // Get the next random node
    words_in_tweet++;
  }

  markov_chain->print_func(temp->data,p_file); // Print the last word of the tweet

  if (!markov_chain->is_last(temp->data)) {
    fprintf(p_file," ");
  }

  fprintf(p_file,"\n");
}


