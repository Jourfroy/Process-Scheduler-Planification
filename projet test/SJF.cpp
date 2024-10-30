#include "Process.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include <chrono>  // pour std::chrono
#include <thread>  // pour std::this_thread


using namespace std ;


bool compareTempsArrive (const Process& P1, const Process& P2) {
    return P1.getm_tempsArrive() < P2.getm_tempsArrive();
}

void algorithme_SJF_NonPreemptif (std::vector <Process> processus) {

    std::sort(processus.begin(), processus.end(), compareTempsArrive);

    processus[0].setm_presenceMemoire ("Charge En Memoire");

    double memoireDispo = 100.0 ;
    int secondeMemoire = 0;
    int secondeES = 0;
    std::vector <Process> memory; // Ce vecteur contient les processus charge en memoire
    std::vector <Process> waiting_list;
    std::vector <Process> ProcessusExecuter;
    memory.push_back (processus[0]);
    int indiceProcessus;


    for (int i =1; i <= processus.size(); i++) {
        int secondeCPU = 0;


        //Recherche du processus ayant le plus petit temps processeur parmi ceux qui sont charges en memoire
        if (!memory.empty()){

            int tempsProcesseur = memory[0].getm_tempsCpu();
            int found;
            for (int j = 0; j < memory.size(); j++) {
                if (memory[j].getm_tempsCpu() < tempsProcesseur && memory[j].getm_dejaExecuter() == false) {
                    found = j;
                }
            }

            // Minuteur pour simuler l'exécution CPU
            cout << "------------------------------------------------"<< endl;
            cout << "Le processus caracterise par :" << endl;
            cout << "-->Temps d'arrivee :  " << memory[found].getm_tempsArrive () << " seconde(s)" << endl;
            cout << "-->Temps Processeur : " << memory[found].getm_tempsCpu () << " seconde(s)" << endl;
            cout << "-->PID :              " << memory[found].getm_PID () << endl;
            cout << "-->Nom :              " << memory[found].getm_name () <<endl;


            memory[found].setm_tempsAttente (secondeMemoire - memory[found].getm_tempsArrive() + 1);
            memory[found].setm_tempsExecution (memory[found].getm_tempsAttente() + memory[found].getm_tempsCpu() + memory[found].getm_tempsES());

            memory[found].setm_dejaExecuter (true);
            while (secondeCPU <= memory[found].getm_tempsCpu()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Attendre une seconde par unité de temps CPU
                secondeCPU++;
                secondeMemoire++;
                secondeES++;

                //Verification si un processus ne devrait pas etre charge en memoire pendant l'execution de celui-ci
                for (int j = i - 1; j < processus.size(); j++) {
                    if (secondeMemoire == processus[j].getm_tempsArrive()) {

                        if (waiting_list.empty()){
                            memory.push_back(processus[j]);
                            memory[0].setm_presenceMemoire ("Charge En Memoire");
                        } else if (waiting_list[0].getm_memoire() >= memoireDispo){
                            memory.push_back(waiting_list[0]);
                            memory[memory.size() - 1].setm_presenceMemoire ("Charge En Memoire");
                            waiting_list.push_back(processus[j]);
                            waiting_list.erase(waiting_list.begin());
                            processus[j].setm_presenceMemoire("Liste d'attente");
                            memoireDispo -= waiting_list[0].getm_memoire();
                        } else {
                            waiting_list.push_back(processus[j]);
                            processus[j].setm_presenceMemoire("Liste d'attente");
                        }

                    }
                }


                //Verification si le temps entree/sortie d'un processus deja executer n'est pas arrive
                //Si la condition ci-dessus est verifier, je sais que les i-1 processus ont ete executer avec succes
                for (int j = 0; j < ProcessusExecuter.size(); j++ ) {
                    ProcessusExecuter[j].setm_tempsES_Ecoule(ProcessusExecuter[j].getm_tempsES_Ecoule()+1);
                    if (ProcessusExecuter[j].getm_tempsES_Ecoule() >= ProcessusExecuter[j].getm_tempsES() ){
                        ProcessusExecuter[j].setm_etat ("Execution complete");
                        ProcessusExecuter[j].setm_presenceMemoire("");

                        //std::remove (memory.begin(), memory.end(), ProcessusExecuter[j]) ;
                        int trouve ;
                        for (int k = 0; k < memory.size(); k++) {
                            if (memory[k].getm_PID() == ProcessusExecuter[j].getm_PID()) {
                                trouve = k;
                            }
                        }

                        memory.erase (memory.begin() + trouve);

                        // Chargement en memoire des processus de la liste d'attente
                        int j = 0;
                        int taille = waiting_list.size();
                        while (memoireDispo - waiting_list[j].getm_memoire() >= 0 && j < taille) {
                            memory.push_back(waiting_list[0]);
                            memoireDispo -= waiting_list[0].getm_memoire();
                            waiting_list.erase(waiting_list.begin());
                            j++;
                        }
                    }
                }

            }

            cout << "Processus executer avec succes mais il sera encore present en memoire pour " << memory[found].getm_tempsES() << " seconde(s) !!!\n" <<endl;

            ProcessusExecuter.push_back(memory[found]);

        }else {

            cout << "Aucun processus charge en memoire !!!" << endl;
            cout << "Veuillez patienter pendant " << processus[i-1].getm_tempsArrive() - secondeMemoire << " seconde(s) le temps que le processus " << processus[i-1].getm_name () << " soit lancer et charge en memoire pour etre executer !!!" << endl;
            while (10*secondeMemoire <= processus[i-1].getm_tempsArrive()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Attendre une seconde par unité de temps
                secondeMemoire++;
            }

            memory.push_back(processus[i-1]);

        }

    }

    processus.clear();
    processus.insert(processus.end(), ProcessusExecuter.begin(), ProcessusExecuter.end());

    /*
    //Ceci n'est pas pris en compte dans la boucle
    for (int i = 0; i < processus.size(); i++) {
        processus[i].setm_etat("Execution complete");
    }*/

}

    /*int tempsAttente = 0;
    int tempsExecution = 0;

    double memoireDispo = 100.0;
    for (int i = 0; i < processus.size(); i++){

        memoireDispo = memoireDispo - processus[i].getm_memoire() ;
        if (memoireDispo >= 0){
            processus[i].setm_etat ("Charge en Memoire") ;
        } else {
            processus[i].setm_etat ("Waiting List");
        }

        tempsAttente = tempsAttente + processus[i].getm_tempsAttente ();
        tempsExecution = tempsAttente + processus[i].getm_tempsCpu ();
        processus[i].setm_tempsAttente (tempsAttente);
        processus[i].setm_tempsExecution (tempsExecution) ;
        tempsAttente = tempsAttente + processus[i].getm_tempsCpu ();
    }*/

