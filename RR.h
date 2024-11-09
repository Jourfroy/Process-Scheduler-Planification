#ifndef RR_H_INCLUDED
#define RR_H_INCLUDED

#include "Process.h"

void algorithme_RR_Preemptif (std::vector <Process> &processus, int quantum);
void algorithme_RR_NonPreemptif (std::vector <Process> &processus, int quantum);

#endif // RR_H_INCLUDED
