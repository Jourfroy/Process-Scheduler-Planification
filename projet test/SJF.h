#ifndef SJF_H_INCLUDED
#define SJF_H_INCLUDED

#include "Process.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>



bool compareTempsArrive (const Process& P1, const Process& P2) ;

void algorithme_SJF_NonPreemptif (std::vector <Process>& processus) ;
void algorithme_SJF_Preemptif(std::vector<Process>& processus);
void chargerEnMemoire(std::vector<Process>& memory, std::vector<Process>& waiting_list, double& memoireDispo);
int retrouverIndice(std::vector<Process>& processus, int PID_x);
void affichage (Process &p);


#endif // SJF_H_INCLUDED
