#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "parking.h"

typedef struct {
    char id_parking[20];
    char assigned_agent_id[20];
} work;


int addParking(const char *filename, Parking p) {
    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s %d %d %d %d %d %s\n",
                p.id_parking,
                p.id_agent,
                p.adresse,
                p.municipalite,
                p.capacite,
                p.etat,
                p.tarif[0], p.tarif[1], p.tarif[2],
                p.prix);
        fclose(f);
        return 1;
    }
    return 0;
}

int modifyParking(const char *filename, const char *id, Parking p_modify) {
    int found = 0;
    Parking p;
    FILE *f = fopen(filename, "r");
    FILE *f_temp = fopen("temp.txt", "w");

    if (f != NULL && f_temp != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %d %d %s\n",
                p.id_parking,
                p.id_agent,
                p.adresse,
                p.municipalite,
                &p.capacite,
                &p.etat,
                &p.tarif[0], &p.tarif[1], &p.tarif[2],
                p.prix) != EOF) {

            if (strcmp(p.id_parking, id) == 0) {
                fprintf(f_temp, "%s %s %s %s %d %d %d %d %d %s\n",
                        p_modify.id_parking,
                        p_modify.id_agent,
                        p_modify.adresse,
                        p_modify.municipalite,
                        p_modify.capacite,
                        p_modify.etat,
                        p_modify.tarif[0], p_modify.tarif[1], p_modify.tarif[2],
                        p_modify.prix);
                found = 1;
            } else {
                fprintf(f_temp, "%s %s %s %s %d %d %d %d %d %s\n",
                        p.id_parking,
                        p.id_agent,
                        p.adresse,
                        p.municipalite,
                        p.capacite,
                        p.etat,
                        p.tarif[0], p.tarif[1], p.tarif[2],
                        p.prix);
            }
        }
        fclose(f);
        fclose(f_temp);

        if (found) {
            remove(filename);
            rename("temp.txt", filename);
        } else {
            remove("temp.txt");
        }
    }
    return found;
}

Parking showParking(const char *filename, const char *id) {
    Parking p;
    int found = 0;
    FILE *f = fopen(filename, "r");

    if (f != NULL) {
        while (!found && fscanf(f, "%s %s %s %s %d %d %d %d %d %s\n",
                p.id_parking,
                p.id_agent,
                p.adresse,
                p.municipalite,
                &p.capacite,
                &p.etat,
                &p.tarif[0], &p.tarif[1], &p.tarif[2],
                p.prix) != EOF) {

            if (strcmp(p.id_parking, id) == 0) {
                found = 1;
            }
        }
        fclose(f);
    }
    if (!found) {
        strcpy(p.id_parking, "");
    }
    return p;
}

GtkListStore *create_parking_list_store() {
    return gtk_list_store_new(8,
                             G_TYPE_STRING,  // id_parking
                             G_TYPE_STRING,  // name_parking
                             G_TYPE_STRING,
                             G_TYPE_STRING,  // location
                             G_TYPE_INT,     // capacity
                             G_TYPE_INT,     // type_of_parking
                             G_TYPE_STRING,  // pricing (as string)
                             G_TYPE_STRING);  // prix
}

void create_treeview_columns(GtkTreeView *treeview) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column;

    const char *column_titles[] = {
        "ID Parking", "ID_agent", "Municipalite","adresse", "Capacite",
        "Etat", "Tarif", "Prix"
    };

    for (int i = 0; i < 7; i++) {
        column = gtk_tree_view_column_new_with_attributes(column_titles[i],
                                                        renderer, "text", i, NULL);
        gtk_tree_view_append_column(treeview, column);
    }
}

