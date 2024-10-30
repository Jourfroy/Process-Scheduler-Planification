#include "process.h"

#include <iostream>
#include <string>
#include <iomanip>  // Pour std::setw()
#include <random>   // Pour std::mt19937, std::uniform_int_distribution
#include <ctime>    // Pour std::time




using namespace std ;

//Constructeurs

Process::Process (int PID, int priorite, std::string name, int tempsCpu, int tempsES, double memoire, int tempsArrive) {
    m_PID = PID;
    m_priorite = priorite;
    m_name = name ;
    m_tempsCpu = tempsCpu;
    m_tempsES = tempsES ;
    m_memoire = memoire;
    m_tempsArrive = tempsArrive ;
}


Process::Process (){
    m_PID = 0;
    m_priorite = 0;
    m_name = "" ;
    m_tempsCpu = 0;
    m_tempsES = 0 ;
    m_memoire = 0;
    m_tempsArrive = 0;
}


//Setters

void Process::setm_etat (std::string etat) {
    m_etat = etat;
}

void Process::setm_memoire (double memoire ){
    m_memoire = memoire;
}

void Process::setm_name (std::string name) {
    m_name = name ;
}

void Process::setm_PID (int PID) {
    m_PID = PID;
}

void Process::setm_priorite(int priorite) {
    m_priorite = priorite;
}

void Process::setm_tempsAttente (int tempsAttente) {
    m_tempsAttente = tempsAttente ;
}

void Process::setm_tempsCpu (int tempsCpu) {
    m_tempsCpu = tempsCpu ;
}

void Process::setm_tempsES (int tempsES) {
    m_tempsES = tempsES ;
}

void Process::setm_tempsExecution (int tempsExecution) {
    m_tempsExecution = tempsExecution ;
}

void Process::setm_tempsArrive (int tempsArrive) {
    m_tempsArrive = tempsArrive;
}

void Process::setm_presenceMemoire (std::string presenceMemoire) {
    m_presenceMemoire = presenceMemoire;
}

void Process::setm_tempsES_Ecoule (int tempsES_Ecoule) {
    m_tempsES_Ecoule = tempsES_Ecoule;
}

void Process::setm_dejaExecuter (bool dejaExecuter) {
    m_dejaExecuter = dejaExecuter;
}

//Getters

double Process::getm_memoire ()const{
    return m_memoire;
}

std::string Process::getm_name () const{
    return m_name;
}

int Process::getm_PID () const{
    return m_PID;
}

int Process::getm_priorite ()const{
    return m_priorite;
}

int Process::getm_tempsAttente ()const{
    return m_tempsAttente;
}

int Process::getm_tempsCpu ()const{
    return m_tempsCpu;
}

int Process::getm_tempsES () const{
    return m_tempsES;
}

int Process::getm_tempsExecution ()const {
    return m_tempsExecution;
}

std::string Process::getm_etat ()const {
    return m_etat ;
}

int Process::getm_tempsArrive() const {
    return m_tempsArrive;
}

std::string Process::getm_presenceMemoire () const {
    return m_presenceMemoire;
}

int Process::getm_tempsES_Ecoule () const {
    return m_tempsES_Ecoule;
}

bool Process::getm_dejaExecuter () const {
    return m_dejaExecuter;
}

//Methodes

void Process::displayInfo() {
    // Largeur des colonnes
    const int largeurColonne = 20;

    // Affichage des données avec un format en tableau
    std::cout << std::setw(largeurColonne) << m_tempsArrive
              << std::setw(largeurColonne) << m_PID
              << std::setw(largeurColonne) << m_name
              << std::setw(largeurColonne) << m_tempsCpu
              << std::setw(largeurColonne) << m_tempsES
              << std::setw(largeurColonne) << m_memoire
              << std::endl;
}

void Process::displayInfo ( int nombreColonne){
    // Largeur des colonnes
    const int largeurColonne = 20;

    // Affichage des données avec un format en tableau
    std::cout << std::setw(largeurColonne) << m_tempsArrive
              << std::setw(largeurColonne) << m_PID
              << std::setw(largeurColonne) << m_name
              << std::setw(largeurColonne) << m_tempsCpu
              << std::setw(largeurColonne) << m_tempsES
              << std::setw(largeurColonne) << m_memoire
              << std::setw(largeurColonne) << m_tempsExecution
              << std::setw(largeurColonne) << m_tempsAttente
              << std::setw(largeurColonne) << m_etat
              << std::endl;
}


