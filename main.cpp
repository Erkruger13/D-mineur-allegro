// afficher un texte centrer a l'endroit ou ne clique
// bouton souris 0 : aucun ,  1 : gauche pressé, 2 : droite pressé, 3 : droit et gauche pressé, 4 : bouton du milieu presé 
#include "Minesweeper.hpp"


int main(){
    al_init();
    al_init_primitives_addon();
    al_install_mouse();
    ALLEGRO_DISPLAY *display = al_create_display(1000, 800);
    al_set_window_title(display, "Démineur");
    ALLEGRO_COLOR color = al_map_rgba(173, 216, 230, 255);
    al_clear_to_color(color); 
    al_flip_display();
    // Créer la file d'événements
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    bool running = true;
    Game partie(1, display);
    running = partie.partie();
    if(!running){std::cout<<"La partie est perdu mon pote"<<std::endl;}
    //al_destroy_display(display);
    std::cout << "les modif nv" << std::endl;
    return 0;
} //

// faire la fonction qui crée la liste d'objet en ajoutant a chaque fois leurs coordonée et en modifiant l'attribu bombe si il le sont
//crée une fonction qui fait la matrice de la grille et une la matrice d'adjacence 
// faut finir la liste objet et sensuite faire les fonctions des matrices d'adjaçanse et la matrice classique
// on revois la fun d'affichage map
// Pas besoin de placer un attribut nombre enfet il suffit de garder la matrice d'adjacant et de se servir de l'attribut coor pour connaitre le i,j de l'obj dans la mat
// il faudra faire une class game
/*
Déroulement d'une partie : Un objet game est appeler il crée des attributs avec la liste des bombes, les matrices, les cases
La methode partie est appeler et ça lance une boucle qui reçoit les coordonées des clic et les donne
*/
