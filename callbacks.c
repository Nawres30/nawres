#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "parking.h"


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button38_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button55_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button39_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview1");

    // Get the model associated with the treeview
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

    if (model == NULL) {
        // If no model exists, create a new one and set it
        GtkListStore *store = create_parking_list_store();
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store); // Release reference as TreeView holds its own
    } else {
        // If a model already exists, clear it
        gtk_list_store_clear(GTK_LIST_STORE(model));
    }

    // Ensure columns are created only once
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        create_treeview_columns(GTK_TREE_VIEW(treeview));
    }

    // Populate the treeview with data
    populate_treeview_with_data(GTK_TREE_VIEW(treeview), "parking.txt");
}


void on_button14_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *id_parking, *municipalite, *adresse, *etat_ouvert, *etat_ferme;
    GtkWidget *id_agent, *tarif_heure, *tarif_mensuel, *tarif_annuel, *capacite, *prix;
    GtkWidget *ajout_label;
    Parking p;
    
    // Initialize structure to prevent undefined behavior
    memset(&p, 0, sizeof(Parking));
    
    // Lookup all widgets
    id_parking = lookup_widget(GTK_WIDGET(button), "entry6");
    prix = lookup_widget(GTK_WIDGET(button), "entry51");
    municipalite = lookup_widget(GTK_WIDGET(button), "comboboxentry2");
    adresse = lookup_widget(GTK_WIDGET(button), "entry7");
    etat_ouvert = lookup_widget(GTK_WIDGET(button), "radiobutton3");
    etat_ferme = lookup_widget(GTK_WIDGET(button), "radiobutton4");
    id_agent = lookup_widget(GTK_WIDGET(button), "entry52");
    tarif_heure = lookup_widget(GTK_WIDGET(button), "checkbutton40");
    tarif_mensuel = lookup_widget(GTK_WIDGET(button), "checkbutton41");
    tarif_annuel = lookup_widget(GTK_WIDGET(button), "checkbutton42");
    capacite = lookup_widget(GTK_WIDGET(button), "spinbutton25");
    ajout_label = lookup_widget(GTK_WIDGET(button), "label187");

    // Check if all widgets were found
    if (!id_parking || !prix || !municipalite || !adresse || !etat_ouvert || 
        !etat_ferme || !id_agent || !tarif_heure || !tarif_mensuel || 
        !tarif_annuel || !capacite || !ajout_label) {
        gtk_label_set_text(GTK_LABEL(ajout_label), 
            "Erreur : certains éléments de l'interface sont manquants.");
        return;
    }

    // Get the values from entries
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(id_parking));
    const char *addr_text = gtk_entry_get_text(GTK_ENTRY(adresse));
    const char *agent_text = gtk_entry_get_text(GTK_ENTRY(id_agent));
    const char *prix_text = gtk_entry_get_text(GTK_ENTRY(prix));

    // Get municipalite from comboboxentry - Fix for municipalite
    const char *muni_text = gtk_entry_get_text(GTK_ENTRY(GTK_BIN(municipalite)->child));
    
    // Validate input data
    if (!id_text || !addr_text || !agent_text || !prix_text || !muni_text ||
        strlen(id_text) == 0 || strlen(addr_text) == 0 || 
        strlen(agent_text) == 0 || strlen(prix_text) == 0 || strlen(muni_text) == 0) {
        gtk_label_set_text(GTK_LABEL(ajout_label), 
            "Erreur : tous les champs doivent être remplis.");
        return;
    }

    // Copy values to parking structure
    strncpy(p.id_parking, id_text, sizeof(p.id_parking) - 1);
    strncpy(p.adresse, addr_text, sizeof(p.adresse) - 1);
    strncpy(p.id_agent, agent_text, sizeof(p.id_agent) - 1);
    strncpy(p.prix, prix_text, sizeof(p.prix) - 1);  // Fix for prix
    strncpy(p.municipalite, muni_text, sizeof(p.municipalite) - 1);  // Fix for municipalite

    // Get numeric and boolean values
    p.capacite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacite));
    
    // Set parking state
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(etat_ouvert))) {
        p.etat = 1; // ouvert
    } else {
        p.etat = 0; // ferme
    }

    // Set tarif options
    p.tarif[0] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tarif_heure)) ? 1 : 0;
    p.tarif[1] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tarif_mensuel)) ? 1 : 0;
    p.tarif[2] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tarif_annuel)) ? 1 : 0;

    // Add parking data to the file
    if (addParking("parking.txt", p) == -1) {
        gtk_label_set_text(GTK_LABEL(ajout_label), 
            "Erreur : impossible d'ajouter le parking. Vérifiez les données.");
    } else {
        gtk_label_set_text(GTK_LABEL(ajout_label), "Parking ajouté avec succès !");
        
        // Clear the form after successful addition
        gtk_entry_set_text(GTK_ENTRY(id_parking), "");
        gtk_entry_set_text(GTK_ENTRY(prix), "");
        gtk_entry_set_text(GTK_ENTRY(GTK_BIN(municipalite)->child), "");
        gtk_entry_set_text(GTK_ENTRY(adresse), "");
        gtk_entry_set_text(GTK_ENTRY(id_agent), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(capacite), 0);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ouvert), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ferme), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_heure), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_mensuel), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_annuel), FALSE);
    }
}


