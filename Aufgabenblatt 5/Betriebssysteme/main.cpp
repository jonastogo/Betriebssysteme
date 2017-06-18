/*
 * main.c
 *
 *  Created on: 11.06.2017
 *      Author: Jonas-Lampe
 */

#include <windows.h>
#include "lists.h"
#include "FCFS.cpp"
#include "Job.cpp"
#include "SJF.cpp"

static JobList list;
Job* a;
Job* b;
Job* c;
Job* d;
Job* e;
FCFS* fcfs;
SJF* sjf;

int main(){
	List_init(&list);
	initJobs();
	return 0;
}

int initJobs(){
	a = LIST_NEW_NODE(JobNode);
	a->name = "A";
	a->runtime = 30;
	a->priority = 2;

	b = LIST_NEW_NODE(JobNode);
	b->name = "B";
	b->runtime = 20;
	b->priority = 1;

	c = LIST_NEW_NODE(JobNode);
	c->name = "C";
	c->runtime = 25;
	c->priority = 5;

	d = LIST_NEW_NODE(JobNode);
	d->name = "D";
	d->runtime = 28;
	d->priority = 4;

	e = LIST_NEW_NODE(JobNode);
	e->name = "E";
	e->runtime = 18;
	e->priority = 3;

	List_append(&list, a);
	List_append(&list, b);
	List_append(&list, c);
	List_append(&list, d);
	List_append(&list, e);

	fcfs = new FCFS(list);
	sjf = new SJF(list);

	return 0;
}


