
#include "Process.h"
#include "FCFS.h"
#include "SJF.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>  // Pour std::setw()
#include <random>   // Pour std::mt19937, std::uniform_int_distribution
#include <ctime>    // Pour std::time



using namespace std;


std::string generateurNom (int i) {
    return "Process_"+to_string(i);
}


void afficherEnTete() {
    const int largeurColonne = 20;

    // Affichage de l'en-tête
    std::cout << std::setw(largeurColonne) << "Arrivee"
              << std::setw(largeurColonne) << "PID"
              << std::setw(largeurColonne) << "Priorite"
              << std::setw(largeurColonne) << "Nom"
              << std::setw(largeurColonne) << "Temps Processeur"
              << std::setw(largeurColonne) << "Temps E/S"
              << std::setw(largeurColonne) << "Memoire (en %)"
              << std::endl;

    std::cout << std::string(largeurColonne * 7, '-') << std::endl; // Séparateur
}


void afficherEnTete1 (int nombreColonne) {
   const int largeurColonne = 20;

    // Affichage de l'en-tête
    std::cout << std::setw(largeurColonne) << "Arrivee"
              << std::setw(largeurColonne) << "PID"
              << std::setw(largeurColonne) << "Nom"
              << std::setw(largeurColonne) << "Temps Processeur"
              << std::setw(largeurColonne) << "Temps E/S"
              << std::setw(largeurColonne) << "Memoire (en %)"
              << std::setw(largeurColonne) << "Temps Execution"
              << std::setw(largeurColonne) << "Temps Attente"
              <<std::setw (largeurColonne) << "Etat"
              << std::endl;

    std::cout << std::string(largeurColonne * nombreColonne, '-') << std::endl; // Séparateur
}

void afficherEnTete2 (int nombreColonne) {
   const int largeurColonne = 20;

    // Affichage de l'en-tête
    std::cout << std::setw(largeurColonne) << "Arrivee"
              << std::setw(largeurColonne) << "PID"
              << std::setw(largeurColonne) << "Priorite"
              << std::setw(largeurColonne) << "Nom"
              << std::setw(largeurColonne) << "Temps Processeur"
              << std::setw(largeurColonne) << "Temps E/S"
              << std::setw(largeurColonne) << "Memoire (en %)"
              << std::setw(largeurColonne) << "Temps Execution"
              << std::setw(largeurColonne) << "Temps Attente"
              << std::setw (largeurColonne) << "Etat"
              << std::endl;

    std::cout << std::string(largeurColonne * nombreColonne, '-') << std::endl; // Séparateur
}