void
on_radiobutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
// Vérifier si le bouton radio "Ouvert" est actif
    if (gtk_toggle_button_get_active(togglebutton)) {
        g_print("L'état du parking est : Ouvert\n");
        // Place ici tout autre traitement spécifique lorsque "Ouvert" est activé
    }
}


void
on_radiobutton4_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
// Vérifier si le bouton radio "Ouvert" est actif
    if (gtk_toggle_button_get_active(togglebutton)) {
        g_print("L'état du parking est : Fermé\n");
        // Place ici tout autre traitement spécifique lorsque "Fermé" est activé
    }
}


void
on_checkbutton41_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton42_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton40_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton6_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
// Vérifier si le bouton radio "Ouvert" est actif
    if (gtk_toggle_button_get_active(togglebutton)) {
        g_print("L'état du parking est : Fermé\n");
        // Place ici tout autre traitement spécifique lorsque "Fermé" est activé
    }
}


void
on_radiobutton5_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton45_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton44_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton43_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


#include <glib.h>  // Include GLib for UTF-8 validation

// Function to check if a string is valid UTF-8
gboolean is_valid_utf8(const gchar *str) {
    return g_utf8_validate(str, -1, NULL);
}

void on_button15_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *id_parking;
    GtkWidget *municipalite;
    GtkWidget *adresse;
    GtkWidget *prix;
    GtkWidget *etat_ouvert;
    GtkWidget *etat_ferme;
    GtkWidget *id_agent;
    GtkWidget *tarif_heure;  
    GtkWidget *tarif_mensuel;    
    GtkWidget *tarif_annuel;     
    GtkWidget *capacite;
    GtkWidget *label189_message;
    Parking p;
    char filename[] = "parking.txt";

    // Retrieve widgets by their IDs
    id_parking = lookup_widget(GTK_WIDGET(button), "entry8");
    municipalite = lookup_widget(GTK_WIDGET(button), "comboboxentry24");
    adresse = lookup_widget(GTK_WIDGET(button), "entry9");
    prix = lookup_widget(GTK_WIDGET(button), "entry55");
    etat_ouvert = lookup_widget(GTK_WIDGET(button), "radiobutton5");
    etat_ferme = lookup_widget(GTK_WIDGET(button), "radiobutton6");
    id_agent = lookup_widget(GTK_WIDGET(button), "entry62");
    tarif_heure = lookup_widget(GTK_WIDGET(button), "checkbutton43");
    tarif_mensuel = lookup_widget(GTK_WIDGET(button), "checkbutton44");
    tarif_annuel = lookup_widget(GTK_WIDGET(button), "checkbutton45");
    capacite = lookup_widget(GTK_WIDGET(button), "spinbutton36");
    label189_message = lookup_widget(GTK_WIDGET(button), "label189");

    // Retrieve parking data from fields
    strcpy(p.id_parking, gtk_entry_get_text(GTK_ENTRY(id_parking)));
    strcpy(p.adresse, gtk_entry_get_text(GTK_ENTRY(adresse)));
    strcpy(p.id_agent, gtk_entry_get_text(GTK_ENTRY(id_agent)));
    p.capacite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacite));

    // Retrieve and sanitize the prix field (ensure it's a valid string)
    const gchar *prix_text = gtk_entry_get_text(GTK_ENTRY(prix));
    if (is_valid_utf8(prix_text)) {
        strcpy(p.prix, prix_text);  // Assuming prix is a string in Parking structure
    } else {
        // Handle invalid UTF-8 (you can sanitize the input or return an error)
        gtk_label_set_text(GTK_LABEL(label189_message), "Erreur : prix invalide.");
        return;
    }

    // Determine type of parking (private or public)
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(etat_ouvert))) {
        p.etat = 1; // ouvert
    } else {
        p.etat = 0; // ferme
    }

    // Determine pricing options
    p.tarif[0] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tarif_heure)) ? 1 : 0;
    p.tarif[1] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tarif_mensuel)) ? 1 : 0;
    p.tarif[2] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(tarif_annuel)) ? 1 : 0;

    // Search for the parking by its ID to modify it
    Parking parking_trouve = showParking(filename, p.id_parking);

    if (strlen(parking_trouve.id_parking) == 0) {
        // Parking not found
        gtk_label_set_text(GTK_LABEL(label189_message), "Erreur : Parking introuvable.");
        return;
    }

    // Retrieve the municipality value from the combo box
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(municipalite));
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

    // Iterate through the model to find the matching item
    while (valid) {
        gchar *value;
        gtk_tree_model_get(model, &iter, 0, &value, -1);  // Get the value in the first column

        if (g_strcmp0(value, parking_trouve.municipalite) == 0) {
            // Sanitize the municipality value before storing
            strcpy(p.municipalite, value);
            g_free(value);  // Free the memory allocated for value
            break;
        }
        g_free(value);  // Free the memory allocated for value
        valid = gtk_tree_model_iter_next(model, &iter);  // Move to the next item
    }

    // Modify the parking data
    if (modifyParking(filename, parking_trouve.id_parking, p) == -1) {
        // Error while modifying parking
        gtk_label_set_text(GTK_LABEL(label189_message), "Erreur : impossible de modifier le parking. Vérifiez les données.");
    } else {
        // Success
        gtk_label_set_text(GTK_LABEL(label189_message), "Parking modifié avec succès !");
    }
}




