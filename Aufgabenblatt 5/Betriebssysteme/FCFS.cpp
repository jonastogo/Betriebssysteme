
#ifdef __cpluplus
extern "C"
{
#endif

#include <cstddef>
#include "lists.h"
#include "Job.cpp"

class FCFS{
public:
	FCFS(JobList list){
		calculate(list);
	}

	void calculate(JobList list){
		int runtime;
		JobNode* first = list.head;
		while (first != NULL){
			runtime += first->runtime*first->priority;
			printf("%s wurde abgearbeitet (Aktuelle Zeit: %d s).", first->name, first->runtime*first->priority);
			first = first->next;
		}
		printf("Mittlere Verweilzeit: %f s", runtime/List_count(&list));
	}
};

#ifdef _cpluplus
}
#endif