int main()
{
    cout << "Veuillez entrer le nombre de processus que vous voullez creer :" << ends;
    int nbreProcessus ;
    cin >> nbreProcessus;

    cout << "Voullez vous que les caracteristiques de ces processus soit generer automatiquement ou voullez vous les entrer manuellement ? "<< endl ;
    cout << "Entrer 1 si vous voulez que tout se passe automatiquent et 2 si vous voullez entrer ces caracteristiques manuellement :" <<ends;
    int genre ;
    cin >> genre ;

    std::vector<Process> processus;
    std::vector<Process> processusCree;
    std::vector<Process> processusExecute;

    if (genre == 1) {

        vector <int> Pid ;  //Contient tous les PID deja attribues


        // Initialiser la seed avec l'horloge système
        std::mt19937 gen(static_cast<unsigned int>(std::time(0)));                                 // Générateur de nombres aléatoires
        std::uniform_int_distribution<> pidDist(1, 1000);       // Distribution pour les PID
        std::uniform_real_distribution<> memoryDist(0.1, 40.0);// Distribution pour l'utilisation de la mémoire, Un processus ne peut pas utiliser pls de 40% de la memoire totale
        std::uniform_int_distribution<> priorityDist(1, 3);    // Distribution pour la priorité
        std::uniform_int_distribution<> arrivalDist(1, 50);    // Distribution pour le temps d'arrivée
        std::uniform_int_distribution<> CPUDist (1, 100);
        std::uniform_int_distribution<> ESDist (1, 50);



        for (int i = 0; i < nbreProcessus; i++) {
            int pid ;
            pid = pidDist(gen);

            bool found1 = false;

            for (int i = 0; i < Pid.size(); i++) {
                if (Pid[i] == pid) {
                    found1 = true;
                }
            }

            while (found1 == true) {
                pid = pidDist(gen);
                for (int i = 0; i < Pid.size(); i++) {
                    if (Pid[i] == pid){
                        found1 = true;
                    }
                }
            }
            Pid.push_back(pid);

            std::string name = generateurNom (i+1);
            double memoire = memoryDist(gen);
            int priorite = priorityDist (gen) ;
            int tempsCpu = CPUDist(gen);
            int tempsES = ESDist(gen);
            int instantArrive = arrivalDist(gen) ;

            processus.emplace_back(pid, priorite, name, tempsCpu, tempsES, memoire, instantArrive);


        }

        //Decalge des temps d'arrive par rapport au plus petit temps generer
        //Recherche du plus petit temps generer
        int tempsMin = processus[0].getm_tempsArrive();
        for (int i = 0; i < processus.size(); i++) {
            if (processus[i].getm_tempsArrive() < tempsMin){
                tempsMin = processus[i].getm_tempsArrive();
            }
        }
        //Decalage effectif
        for (int i = 0; i < processus.size(); i++) {
            processus[i].setm_tempsArrive(processus[i].getm_tempsArrive() - tempsMin +1);
        }

    } else if (genre == 2) {

        cout << "Vous allez devoir entrer successivement : \n-->Le PID (Qui est un entier compris entre 1 et 1000)\n-->Le nom (Qui est une chaine de caracteres)\n-->L'instant d'arrive (Qui est un entier compris entre 1 et 50)"<< endl;
        cout << "-->Le niveau de priorite du processus (Qui est defini sur une echelle de 1 a 3)\n-->Le temps processeur (Qui est un entier compris entre 1 et 100)-->Le temps d'entree/sortie du processus (Qui est aussi un entier compris entre 1 et 50)"<<endl;
        cout << "-->L'espace memoire dont ce processus aura besoin (Qui est en pourcentage)" << endl;

        std::vector <int> Pid ;  //Contient tous les PID deja attribues

        for (int i = 0; i < nbreProcessus; i++) {

            cout << "---------------Processus_"<< i+1<< "------------------"<<endl;

            // Recuperation et test du PID
            cout << "Entrer le PID du processus numero_" << i+1 <<endl;
            int pid;
            cin >> pid;
            while (pid <= 0 || pid >1000){
                cout << "Veuillez entrer un PID compris entre 1 et 1000" << endl;
                cin >> pid;
            }

            //Verification si le PID generer est deja attribue
            bool trouve = false;
            for (int i = 0; i < Pid.size(); i++) {
                if (Pid[i] == pid){
                    trouve = true;
                }
            }
            while (trouve == true) {
                cout << "Ce PID est deja attribue !!\n Veuillez entrer un autre PID " <<endl;
                cin >> pid;
                while (pid <= 0 || pid >1000){
                    cout << "Veuillez entrer un PID compris entre 1 et 1000" << endl;
                    cin >> pid;
                }
                for (int i = 0; i < Pid.size(); i++) {
                    if (Pid[i] == pid){
                        trouve = true;
                    }
                }
            }
            Pid.push_back(pid);


            //Recuperation du nom du processus
            cout << "Entrer le nom du processus numero_"<< i+1 << endl;
            std::string name;
            cin >> name;


            //Recuperation et test de l'instant d'arrive du processus
            cout << "Entrer l'instant d'arrive du processus numero_" << i+1<< endl;
            int instantArrive;
            cin >> instantArrive;
            while (instantArrive <= 0 || instantArrive > 50){
                cout << "Veuillez entrer un instant d'arrive du processus compris entre 1 et 50" << endl;
                cin >> instantArrive;
            }

            //Recuperation et test du niveau de priorite du processus
            cout << "Veuillez entrer le niveau de priorite du processus numero_" << i+1<<endl;
            int priorite;
            cin >> priorite;
            while (priorite <= 0 || priorite > 3) {
                cout << "Veuillez entrer un entier compris entre 1 et 3"<< endl ;
                cin >> priorite;
            }

            //Recuperation et test du temps CPU
            cout << "Entrer le temps processeur du processus numero_" << i+1<<endl;
            int tempsCPU;
            cin >> tempsCPU;
            while (tempsCPU <=0 || tempsCPU >100) {
                cout << "Veuillez entrer un entier compris entre 1 et 100" <<endl;
                cin >> tempsCPU;
            }

            //Recuperation et test du temps entree/sortie
            cout << "Entrer le temps entree/sortie du processus numero_" << i+1<<endl;
            int tempsES;
            cin >> tempsES;
            while (tempsES <=0 || tempsES >50) {
                cout << "Veuillez entrer un entier compris entre 1 et 50" <<endl;
                cin >> tempsES;
            }

            //Recuperation de l'espace memoire necessaire au processus pour sonexecution
            cout <<"Entrer en pourcentage l'espace memoire necessaire au processus numero_"<<i+1<< "pour son execution "<<endl;
            double memoire;

            cin >> memoire ;
            while (memoire <= 0.0 || memoire > 40.0) {
                cout << "Un processus ne peut pas utiliser plus de 40.0% de la memoire totale !!!" <<endl;
                cout << "Veuillez entrer un pourcentage !!!" <<endl;
                cin >> memoire ;
            }


            processus.emplace_back(pid, priorite, name, tempsCPU, tempsES, memoire, instantArrive);

        }

        //Decalge des temps d'arrive par rapport au plus petit temps generer
        //Recherche du plus petit temps generer
        int tempsMin = 0;
        for (int i = 0; i < processus.size(); i++) {
            if (processus[i].getm_tempsArrive() < tempsMin){
                tempsMin = processus[i].getm_tempsArrive();
            }
        }
        //Decalage effectif
        for (int i = 0; i < processus.size(); i++) {
            processus[i].setm_tempsArrive(processus[i].getm_tempsArrive() - tempsMin +1);
        }

    } else {
        cout << "Veuillez choisir 1 ou 2" << endl;
    }


    if (genre == 1 || genre == 2){

        cout << "Voici la liste des processus  !!!" << endl;

        afficherEnTete();
        for (Process p :processus ){
            p.displayInfo();
        }


        cout << "Voulez-vous executer ces processus suivant quel algorithme d'ordonnancement ??" << endl;
        cout << "--> Entrer 1 pour choisir First Come First Served (FCFS):" << endl;
        cout << "--> Entrer 2 pour choisir Short Job First (SJF) :\n" << endl;
        int choix ;
        cin >> choix;

        if (choix == 1) {
            cout << "\nListe des processus trie selon l'algorithme d'ordonnancement Fisrt Come Fisrt Served !!!\n"<<endl;
            algorithme_FCFS_NonPreemptif (processus);
            afficherEnTete1(9);
            for (Process p :processus ){
                p.displayInfo1(9);
            }
        } else if (choix == 2) {
            cout << "Voulez-vous appliquer l'algorithme preemptif ou non preemptif ?" << endl;
            cout << "Entrer 1 pour choisir l'algorithme nonpreemptif et 2 pour choisir la version preemptive" << endl;
            cin >>  choix;
            if (choix == 1) {
                cout << "\nListe des processus trie selon l'algorithme d'ordonnancement Short Job First non preemptif !!!\n"<<endl;
                algorithme_SJF_NonPreemptif (processus);
                afficherEnTete1(9);
                for (Process p :processus ){
                    p.displayInfo1(9);
                }
            } else if (choix == 2) {
                cout << "\nListe des processus trie selon l'algorithme d'ordonnancement Short Job First preemptif !!!\n"<<endl;
                algorithme_SJF_Preemptif (processus);
                afficherEnTete2(10);
                for (Process p :processus ){
                    p.displayInfo2(10);
                }
            }
        }else if (choix == 3) {
            cout << "Entrer le quantum : " << ends;
            int quantum;
            cin >> quantum;
            cout << "Voulez vous la version preemptive ou non preemptive ?" << endl;
            cout << "Entrer 1 pour choisir l'algorithme nonpreemptif et 2 pour choisir la version preemptive" << endl;
            cin >>  choix;
            if (choix == 1) {
                cout << "\nListe des processus trie selon l'algorithme d'ordonnancement  Round Robin (RR) version non preemptive !!!\n"<<endl;
                algorithme_RR_NonPreemptif (processus, quantum);
                afficherEnTete2(10);
                for (Process p :processus ){
                    p.displayInfo2(10);
                }
            } else if (choix == 2) {
                cout << "\nListe des processus trie selon l'algorithme d'ordonnancement  Round Robin (RR) version preemptive !!!\n"<<endl;
                algorithme_RR_Preemptif (processus, quantum);
                afficherEnTete2(10);
                for (Process p :processus ){
                    p.displayInfo2(10);
                }
            }
        }
    }

    return 0;
}
