#include "Process.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

bool compareTempsArrive(const Process& P1, const Process& P2) {
    return P1.getm_tempsArrive() < P2.getm_tempsArrive();
}

// Fonction pour charger les processus de la liste d'attente en mémoire
void chargerEnMemoire(std::vector<Process>& memory, std::vector<Process>& waiting_list, double& memoireDispo) {
    for (int j = waiting_list.size() - 1; j >= 0; --j) {  // Boucle inversée pour éviter les problèmes d'indices
        if (waiting_list[j].getm_memoire() <= memoireDispo) {
            memoireDispo -= waiting_list[j].getm_memoire();
            waiting_list[j].setm_presenceMemoire("Charge En Memoire");
            memory.push_back(waiting_list[j]);
            waiting_list.erase(waiting_list.begin() + j);
        }
    }
}

int retrouverIndice(std::vector<Process>& processus, int PID_x) {
    auto it = std::find_if(processus.begin(), processus.end(),
                           [PID_x](const Process& p) { return p.getm_PID() == PID_x; });
    if (it != processus.end()) {
        return std::distance(processus.begin(), it);
    }
    return -1; // Renvoie -1 si le processus n'a pas été trouvé
}

void affichage (Process &p) {
    cout << "------------------------------------------------" << endl;
    cout << "Execution du processus avec : " << endl;
    cout << "--> Temps d'arrivee : " << p.getm_tempsArrive() << " secondes" << endl;
    cout << "--> Temps processeur : " << p.getm_tempsCpu() << " secondes" << endl;
    cout << "--> Priorite : " << p.getm_priorite() << endl;
    cout << "--> PID : " << p.getm_PID() << endl;
    cout << "--> Nom : " << p.getm_name() << endl;
}


