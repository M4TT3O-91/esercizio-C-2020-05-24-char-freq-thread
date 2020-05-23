/*
 * esercizio-C-2020-05-24-char-freq-thread.c
 *
 *  Created on: May 22, 2020
 *      Author: marco
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

char *text =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut \nlabore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. \nPretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. \nBlandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. \nEtiam non quam lacus suspendisse faucibus interdum posuere. \nEget sit amet tellus cras adipiscing enim eu turpis egestas. \nLectus magna fringilla urna porttitor rhoncus dolor purus non. \nSit amet consectetur adipiscing elit duis tristique sollicitudin nibh. \nNec tincidunt praesent semper feugiat nibh. \nSapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter[256];

void* thread_function_odd(void *arg) {
	int s;
	char *str = (char*) arg;
	int ch;

	for (int i = 1; i < strlen(str); i += 2) {

		// 3.4.2 Mutual exclusion solution, pag. 19
		s = pthread_mutex_lock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		ch = (int) str[i];
		counter[ch]++;

		s = pthread_mutex_unlock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

	}

	return NULL;
}
void* thread_function_even(void *arg) {
	int s;
	char *str = (char*) arg;
	int ch;

	for (int i = 0; i < strlen(str); i += 2) {
		ch = str[i];
		// 3.4.2 Mutual exclusion solution, pag. 19
		s = pthread_mutex_lock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		ch = (int)str[i];
		counter[ch]++;

		s = pthread_mutex_unlock(&mutex);
		if (s != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

	}

	return NULL;
}

int main(int argc, char *argv[]) {

	pthread_t t1;
	pthread_t t2;
	void *res;

	int s;
	s = pthread_create(&t1, NULL, thread_function_odd, text);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	s = pthread_create(&t2, NULL, thread_function_even, text);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t1, &res); //<------------JOIN  T1
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t2, &res); //<------------JOIN  T2
	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 256; i++) {
		printf("frequenza carattere %c = %d\n", i, counter[i]);
	}

	return 0;
}