// Function to sanitize invalid UTF-8 strings
gchar* sanitize_utf8(const gchar *str) {
    if (!is_valid_utf8(str)) {
        g_print("Sanitizing invalid UTF-8: %s\n", str);
        return g_strdup("Invalid Input");
    }
    return g_strdup(str);
}

// Function to safely set text in a widget
void set_text_safe(GtkWidget *widget, const gchar *text) {
    gchar *sanitized_text = sanitize_utf8(text);
    gtk_entry_set_text(GTK_ENTRY(widget), sanitized_text);
    g_free(sanitized_text);  // Free the allocated memory after use
}

void on_button51_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *id_parking;
    GtkWidget *municipalite;
    GtkWidget *prix;
    GtkWidget *adresse;
    GtkWidget *etat_ouvert;
    GtkWidget *etat_ferme;
    GtkWidget *id_agent;
    GtkWidget *tarif_heure;  // Renommé en Tarif
    GtkWidget *tarif_mensuel;    // Renommé en Tarif
    GtkWidget *tarif_annuel;     // Renommé en Tarif
    GtkWidget *capacite;
    GtkWidget *label188_message;
    Parking p;
    char filename[] = "parking.txt";

    // Retrieve widgets by their IDs
    id_parking = lookup_widget(GTK_WIDGET(button), "entry8");
    prix = lookup_widget(GTK_WIDGET(button), "entry55");
    municipalite = lookup_widget(GTK_WIDGET(button), "comboboxentry24");
    adresse = lookup_widget(GTK_WIDGET(button), "entry9");
    etat_ouvert = lookup_widget(GTK_WIDGET(button), "radiobutton5");
    etat_ferme = lookup_widget(GTK_WIDGET(button), "radiobutton6");
    id_agent = lookup_widget(GTK_WIDGET(button), "entry62");
    tarif_heure = lookup_widget(GTK_WIDGET(button), "checkbutton43");
    tarif_mensuel = lookup_widget(GTK_WIDGET(button), "checkbutton44");
    tarif_annuel = lookup_widget(GTK_WIDGET(button), "checkbutton45");
    capacite = lookup_widget(GTK_WIDGET(button), "spinbutton36");
    label188_message = lookup_widget(GTK_WIDGET(button), "label188");

    // Retrieve parking data from fields
    strcpy(p.id_parking, gtk_entry_get_text(GTK_ENTRY(id_parking)));

    // Search for the parking by its ID
    Parking parking_trouve = showParking(filename, p.id_parking);

    if (strlen(parking_trouve.id_parking) == 0) {
GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "ID invalide ou inexistant.");
gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Parking not found
        gtk_label_set_text(GTK_LABEL(label188_message), "Erreur : Parking introuvable.");
        return;
    }

    // Parking found, display the details in the widgets
    // Retrieve the model of the ComboBox
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(municipalite));
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

    // Iterate through the model to find the matching item
    while (valid) {
        gchar *value;
        gtk_tree_model_get(model, &iter, 0, &value, -1);  // Get the value in the first column

        if (g_strcmp0(value, parking_trouve.municipalite) == 0) {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(municipalite), &iter);
            g_free(value);  // Free the memory allocated for value
            break;
        }

        g_free(value);  // Free the memory allocated for value
        valid = gtk_tree_model_iter_next(model, &iter);  // Move to the next item
    }

    // Sanitize and set text for adresse and id_agent
    set_text_safe(adresse, parking_trouve.adresse);
    set_text_safe(id_agent, parking_trouve.id_agent);

    // Sanitize and set prix text
    gchar *prix_text = g_strdup_printf("%s", parking_trouve.prix);
    set_text_safe(prix, prix_text);
    g_free(prix_text);

    // Set the remaining fields
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(capacite), parking_trouve.capacite);

    if (parking_trouve.etat == 1) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ouvert), TRUE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ferme), FALSE);
    } else {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ferme), TRUE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ouvert), FALSE);
    }

    // Set Tarif values based on the array of Tarif in parking_trouve
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_heure), parking_trouve.tarif[0]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_mensuel), parking_trouve.tarif[1]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_annuel), parking_trouve.tarif[2]);

    // Display success message
    gtk_label_set_text(GTK_LABEL(label188_message), "Parking trouvé avec succès !");
}



