#include "Process.h"
#include "SJF.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

void algorithme_RR_NonPreemptif(std::vector<Process>& processus, int quantum) {
    std::sort(processus.begin(), processus.end(), compareTempsArrive);
    processus[0].setm_presenceMemoire("Charge En Memoire");

    double memoireDispo = 100.0;
    int secondeMemoire = 1;
    std::vector<Process> memory, waiting_list, ProcessusExecuter;
    memory.push_back(processus[0]);
    memoireDispo -= memory[0].getm_memoire();
    int i = 1;
    int nbreExecute = 0;
    int found = 0;

    while (!memory.empty() || !waiting_list.empty()) {

        if (memory[memory.size()-1].getm_dejaExecuter() == true) {
            // Tous les processus en mémoire ont déjà été exécutés
            cout << "Tous les processus en memoire ont deja ete executes !!!" << endl;
            if (waiting_list.empty()){
                if (i < processus.size()) {
                    // Ici tous les processus sont deja executes et la liste d'attente est vide, donc il faut patienter qu'un processus soit charge en memoire et que le temps d'entree/sortie d'un processus arrive
                    cout << "Tous les processus en memoire ont deja ete executes !" << endl;
                    cout << "En attente du processus " << processus[i].getm_name() << " pendant " << processus[i].getm_tempsArrive() - secondeMemoire + 1 << " secondes." << endl;
                    while ((processus[i].getm_tempsArrive() - secondeMemoire) >= 0) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        secondeMemoire++;

                        // Si pendant qu'on attend que ce processus soit charge en memoire, le temps ES d'un processus est arrive
                        for (auto& p : ProcessusExecuter) {
                            p.setm_tempsES_Ecoule(p.getm_tempsES_Ecoule() + 1);
                            if (p.getm_tempsES_Ecoule() == p.getm_tempsES()) {
                                p.setm_etat("Execution complete");
                                memoireDispo += p.getm_memoire();

                                auto it = std::find_if(memory.begin(), memory.end(), [&p](const Process& proc) { return proc.getm_PID() == p.getm_PID(); });
                                if (it != memory.end())
                                    memory.erase(it);
                            }
                        }
                    }

                    if (processus[i].getm_memoire() >= memoireDispo) {
                        memory.push_back(processus[i]);
                        found = memory.size() - 1;
                        i++;
                    }else {
                        waiting_list.insert(waiting_list.begin(), processus[i]);
                        i++;
                        continue;
                    }

                }else {
                    // Dans ce cas on a deja executer tous les processus
                    break;
                }
            } else {
                // Ici tous les processus en memoire ont deja ete excuter mais il n'y a pas assez d'espace pour charge un autre processus en memoire
                // Donc je suis oblige d'attendre que le temps d'ES d'un processus se termine pour pouvoir charge un processus de la liste d'attente en memoire
                cout << "Tous le processus charge en memoire ont ete executer par le processeur mais n'ont pas complete leur temps ES." << endl;
                cout << "Et il n'y a plus assez d'espace en memoire pour charger un processus !" << endl;
                cout << "Veuillez attendre qu'un processus complete son temps ES !!!" << endl;

                // Recherche du processus charge en memoire ayant le plus petit temps ES
                int indice_temps_ES = 0;
                for (int j = 0; j < memory.size(); j++) {
                    if ((memory[indice_temps_ES].getm_tempsES() - memory[indice_temps_ES].getm_tempsES_Ecoule()) > (memory[j].getm_tempsES() - memory[j].getm_tempsES_Ecoule()) ){
                        indice_temps_ES = j;
                    }
                }
                while (memory[indice_temps_ES].getm_tempsES() >= memory[indice_temps_ES].getm_tempsES_Ecoule()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    secondeMemoire++;

                    for (int j = i; j < processus.size(); ++j) {
                        if ((processus[j].getm_tempsArrive() == secondeMemoire) && (processus[j].getm_dejaExecuter() == false)) {
                            processus[j].setm_presenceMemoire("Liste d'attente");
                            waiting_list.insert(waiting_list.begin(), processus[j]);
                            i++;
                        }
                    }

                    for (auto& p : ProcessusExecuter) {
                        p.setm_tempsES_Ecoule(p.getm_tempsES_Ecoule() + 1);
                        if (p.getm_tempsES_Ecoule() == p.getm_tempsES()) {
                            p.setm_etat("Execution complete");
                            memoireDispo += p.getm_memoire();

                            auto it = std::find_if(memory.begin(), memory.end(), [&p](const Process& proc) { return proc.getm_PID() == p.getm_PID(); });
                            if (it != memory.end())
                                memory.erase(it);
                            if (!memory.empty()) {
                                chargerEnMemoire(memory, waiting_list, memoireDispo);
                            }
                            // A la prochaine execution, on va donc choisir le processus ayant le plus petit temps CPU parmi ceux qui viennent d'etre charge en memoire
                            continue;
                        }
                    }
                }
            }

        }

         // Simuler l'exécution CPU du processus sélectionné
        affichage (memory[found]);
        int execution_time = std::min(quantum, memory[found].getm_tempsCpu() - memory[found].getm_tempsExecution_Ecoule());
        int temps = 1;

            while (temps <= execution_time) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                memory[found].setm_tempsExecution_Ecoule(memory[found].getm_tempsExecution_Ecoule()+1);
                secondeMemoire++;
                temps++;

                // Charger les processus arrivés en mémoire
                int taille = memory.size();
                for (int j = i; j < processus.size(); j++) {
                    if ((processus[j].getm_tempsArrive() == secondeMemoire) && (processus[j].getm_dejaExecuter() == false)) {
                        if (processus[j].getm_memoire() <= memoireDispo) {
                            processus[j].setm_presenceMemoire("Charge En Memoire");
                            memory.push_back(processus[j]);
                            memoireDispo -= processus[j].getm_memoire();

                        } else {
                            processus[j].setm_presenceMemoire("Liste d'attente");
                            waiting_list.insert(waiting_list.begin(), processus[j]);
                        }
                        i++;
                    }
                }

                // Mettre à jour les processus en attente d'ES
                if (!ProcessusExecuter.empty()) {
                    for (auto& p : ProcessusExecuter) {
                        if ((p.getm_tempsES() - p.getm_tempsES_Ecoule()) > 0){
                            p.setm_tempsES_Ecoule(p.getm_tempsES_Ecoule() + 1);
                            if (p.getm_tempsES_Ecoule() == p.getm_tempsES()) {
                                p.setm_etat("Execution complete");
                                memoireDispo += p.getm_memoire();

                                int pid = memory[found].getm_PID();
                                auto it = std::find_if(memory.begin(), memory.end(), [&p](const Process& proc) { return proc.getm_PID() == p.getm_PID(); });
                                if (it != memory.end()){
                                    memory.erase(it);
                                    found = retrouverIndice (memory,pid);
                                    nbreExecute--;
                                }
                                if (!memory.empty() && !waiting_list.empty()) {
                                    chargerEnMemoire(memory, waiting_list, memoireDispo);
                                }
                            }
                        }
                    }
                }
            }

            if (memory[found].getm_tempsCpu() == memory[found].getm_tempsExecution_Ecoule()) {
                cout << "Processus execute avec succes, il reste en memoire pour " << memory[found].getm_tempsES() << " secondes.\n" << endl;
                memory[found].setm_tempsAttente(secondeMemoire - memory[found].getm_tempsArrive()- memory[found].getm_tempsCpu());
                memory[found].setm_tempsExecution(memory[found].getm_tempsAttente() + memory[found].getm_tempsCpu() + memory[found].getm_tempsES());
                memory[found].setm_dejaExecuter(true);

                ProcessusExecuter.push_back(memory[found]);

                Process &temp = memory[found];
                memory.erase(memory.begin()+ found);
                memory.insert(memory.begin()+nbreExecute, temp);
                nbreExecute++;
            }

        if (found == memory.size()-1) {
            found = nbreExecute;
        } else {
            found++;
        }

    }

    ProcessusExecuter[ProcessusExecuter.size()-1].setm_etat("Execution Complete");

    // Transférer les processus exécutés
    processus.clear();
    processus.insert(processus.end(), ProcessusExecuter.begin(), ProcessusExecuter.end());
}
