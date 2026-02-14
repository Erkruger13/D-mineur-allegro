// afficher un texte centrer a l'endroit ou ne clique
// bouton souris 0 : aucun ,  1 : gauche pressé, 2 : droite pressé, 3 : droit et gauche pressé, 4 : bouton du milieu presé 
#include "Minesweeper.hpp"
#include <ctime>

int main(){
    srand(time(NULL));
    al_init();
    al_init_primitives_addon();
    al_install_mouse();
    
    ALLEGRO_DISPLAY *display = al_create_display(1000, 800);
    al_set_window_title(display, "Démineur");
    
    ALLEGRO_COLOR color = BLEU;
    al_clear_to_color(color); 
    al_flip_display();
    std::cout<<"Bienvenu les garsss !"<<std::endl;
    Game partie(1, display);
    bool win = partie.partie();
    
    if(!win){
        std::cout << "La partie est perdue !" << std::endl;
    }
    else {
        std::cout << "Victoire !" << std::endl;
    }
    //al_rest(10.0);
    al_destroy_display(display);
    return 0;
}

/*S
Déroulement d'une partie : Un objet game est appeler il crée des attributs avec la liste des bombes, les matrices, les cases
La methode partie est appeler et ça lance une boucle qui reçoit les coordonées des clic et les donne
*/