void algorithme_SJF_NonPreemptif(std::vector<Process>& processus) {
    std::sort(processus.begin(), processus.end(), compareTempsArrive);
    processus[0].setm_presenceMemoire("Charge En Memoire");

    double memoireDispo = 100.0;
    int secondeMemoire = 1;
    std::vector<Process> memory, waiting_list, ProcessusExecuter;
    memory.push_back(processus[0]);
    memoireDispo -= memory[0].getm_memoire();
    int i = 1;  //Cette variable m'indique quels processus j'ai deja charge en memoire

    while (!memory.empty() || !waiting_list.empty() ) {
        int secondeCPU = 0;

        if (!memory.empty()) {

            // Trouver le processus avec le plus petit temps processeur en mémoire
            int found = -1;
            for (int j = 0; j < memory.size(); ++j) {
                if ((memory[j].getm_dejaExecuter() == false) && ((found == -1) || (memory[j].getm_tempsCpu() < memory[found].getm_tempsCpu()))) {
                    found = j;
                }
            }

            if (found == -1) {
                // Tous les processus en mémoire ont déjà été exécutés

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
                    } else {
                        // Dans ce cas on a deja executer tous les processus
                        break;
                    }

                } else {
                    // Ici tous les processus en memoire ont deja ete excuter mais il n'y a pas assez d'espace pour charge un autre processus en memoire
                    // Donc je suis oblige d'attendre que le temps d'ES d'un processus se termine pour pouvoir charge un processus de la liste d'attente en memoire
                    cout << "Tous le processus charge en memoire ont ete executer par le processeur mais n'ont pas completer leur temps ES." << endl;
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
            cout << "-------------------------------------------------------------------------" << endl;
            cout << "Execution du processus avec : " << endl;
            cout << "--> Temps d'arrivee : " << memory[found].getm_tempsArrive() << " secondes" << endl;
            cout << "--> Temps processeur : " << memory[found].getm_tempsCpu() << " secondes" << endl;
            cout << "--> PID : " << memory[found].getm_PID() << endl;
            cout << "--> Nom : " << memory[found].getm_name() << endl;


            memory[found].setm_tempsAttente(secondeMemoire - memory[found].getm_tempsArrive());
            memory[found].setm_dejaExecuter(true);


            while (secondeCPU <= memory[found].getm_tempsCpu()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                secondeCPU++;
                secondeMemoire++;

                // Charger les processus arrivés en mémoire
                for (int j = i; j < processus.size(); ++j) {
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
                for (auto& p : ProcessusExecuter) {
                    p.setm_tempsES_Ecoule(p.getm_tempsES_Ecoule() + 1);
                    if (p.getm_tempsES_Ecoule() == p.getm_tempsES()) {
                        p.setm_etat("Execution complete");
                        memoireDispo += p.getm_memoire();

                        int pid = memory[found].getm_PID();
                        auto it = std::find_if(memory.begin(), memory.end(), [&p](const Process& proc) { return proc.getm_PID() == p.getm_PID(); });
                        if (it != memory.end())
                            memory.erase(it);
                        if (!memory.empty()) {
                            chargerEnMemoire(memory, waiting_list, memoireDispo);
                        }
                        found = retrouverIndice (memory,pid);
                    }
                }
            }
            cout << "Processus execute avec succes, il reste en memoire pour " << memory[found].getm_tempsES() << " secondes.\n" << endl;

            memory[found].setm_tempsExecution(memory[found].getm_tempsAttente() + memory[found].getm_tempsCpu()  + memory[found].getm_tempsES());

            ProcessusExecuter.push_back(memory[found]);


        }
    }

    ProcessusExecuter[ProcessusExecuter.size()-1].setm_etat("Execution Complete");
    // Transférer les processus exécutés
    processus.clear();
    processus.insert(processus.end(), ProcessusExecuter.begin(), ProcessusExecuter.end());
}

void algorithme_SJF_Preemptif(std::vector<Process>& processus) {
    std::sort(processus.begin(), processus.end(), compareTempsArrive);
    processus[0].setm_presenceMemoire("Charge En Memoire");

    double memoireDispo = 100.0;
    int secondeMemoire = 0;
    std::vector<Process> memory, waiting_list, ProcessusExecuter;
    memory.push_back(processus[0]);
    memoireDispo -= memory[0].getm_memoire();
    int i = 1;

    while (!memory.empty() || !waiting_list.empty()) {

        int found = -1;
        for (int j = 0; j < memory.size(); j++) {
            if ((memory[j].getm_tempsCpu() > memory[j].getm_tempsExecution_Ecoule()) && ((found == -1) || (memory[j].getm_priorite() > memory[found].getm_priorite()))) {
                found = j;
            }
        }

        if (found == -1) {
            // Tous les processus en mémoire ont déjà été exécutés
            cout << "Tous les processus en memoire ont deja ete executes !!!" << endl;
            if (waiting_list.empty()){
                if (i < processus.size()) {
                    // Ici tous les processus sont deja executes et la liste d'attente est vide, donc il faut patienter qu'un processus soit charge en memoire et que le temps d'entree/sortie d'un processus arrive
                    cout << "La liste d'attente est vide !!!" << endl;
                    cout << "En attente du processus " << processus[i].getm_name() << " pendant " << (processus[i].getm_tempsArrive() - secondeMemoire) << " secondes." << endl;
                    while (processus[i+1].getm_tempsArrive() - secondeMemoire) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        secondeMemoire++;

                        // Si pendant qu'on attend que ce processus soit charge en memoire, le temps ES d'un processus est arrive
                        std::vector <int> Asupprimer;   // Ce vecteur contient les differents indices des processus qui ont enpiser leur temps ES
                        int j = 0;
                        while (j < memory.size()) {
                            memory[j].setm_tempsES_Ecoule (memory[j].getm_tempsES_Ecoule() + 1);
                            if ((memory[j].getm_tempsES() - memory[j].getm_tempsES_Ecoule()) == 0) {
                                memory.erase(memory.begin() + j);
                            }
                            j++;
                        }
                        if (secondeMemoire == processus[i].getm_tempsArrive()) {
                            if (memoireDispo >= processus[i].getm_memoire()) {
                                memory.push_back (processus[i]);
                                cout << "Ce processus est charge en memoire !!!" << endl;
                                found = memory.size()-1;
                            } else {
                                waiting_list.insert(waiting_list.begin(), processus[i]);
                                cout << "Ce processus est dans la liste d'attente !!!" << endl;
                                continue;
                            }
                        }
                    }
                    i++;
                }else {
                    // Dans ce cas on a deja executer tous les processus
                    break;
                }
            } else {
                // Ici tous les processus en memoire ont deja ete excuter mais il n'y a pas assez d'espace pour charge un autre processus en memoire
                // Donc je suis oblige d'attendre que le temps d'ES d'un processus se termine pour pouvoir charge un processus de la liste d'attente en memoire
                cout << "\nLa liste d'attente n'est pas vide, donc la memoire est pleine et tous les processus en memoire n'ont pas completer leur temps Entree/Sortie !!!" << endl;
                cout << "Veuillez attendre qu'un processus complete son temps Entree/Sortie !!!\n" << endl;

                // Recherche du processus charge en memoire ayant le plus petit temps ES
                int indice_temps_ES = 0;
                for (int j = 0; j < memory.size(); j++) {
                    if ((memory[indice_temps_ES].getm_tempsES() - memory[indice_temps_ES].getm_tempsES_Ecoule()) > (memory[j].getm_tempsES() - memory[j].getm_tempsES_Ecoule()) ){
                        indice_temps_ES = j;
                    }
                }
                while ((memory[indice_temps_ES].getm_tempsES() - memory[indice_temps_ES].getm_tempsES_Ecoule()) > 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    secondeMemoire++;
                    memory[indice_temps_ES].setm_tempsES_Ecoule(memory[indice_temps_ES].getm_tempsES_Ecoule() + 1);
                }
                memoireDispo += memory[indice_temps_ES].getm_memoire();
                memory.erase(memory.begin() + indice_temps_ES);
                chargerEnMemoire(memory, waiting_list, memoireDispo);

                found = memory.size()-1;
            }

        }

         // Simuler l'exécution CPU du processus sélectionné
        affichage (memory[found]);



            while (memory[found].getm_tempsCpu() > memory[found].getm_tempsExecution_Ecoule()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                memory[found].setm_tempsExecution_Ecoule(memory[found].getm_tempsExecution_Ecoule()+1);
                secondeMemoire++;

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
                if (memory.size() > taille){
                    int priorite = taille;
                    for (int j = taille; j < memory.size(); j++) {
                        // Recherche du processus le plus prioritaire parmi ceux qui viennent d'etre charge en memoire
                        if (memory[priorite].getm_priorite() < memory[j].getm_priorite()) {
                            priorite = j;
                        }
                    }
                    if (memory[priorite].getm_priorite() > memory[found].getm_priorite()) {
                        cout << "Ce processus a ete preempte lorsqu'il avait effectuer " << memory[found].getm_tempsExecution_Ecoule() << " secondes !!!\n" << endl;
                        found = priorite;
                        affichage(memory[found]);

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
                                }
                                if (!memory.empty()) {
                                    chargerEnMemoire(memory, waiting_list, memoireDispo);

                                    // Il faut verifier le niveau de priorite du nouveau processus qui est charge en memoire
                                    if (memory[memory.size()-1].getm_priorite() > memory[found].getm_priorite()) {
                                        cout << "Ce processus a ete preempte lorsqu'il avait effectuer " << memory[found].getm_tempsExecution_Ecoule() << " secondes !!!\n" << endl;
                                        found = memory.size()-1;
                                        affichage(memory[found]);
                                    }
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
            }

    }

    ProcessusExecuter[ProcessusExecuter.size()-1].setm_etat("Execution Complete");
    // Transférer les processus exécutés
    processus.clear();
    processus.insert(processus.end(), ProcessusExecuter.begin(), ProcessusExecuter.end());
}
