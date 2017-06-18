/*
 * Job.c
 *
 *  Created on: 11.06.2017
 *      Author: Jonas-Lampe
 */

#include "lists.h"

typedef struct Job {

	LIST_NODE_HEADER(struct Job);
	char *name;
	float runtime;
	int priority;

}JobNode;

typedef struct {

	LIST_HEADER(JobNode);

} JobList;

int compareJobNodes(JobNode *node1, JobNode *node2, void *userData) {

	if (node1->priority > node2->priority)
		return 1;
	else if (node1->priority < node2->priority)
		return -1;
	else
		return 0;

}