void populate_treeview_with_data(GtkTreeView *treeview, const char *filename) {
    GtkListStore *store = create_parking_list_store();
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        g_print("Error: Could not open the file\n");
        return;
    }

    Parking p;
    GtkTreeIter iter;
    char tarif_str[100];

    while (fscanf(file, "%s %s %s %s %d %d %d %d %d %s\n",
                 p.id_parking,
                 p.id_agent,
                 p.adresse,
                 p.municipalite,
                 &p.capacite,
                 &p.etat,
                 &p.tarif[0], &p.tarif[1], &p.tarif[2],
                 p.prix) != EOF) {

        snprintf(tarif_str, sizeof(tarif_str), "H:%d M:%d A:%d",
                p.tarif[0], p.tarif[1], p.tarif[2]);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                          0, p.id_parking,
                          1, p.id_agent,
                          2, p.adresse,
                          3, p.municipalite,
                          4, p.capacite,
                          5, p.etat,
                          6, tarif_str,
                          7, p.prix,
                          -1);
    }

    fclose(file);
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(store));
    g_object_unref(store);
}
int removeParking(const char *filename, const char *id) {
    int found = 0;
    Parking p;
    FILE *f = fopen(filename, "r");
    FILE *f_temp = fopen("temp.txt", "w");

    if (f != NULL && f_temp != NULL) {
        while (fscanf(f, "%s %s %s %s %d %d %d %d %d %s\n",
                      p.id_parking, p.id_agent, p.adresse, p.municipalite,
                      &p.capacite, &p.etat,
                      &p.tarif[0], &p.tarif[1], &p.tarif[2], p.prix) != EOF) {
            if (strcmp(p.id_parking, id) != 0) {
                fprintf(f_temp, "%s %s %s %s %d %d %d %d %d %s\n",
                        p.id_parking, p.id_agent, p.adresse, p.municipalite,
                        p.capacite, p.etat,
                        p.tarif[0], p.tarif[1], p.tarif[2], p.prix);
            } else {
                printf("Deleting parking ID: %s\n", p.id_parking);
                found = 1;
            }
        }
        fclose(f);
        fclose(f_temp);
        remove(filename);
        rename("temp.txt", filename);
    } else {
        if (f != NULL) fclose(f);
        if (f_temp != NULL) fclose(f_temp);
        printf("Error: Unable to open files.\n");
    }

    return found;
} 

	


void AfficherAgents(char *filename) {
    Agent a;
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        printf("Liste des agents :\n");
        printf("ID\tNom\t\tPrenom\t\tAdresse\t\tGouvernorat\tEmail\t\tNaissance\tSexe\tTarifs\t\tTelephone\n");
        printf("------------------------------------------------------------------------------------------------------------\n");
        while (fscanf(f, "%s %s %s %s %d %s %d %d %d %c %d %d %d\n",
                      a.id, a.nom, a.prenom, a.adresse, &a.governorate, a.email,
                      &a.jour, &a.mois, &a.annee, &a.sexe,
                      &a.pricing[0], &a.pricing[1], &a.phone) != EOF) {
            printf("%s\t%s\t\t%s\t\t%s\t%d\t\t%s\t%d/%d/%d\t%c\t%d-%d\t\t%d\n",
                   a.id, a.nom, a.prenom, a.adresse, a.governorate, a.email,
                   a.jour, a.mois, a.annee, a.sexe,
                   a.pricing[0], a.pricing[1], a.phone);
        }
        fclose(f);
    } else {
        printf("Erreur lors de l'ouverture du fichier des agents.\n");
    }
}