void
on_button16_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *id_parking;
    GtkWidget *municipalite;
    GtkWidget *prix;
    GtkWidget *adresse;
    GtkWidget *etat_ouvert;
    GtkWidget *etat_ferme;
    GtkWidget *id_agent;
    GtkWidget *tarif_heure;  // Renommé en Tarif
    GtkWidget *tarif_mensuel;    // Renommé en Tarif
    GtkWidget *tarif_annuel;     // Renommé en Tarif
    GtkWidget *capacite;
    GtkWidget *label188_message;
    Parking p;
    char filename[] = "parking.txt";

    // Retrieve widgets by their IDs
    id_parking = lookup_widget(GTK_WIDGET(button), "entry8");
    prix = lookup_widget(GTK_WIDGET(button), "entry55");
    municipalite = lookup_widget(GTK_WIDGET(button), "comboboxentry24");
    adresse = lookup_widget(GTK_WIDGET(button), "entry9");
    etat_ouvert = lookup_widget(GTK_WIDGET(button), "radiobutton5");
    etat_ferme = lookup_widget(GTK_WIDGET(button), "radiobutton6");
    id_agent = lookup_widget(GTK_WIDGET(button), "entry62");
    tarif_heure = lookup_widget(GTK_WIDGET(button), "checkbutton43");
    tarif_mensuel = lookup_widget(GTK_WIDGET(button), "checkbutton44");
    tarif_annuel = lookup_widget(GTK_WIDGET(button), "checkbutton45");
    capacite = lookup_widget(GTK_WIDGET(button), "spinbutton36");
    label188_message = lookup_widget(GTK_WIDGET(button), "label188");

    // Retrieve parking data from fields
    strcpy(p.id_parking, gtk_entry_get_text(GTK_ENTRY(id_parking)));

    // Search for the parking by its ID
    Parking parking_trouve =  showParking(filename, p.id_parking);

    if (strlen(parking_trouve.id_parking) == 0) {
        // Parking not found
        gtk_label_set_text(GTK_LABEL(label188_message), "Erreur : Parking introuvable.");
        return;
    }

    // Parking found, display the details in the widgets
    // Retrieve the model of the ComboBox
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(municipalite));
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

    // Iterate through the model to find the matching item
    while (valid) {
        gchar *value;
        gtk_tree_model_get(model, &iter, 0, &value, -1);  // Get the value in the first column

        if (g_strcmp0(value, parking_trouve.municipalite) == 0) {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(municipalite), &iter);
            g_free(value);  // Free the memory allocated for value
            break;
        }

        g_free(value);  // Free the memory allocated for value
        valid = gtk_tree_model_iter_next(model, &iter);  // Move to the next item
    }

    gtk_entry_set_text(GTK_ENTRY(adresse), parking_trouve.adresse);
    gtk_entry_set_text(GTK_ENTRY(prix), g_strdup_printf("%s", parking_trouve.prix));
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(capacite), parking_trouve.capacite);

    if (parking_trouve.etat == 1) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ouvert), TRUE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ferme), FALSE);
    } else {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ferme), TRUE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(etat_ouvert), FALSE);
    }

    // Set Tarif values based on the array of Tarif in parking_trouve
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_heure), parking_trouve.tarif[0]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_mensuel), parking_trouve.tarif[1]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tarif_annuel), parking_trouve.tarif[2]);

    // Display success message
    gtk_label_set_text(GTK_LABEL(label188_message), "Parking trouvé avec succès !");
