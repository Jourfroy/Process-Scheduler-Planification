#ifndef SJF_H_INCLUDED
#define SJF_H_INCLUDED

#include "Process.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>



bool compareTempsArrive (const Process& P1, const Process& P2) ;

void algorithme_SJF_NonPreemptif (std::vector <Process> processus) ;

#endif // SJF_H_INCLUDED
