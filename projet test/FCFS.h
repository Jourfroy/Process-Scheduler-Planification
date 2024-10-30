#ifndef FCFS_H_INCLUDED
#define FCFS_H_INCLUDED

#include "Process.h"
#include <iostream>
#include <vector>


void algorithme_FCFS_NonPreemptif (std::vector <Process>& processus);
bool compare ( Process& P1, Process& P2);

#endif // FCFS_H_INCLUDED
