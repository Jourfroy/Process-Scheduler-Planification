#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>


class Process {

private :
    int m_PID;
    std::string m_name ;
    std::string m_etat;   // Cet attribut contient l'etat du processus
    int m_tempsCpu;
    int m_tempsES;
    double m_memoire;      // Cette variable contient l'espace (en pourcentage) dont a besoin le processus pour s'executer
    int m_priorite;       //Cet attribut contient le niveau de priorite du processus sur une echelle de 1 a 3
    int m_tempsExecution; //cet attribut contient le temps qu'a mis le processus pour s'executer
    int m_tempsAttente ;  // Cet attribut contient la temps qu'a mis le processus pour etre executer
    int m_tempsArrive;
    std::string m_presenceMemoire ;   //Cette attribut permet de savoir si un objet de type Process et present en memoire ou non
    int m_tempsES_Ecoule; // Cet attribut compte le temps ecoule apres l'execution du processus
    bool m_dejaExecuter = false;


public :

    //Constructeurs

    Process (int PID, int priorite,std::string name, int tempsCpu, int tempsES, double memoire, int instantArrive);
    Process ();


    // Setters

    void setm_PID(int PID) ;
    void setm_name(std::string name) ;
    void setm_etat (std::string etat) ;
    void setm_tempsCpu (int tempsCpu) ;
    void setm_tempsES (int tempsES) ;
    void setm_memoire (double memoire);
    void setm_priorite (int priorite) ;
    void setm_tempsExecution (int tempsExecution) ;
    void setm_tempsAttente (int tempsAttente)  ;
    void setm_tempsArrive (int instantArrive);
    void setm_presenceMemoire (std::string presenceMemoire);
    void setm_tempsES_Ecoule (int tempsES_Ecoule);
    void setm_dejaExecuter (bool dejaExecuter);

    //Getters

    int getm_PID () const;
    std::string getm_name () const ;
    std::string getm_etat () const ;
    int getm_tempsCpu () const ;
    int getm_tempsES () const ;
    double getm_memoire () const ;
    int getm_priorite () const ;
    int getm_tempsExecution () const ;
    int getm_tempsAttente () const ;
    int getm_tempsArrive () const;
    std::string getm_presenceMemoire () const;
    int getm_tempsES_Ecoule () const;
    bool getm_dejaExecuter () const;



    //Methodes

    void displayInfo ();
    void displayInfo (int nombreColonne) ;


};


//Generateur d'objet de type Process avec dans attributs aleatoire
void generateProcess_aleatoirement (std::vector <Process> processus, std::vector <int> Pid, double& memoireDispo) ;


#endif // PROCESS_H_INCLUDED