// Affecter un agent à un parking
/*int AffecterAgent(char *filename, int id_parking, Agent agent) {
    FILE *f = fopen(filename, "r");
    char buffer[256];
    int agentAffecte = 0;
    int idAgent, idParking;

    // Vérifier si l'agent est déjà affecté
    if (f != NULL) {
        while (fgets(buffer, sizeof(buffer), f)) {
            if (strstr(buffer, "Agent") && strstr(buffer, "affecté au Parking")) {
                sscanf(buffer, "Agent %d affecté au Parking %d", &idAgent, &idParking);
                if (idAgent == atoi(agent.id)) {
                    agentAffecte = 1;
                    break;
                }
            }
        }
        fclose(f);
    }

    if (agentAffecte) {
        printf("Erreur : Cet agent est déjà affecté à un autre parking.\n");
        return 0;
    }

    // Affecter l'agent au parking
    f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "Agent %s affecté au Parking %d\n", agent.id, id_parking);
        fclose(f);
        return 1;
    }
    return 0;
}*/
/*int supprimer_parking(const char *filename, const char *id) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        return 0;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(f);
        printf("Erreur : Impossible de créer le fichier temporaire.\n");
        return 0;
    }

    Parking p;
    char line[512];
    int found = 0;

    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), f)) {
        // Utilisation de sscanf pour lire la ligne et remplir la structure 'Parking'
        if (sscanf(line, "%49s %49s %99s %99s %d %d %d %d %d %d %d %f %[^\n]",
                   p.id_parking,p.id_agent,p.adresse,p.municipalite,&p.capacite
                   &p.etat, &p.tarif[0], &p.tarif[1], &p.tarif[2],
                   &p.ouvert, &p.ferme, p.prix) == 11) {

            // Vérifier si l'ID correspond
            if (strcmp(p.id_parking, id) == 0) {
                found = 1; // Marquer comme trouvé
                continue;  // Ne pas écrire cette ligne dans le fichier temporaire
            }
        }

        // Écrire les lignes non supprimées dans le fichier temporaire
        fprintf(temp, "%s", line);
    }

    fclose(f);
    fclose(temp);

    if (found) {
        remove(filename);  // Supprimer l'ancien fichier
        rename("temp.txt", filename);  // Renommer le fichier temporaire
        return 1; // Succès
    } else {
        remove("temp.txt");
        printf("Erreur : Parking avec l'ID '%s' introuvable.\n", id);
        return 0; // Échec
    }
}*/
int assignAgentToParking(const char *parking_filename, const char *agents_filename, const char *id_parking, const char *id) {
    // Open and check agents file
    FILE *agents_file = fopen(agents_filename, "r");
    if (agents_file == NULL) {
        printf("Error: Could not open agents file.\n");
        return 0;
    }

    // Check if agent exists and is available
    char line[256];
    int agent_found = 0;
    while (fgets(line, sizeof(line), agents_file)) {
        char current_id[10];
        char name[50];
        char schedule[20];
        int is_assigned;
       
        if (sscanf(line, "%[^,],%[^,],%[^,],%d", current_id, name, schedule, &is_assigned) == 4) {
            if (strcmp(current_id, id) == 0) {
                agent_found = 1;
                if (is_assigned) {
                    printf("Error: Agent %s is already assigned.\n", id);
                    fclose(agents_file);
                    return 0;
                }
                break;
            }
        }
    }
    fclose(agents_file);

    if (!agent_found) {
        printf("Error: Agent %s not found.\n", id);
        return 0;
    }

    // Check if parking ID exists
    FILE *parking_file = fopen(parking_filename, "r");
    if (parking_file == NULL) {
        printf("Error: Could not open parking file.\n");
        return 0;
    }

    int parking_found = 0;
    while (fgets(line, sizeof(line), parking_file)) {
        if (strncmp(line, id_parking, strlen(id_parking)) == 0) {
            parking_found = 1;
            break;
        }
    }
    fclose(parking_file);

    if (!parking_found) {
        printf("Error: Parking ID %s not found.\n", id_parking);
        return 0;
    }

    // Write only parking_id and agent_id to work.txt file
    FILE *work_file = fopen("work.txt", "a");
    if (work_file == NULL) {
        printf("Error: Could not open work file.\n");
        return 0;
    }
    fprintf(work_file, "%s,%s\n", id_parking, id);
    fclose(work_file);

    // Update agent status in agents file
    FILE *agents_read = fopen(agents_filename, "r");
    FILE *agents_temp = fopen("agents_temp.txt", "w");
    if (agents_read == NULL || agents_temp == NULL) {
        if (agents_read) fclose(agents_read);
        if (agents_temp) fclose(agents_temp);
        printf("Error: Could not update agent status.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), agents_read)) {
        char current_id[10];
        char name[50];
        char schedule[20];
        int is_assigned;
       
        if (sscanf(line, "%[^,],%[^,],%[^,],%d", current_id, name, schedule, &is_assigned) == 4) {
            if (strcmp(current_id, id) == 0) {
                fprintf(agents_temp, "%s,%s,%s,1\n", current_id, name, schedule);
            } else {
                fputs(line, agents_temp);
            }
        }
    }
    fclose(agents_read);
    fclose(agents_temp);

    if (remove(agents_filename) != 0 || rename("agents_temp.txt", agents_filename) != 0) {
        printf("Error: Could not update agent status.\n");
        return 0;
    }

    return 1;
}
GtkListStore *create_work_list_store() {
    return gtk_list_store_new(2,  // Total number of columns
                              G_TYPE_STRING,  // Parking ID (string)
                              G_TYPE_STRING);  // Assigned Agent ID (string)
                              }



// Create columns for the TreeView
void create_treeview1_columns(GtkTreeView *treeview) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Column 1: Parking ID
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Parking ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(treeview, column);

    // Column 2: Assigned Agent ID
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Agent ID", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(treeview, column);
}

// Function to populate the TreeView with data from the file
void populate_treeview1_with_data(GtkTreeView *treeview, const char *filename) {
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

    if (store == NULL) {
        g_print("Error: No model associated with the TreeView\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        g_print("Error: Could not open the file\n");
        return;
    }

    GtkTreeIter iter;
    work parking_data; // Correct structure name
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Parse the line assuming the format: p1,AG005
        if (sscanf(line, "%[^,],%[^,]",
                   parking_data.id_parking,
                   parking_data.assigned_agent_id) == 2) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, parking_data.id_parking,
                               1, parking_data.assigned_agent_id,
                               -1);
        }
    }

    fclose(file);
}
	


