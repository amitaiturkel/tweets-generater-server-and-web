tweets: tweets_generator.o linked_list.o markov_chain.o
	gcc -Wall -Wextra -Wvla -std=c99 linked_list.o tweets_generator.o markov_chain.o -o tweets_generator

snake: snakes_and_ladders.o markov_chain.o linked_list.o
	gcc -Wall -Wextra -Wvla -std=c99 snakes_and_ladders.o markov_chain.o linked_list.o -o snake

markov_chain.o: markov_chain.c markov_chain.h linked_list.h
	gcc -Wall -std=c99 -c markov_chain.c

linked_list.o: linked_list.c linked_list.h
	gcc -Wall -std=c99 -c linked_list.c

tweets_generator.o : tweets_generator.c markov_chain.h
	gcc -Wall -std=c99 -c tweets_generator.c

snakes_and_ladders.o: snakes_and_ladders.c markov_chain.o
	gcc -Wall -std=c99 -c snakes_and_ladders.c