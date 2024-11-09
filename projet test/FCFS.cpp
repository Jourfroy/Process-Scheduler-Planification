#include "Process.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include <chrono>  // pour std::chrono
#include <thread>  // pour std::this_thread

using namespace std;


bool compareInstantArrive ( Process& P1, Process& P2){
    return P1.getm_tempsArrive() < P2.getm_tempsArrive();
}

void algorithme_FCFS_NonPreemptif (std::vector <Process>& processus) {

    std::sort(processus.begin(), processus.end(), compareInstantArrive);


    processus[0].setm_presenceMemoire ("Charge En Memoire");

    double memoireDispo = 100.0 ;
    int secondeMemoire = 0;
    int secondeES = 0;


    for (int i =1; i <= processus.size(); i++) {
        int secondeCPU = 0;

        if (processus[i-1].getm_presenceMemoire() == "Charge En Memoire"){
            // Minuteur pour simuler l'exécution CPU
            cout << "------------------------------------------------"<< endl;
            cout << "Le processus caracterise par :" << endl;
            cout << "-->Temps d'arrivee :  " << processus[i-1].getm_tempsArrive () << " seconde(s)" << endl;
            cout << "-->Temps Processeur : " << processus[i-1].getm_tempsCpu () << " seconde(s)" << endl;
            cout << "-->PID :              " << processus[i-1].getm_PID () << endl;
            cout << "-->Nom :              " << processus[i-1].getm_name () <<endl;


            processus[i-1].setm_tempsAttente (secondeMemoire - processus[i-1].getm_tempsArrive() + 1);
            processus[i-1].setm_tempsExecution (processus[i-1].getm_tempsAttente() + processus[i-1].getm_tempsCpu() + processus[i-1].getm_tempsES());

            while (secondeCPU <= processus[i-1].getm_tempsCpu()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Attendre une seconde par unité de temps CPU
                secondeCPU++;
                secondeMemoire++;
                secondeES++;

                //Verification si un processus ne devait pas etre charge en memoire pendant l,execution de celui-ci
                for (int j =i; j< processus.size(); j++) {
                    if (secondeMemoire == processus[j].getm_tempsArrive()) {
                        if (memoireDispo >= processus[j].getm_memoire()){
                            processus[j].setm_presenceMemoire("Charge En Memoire");
                        }else {
                            processus[j].setm_presenceMemoire("Liste d'attente");
                        }
                    }
                }

                //Verification si le temps entree/sortie d'un processus d'un processus deja executer n'est pas arrive
                //Si la condition ci-dessus est verifier, je sais que les i-1 processus ont ete executer avec succes
                for (int j = 0; j < i-1; j++ ) {
                    processus[j].setm_tempsES_Ecoule(processus[j].getm_tempsES_Ecoule()+1);
                    if (processus[j].getm_tempsES_Ecoule() >= processus[j].getm_tempsES() ){
                        processus[j].setm_presenceMemoire("");
                        processus[j].setm_etat ("Execution complete");
                        memoireDispo += processus[j].getm_memoire();
                    }
                }

            }
            cout << "Processus executer avec succes mais il sera encore present en memoire pour " << processus[i-1].getm_tempsES() << " seconde(s) !!!\n" <<endl;
        }else {

            cout << "Aucun processus charge en memoire !!!" << endl;
            cout << "Veuillez patienter pendant " << processus[i-1].getm_tempsArrive() - secondeMemoire << " seconde(s) le temps que le processus " << processus[i-1].getm_name () << " soit lancer et charge en memoire pour etre executer !!!" << endl;
            while (secondeMemoire <= processus[i-1].getm_tempsArrive()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Attendre une seconde par unité de temps
                secondeMemoire++;
            }

            //Verification si le temps entree/sortie d'un processus d'un processus deja executer n'est pas arrive
            for (int j = 0; j < i-1; j++ ) {
                processus[j].setm_tempsES_Ecoule(processus[j].getm_tempsES_Ecoule()+1);
                if (processus[j].getm_tempsES_Ecoule() >= processus[j].getm_tempsES() ){
                    processus[j].setm_presenceMemoire("");
                    processus[j].setm_etat ("Execution complete");
                    memoireDispo += processus[j].getm_memoire();
                }
            }
        }

    }
    //Ceci n'est pas pris en compte dans la boucle
    for (int i = 0; i < processus.size(); i++) {
        processus[i].setm_etat("Execution complete");
    }
}
