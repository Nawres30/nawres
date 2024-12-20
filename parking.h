#ifndef PARKING_H
#define PARKING_H

#include <stdio.h>  // Pour la gestion de fichiers
#include <gtk/gtk.h>
typedef struct {
    char id_parking[50];
    char id_agent[50];
    char adresse[50];
    char municipalite[100];
    int capacite;
    int etat;
    int tarif[3];
    char prix[50];
} Parking;
typedef struct{
    char id[20];
    char nom[20];
    char prenom[20];
    char adresse[20];
    int governorate;
    char email[20];
    int jour;
    int mois;
    int annee;
    char sexe;
    int pricing [2];
    int phone;
}Agent;
typedef struct {
    int id;                      // Identifiant
    char nom[50];                // Nom
    char categorie[50];          // Catégorie
    char description[200];       // Description
    char telephone[15];          // Téléphone
} reserv;

// Déclarations des fonctions
int addParking(const char *filename, Parking p);
int modifyParking(const char *filename, const char *id, Parking p_modify);
int removeParking(const char *filename, const char *id);
Parking showParking(const char *filename, const char *id); // Utiliser const pour éviter toute modification du fichier
void AfficherParkings(char *filename);
int RechercherAgent(char *filename, char *idRecherche);
void AfficherAgents(char *filename);
/*int AffecterAgent(char *filename, int id, Agent agent);*/

void insert_parking_data(GtkListStore *store, Parking *p);
Parking *get_parking_by_id(const char *filename, const char *id);
void create_treeview_columns(GtkTreeView *treeview);
GtkListStore *create_parking_list_store();
void populate_treeview_with_data(GtkTreeView *treeview, const char *filename);
int supprimer_parking(const char *filename, const char *id) ;
reserv chercher_reserv(const char *filename, int id) ;
int assignAgentToParking(const char *parking_filename, const char *agents_filename, const char *parking_id, const char *agent_id);
void populate_treeview1_with_data(GtkTreeView *treeview, const char *filename);
void create_treeview1_columns(GtkTreeView *treeview);
GtkListStore *create_work_list_store();
#endif