const char *Id_parking = gtk_entry_get_text(GTK_ENTRY(id_parking));
// Add parking data to the file
    if (removeParking("parking.txt", Id_parking) == -1) {
         
        // Error while adding parking
        gtk_label_set_text(GTK_LABEL(label188_message), "Erreur : impossible d'ajouter le parking. Vérifiez les données.");
        
    } else {
        // Success
        gtk_label_set_text(GTK_LABEL(label188_message), "Parking ajouté avec succès !");
        removeParking("work.txt", Id_parking);
    }        

}


void
on_treeview13_row_activated            (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button17_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

    // Lookup widgets for parking and agent entries
    GtkWidget *box_parking = lookup_widget(GTK_WIDGET(button), "entry56");
    GtkWidget *box_agent = lookup_widget(GTK_WIDGET(button), "entry57");

    // Get the text from entries
    const char *id_parking = gtk_entry_get_text(GTK_ENTRY(box_parking));
    const char *id = gtk_entry_get_text(GTK_ENTRY(box_agent));

    // Validate parking_id and agent_id
    if (strlen(id_parking) == 0 || strlen(id) == 0) {
        g_print("Error: Please enter both a parking ID and an agent ID.\n");
        return;
    }

    // Call assignAgentToParking function
    int success = assignAgentToParking("parking.txt", "agent.txt", id_parking, id);

    if (success) {
        g_print("Agent %s successfully assigned to Parking %s.\n", id, id_parking);
    } else {
        g_print("Error: Could not assign Agent %s to Parking %s. Please check availability.\n", id, id_parking);
    }


}



void
on_button50_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
// Get the GtkEntry widget for parking
GtkWidget *entry_widget_parking = lookup_widget(GTK_WIDGET(button), "entry56");
GtkEntry *entry_parking = GTK_ENTRY(entry_widget_parking);

// Get the GtkEntry widget for agents
GtkWidget *entry_widget_agents = lookup_widget(GTK_WIDGET(button), "entry57");
GtkEntry *entry_agents = GTK_ENTRY(entry_widget_agents);

// Ensure the user enters values directly into the entries
const gchar *parking_text = gtk_entry_get_text(entry_parking);
const gchar *agent_text = gtk_entry_get_text(entry_agents);

// Use the entered values as needed
if (g_strcmp0(parking_text, "") == 0 || g_strcmp0(agent_text, "") == 0) {
    g_print("Please fill in both parking and agent fields.\n");
} else {
    g_print("Parking: %s, Agent: %s\n", parking_text, agent_text);
}

// Lookup the TreeView widget
GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview13");

// Get the model associated with the TreeView
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

if (model == NULL) {
    // If no model exists, create a new one and set it
    GtkListStore *store = create_work_list_store();
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store); // Release reference since TreeView holds its own
} else {
    // If a model already exists, clear it
    gtk_list_store_clear(GTK_LIST_STORE(model));
}

// Ensure columns are created only once
if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
    create_treeview1_columns(GTK_TREE_VIEW(treeview));
}

// Populate the TreeView with data from the file
populate_treeview1_with_data(GTK_TREE_VIEW(treeview), "work.txt");


}


void
on_treeview2_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button40_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button56_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_checkbutton30_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton31_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button18_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton7_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton8_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button19_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton9_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton10_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button41_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button20_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview3_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button21_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview4_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button57_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button36_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton17_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton18_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton19_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton32_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton33_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button22_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button54_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton20_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton22_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton21_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton35_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton34_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button23_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button24_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview10_row_activated            (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button25_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button37_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview12_row_activated            (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button43_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button58_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton13_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton14_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button26_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_checkbutton13_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton14_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton15_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton16_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton15_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton16_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton17_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton18_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton19_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton20_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button27_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button42_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button28_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview11_row_activated            (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button35_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview5_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button53_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button59_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton23_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton24_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton25_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton36_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton37_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button52_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button48_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton26_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_radiobutton27_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton38_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton39_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button30_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_radiobutton28_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button31_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_treeview6_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_button46_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}

