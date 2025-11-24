#ifndef MINESWEEPER_HPP
#define MINESWEEPER_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define NOIR al_map_rgb(0,0,0)
#define BEIGE al_map_rgb(238, 214, 175)
#define BLANC al_map_rgb(255, 255, 255)
#define ROUGE al_map_rgb(255, 0, 0) // pour les drapeaux
#define BLEU al_map_rgba(173, 216, 230, 255)
#define GRIS al_map_rgb(200, 200, 200)
struct Point {
    short int x;
    short int y;
    
    Point() : x(0), y(0) {}
    
    Point(short int xi, short int yi) {
        x = xi;
        y = yi;
    }
};


bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool contient(const std::vector<Point>& v, const Point& p) {
    return std::find(v.begin(), v.end(), p) != v.end();
}

// ===== FONCTIONS UTILITAIRES =====
void erreur(const char* txt) {
    ALLEGRO_DISPLAY* d;
    d = al_is_system_installed() ? al_get_current_display() : NULL;
    al_show_native_message_box(d, "Erreur", txt, "Flop en approche", NULL, 0);
    exit(EXIT_FAILURE);
}

bool used[100] = {false};

int getUniqueRandom(bool used[]) {
    int n;
    do {
        n = rand() % 100;
    } while (used[n]);
    
    used[n] = true;
    return n;
}

std::vector<Point> Bombe(short int level) {
    short int x, y;
    std::vector<Point> liste;
    
    for(int i = 0; i < level * 20; i++) {
        int nombre = getUniqueRandom(used);
        if(nombre > 9) {
            x = nombre / 10;
            y = nombre % (x * 10);
        }
        else if(nombre == 0) {
            x = 0; 
            y = 0;
        }
        else {
            x = 0; 
            y = nombre;
        }
        liste.push_back(Point(x, y));
    }
    return liste;
}

// ===== CLASSE CARRE =====
class Carre {
public:
    Point coor;// coor dans la matrice pour l'instant ne sert a rien mais au cas ou
    Point p1; // coor en haut a gauche du carre
    Point p2; // coor en bas a droite du carre
    bool bombe;
    bool show, show_twice{false};
    bool flag;
    int nombre; // nombres de bombes adjacante
    
    Carre(short int a, short int o, int b, int c, Point p) 
        : bombe(false), show(false), flag(false), nombre(0) {
        p1.x = a;
        p2.x = b;
        p1.y = o;
        p2.y = c;
        coor = p;
    }
    
    void draw() {
        ALLEGRO_COLOR color = al_map_rgba(0, 0, 0, 255);
        al_draw_filled_rectangle(p1.x, p1.y, p2.x, p2.y, GRIS);
        al_draw_rectangle(p1.x, p1.y, p2.x, p2.y, color, 2.0);
    }
    
    void flags() {
        al_draw_filled_rectangle(p1.x, p1.y, p1.x + 50, p1.y + 50, al_map_rgb(255, 0, 0));
    }
    
    void game_over() {
        al_draw_filled_rectangle(p1.x, p1.y, p1.x + 50, p1.y + 50, al_map_rgb(0, 0, 0));
    }
    
    void showed() {
        int center_x = (p1.x + p2.x) / 2;
        int center_y = (p1.y + p2.y) / 2;
        
        ALLEGRO_FONT* font = al_create_builtin_font();
        draw();
        al_draw_filled_rectangle(p1.x, p1.y, p2.x, p2.y, BEIGE);
        al_draw_textf(font, NOIR, center_x, center_y, ALLEGRO_ALIGN_CENTER, "%d", nombre);
        //al_destroy_font(font);
    }
};

struct GameData {
    std::vector<Carre> objets;
    std::vector<std::vector<int>> matrix;
};

// ===== FONCTIONS DE JEU =====
void grille(std::vector<std::vector<int>> matrix) {
    for (const auto& row : matrix) {
        std::cout << "[";
        for (size_t i = 0; i < row.size(); ++i) {
            if (i != 0)
                std::cout << ", ";
            std::cout << static_cast<int>(row[i]);
        }
        std::cout << "] " << std::endl;
    }
}

void map(std::vector<Carre> liste, bool over = false) {
    for(auto& p : liste) {
        if(p.show) {
            p.showed();
        }
        else if(p.flag) {
            p.flags();
        }
        else {
            p.draw();
        }
    }
}

std::vector<Point> cases_autour(std::vector<std::vector<int>> liste, Point p)
{
    std::vector<Point> voisins;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1,  0,  1,-1, 1,-1, 0, 1};
    for (int k = 0; k < 8; k++)
    {
        int nx = p.x + dx[k];
        int ny = p.y + dy[k];
        if (nx >= 0 && nx < 12 && ny >= 0 && ny < 12)
        {voisins.push_back(Point(nx, ny));}
    }

    return voisins;
}


std::vector<std::vector<int>> adjacant(std::vector<std::vector<int>> res)
{
    std::vector<std::vector<int>> matrix(12, std::vector<int>(12, 0)); // cette fonction construit la matrice d'ajacant

    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1,  0,  1,-1, 1,-1, 0, 1};

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (res[i][j] == 1)
            {
                matrix[i][j] = 9;   // Case qui contient une bombe
                continue;
            }

            int count = 0;

            for (int d = 0; d < 8; d++)
            {
                int ni = i + dx[d];
                int nj = j + dy[d];

                if (ni >= 0 && ni < 12 && nj >= 0 && nj < 12)
                {
                    if (res[ni][nj] == 1)
                        count++;
                }
            }

            matrix[i][j] = count;
        }
    }

    return matrix;
}



GameData liste_objets(std::vector<Point> bombes)
{// cette fonction met les bombes dans les attributs des objet concernée et met les coordonées matriciel dans les attributs de chaque objet
    // elle crée ainsi notre premièreb grille 
    // elle ne seras utiliser qu'au début de chaque partie
    // elle place aussi le nombres de bombe adjacantes
    int x1, x2, y1{100}, y2{150}; // m colones et n lignes
    std::vector<Carre> obj;
    // création d'une matrice vide ou on ira plaçé les bombes
    std::vector<std::vector<int>> matrix;
    int test= 0;
    matrix.resize(12); // Row vectors inside the main vector
    for (int i = 0; i < 12; ++i) {
      matrix[i].resize(12);
    }
    for(int i = 0; i < 12; i++ )
    {
        x1 = 200; x2 = 250;
        for(int j = 0; j < 12; j++)
        {
           Point p(i, j);
           if(contient(bombes, p))//il contient une bombe
           {
              Carre c(x1, y1, x2, y2, Point(i, j));
              test++;
              //printf("Bombe : (%d, %d)", i, j);
              c.bombe = true; matrix[i][j] = 1; obj.push_back(c);}
            //Carre c (Point(i, j);
            else{
                Carre c(x1, y1, x2, y2, Point(i, j)); obj.push_back(c);
            }
            x1+= 50; x2+= 50;
        }
        y1 += 50; y2 += 50;
    }
    GameData tup; tup.objets = obj; tup.matrix = matrix;
    // faut gérer le return car faut aussi retourner la matrix
//std::vector<std::vector<int>> vec = adjacant(matrix);
    return tup; 
}

bool autour_flag(std::vector<Carre> objet, std::vector<std::vector<int>> mat, Point p, std::vector<std::vector<int>> adj, std::vector<Point> liste){
    // cette fonction sert a voir si il y a assez de flags autour d'un nombre
    short flags_number = 0;
    //std::vector<Point> liste = cases_autour(mat, p);
    for(auto& i : objet){
        if(contient(liste, i.coor)){
            if(i.flag){flags_number++;
            if(flags_number == 8){break;}}
        }
    }
    return adj[p.x][p.y]== flags_number;
    // elle sert dans le cas ou on clique sur une case déjà révélée pour déverouiller ce qu'il y a autour
}
// ce serait bien d'avoir une liste des coor autour comme ça je regarde if(coor a tester in liste des coor) je vérifie l'attribut flag
// et si jamais il est égale c'est carré
bool autour(std::vector<Carre>& objet, std::vector<std::vector<int>> mat, Point p, std::vector<std::vector<int>> adj){
    // cette fonction vas regarder quels case est autour et edit les flags nécésaire
    // elle est appeler pour re cliquer sur une case déjà dévoiler
    std::vector<Point> liste = cases_autour(mat, p);// liste des cases autour
    if(autour_flag(objet, mat, p, adj, liste)){
        for(auto& i : objet){
            if(contient(liste, i.coor)){
                if(i.bombe){return false;}
                else{i.show = true;}
            }
        }
    }
    return true;
    
    
}


bool clic(std::vector<Carre>& liste, Point point, std::vector<std::vector<int>> mat, std::vector<std::vector<int>> adj){
    // cette fonction reçoit un Point et doit trouver le carre qui a été cliqué 
    // elle doit aussi mettre a jour ses attribut et return true si c'est ok et false si le c'est une bombe qui a été touché
    for(auto& p : liste){
        if((p.p1.x <= point.x) && (point.y >= p.p1.y) && (point.x <=p.p2.x) && (point.y <= p.p2.y) )
        {
            //if(droit){p.flags();}
            if(p.bombe){return false;}
            else if(p.show && !p.show_twice){
                return autour(liste, mat, p.coor, adj);}// ici il faudra appeler la fonction qui révèle les cases qui ne sont pas des bombes autour de la case
                //si c'est true c'est qu'il n'y avais pas de bombes parmis autour
            else if(p.flag){p.flag=false;}
            else{p.show = true;}
        }
    }
    return true;
}
bool is_win(std::vector<Carre> objet){
    bool var = true;
    for(auto& p : objet){
        if(!p.bombe and !p.show){return false;} // si il croise une case qui n'est pas une bombe et qui n'a pas encore été révélé c'est que la partie est pas fini
    }
    return true;// si il n'en croise pas c'est gagné
}



class Game{
public :
       std::vector<std::vector<int>> mat; // matrice normal
       std::vector<std::vector<int>> mat_adj;// matrice d'adjacant
       std::vector<Point> bombe; //Bombe
       std::vector<Carre> objet;
       ALLEGRO_DISPLAY* display;
    
       Game(short int level, ALLEGRO_DISPLAY* disp){
        bombe = Bombe(level);
        display = disp;
        GameData tuple = liste_objets(bombe);
        objet = tuple.objets; mat = tuple.matrix;
        mat_adj = adjacant(mat);
        number();
        // a la fin de mon constructeur j'affiche la première grille
        map(objet);
        grille(mat);
        //partie();
        // cette fonction lance une partie
        
       }
       void number(){
        for(auto& p : objet){
            if(!p.bombe){p.nombre = mat_adj[p.coor.x][p.coor.y];}
        }
       }
       void game_over(){
        int a;
        for(auto& p : objet){
            if(!p.bombe){
            p.showed();}// si la game est perdu on affiche tout
            else{
                p.game_over();
            }
            }
        al_flip_display(); al_rest(10.0); // quand la partie est fini on est ici
       }
       bool partie(){
           bool win = true;
           ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
           al_register_event_source(queue, al_get_mouse_event_source());
           al_register_event_source(queue, al_get_display_event_source(display));
           int bouton;
           al_clear_to_color(BLEU);
           map(objet);
           while(win){
                ALLEGRO_EVENT event;
                al_wait_for_event(queue, &event); 
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                win = false; al_destroy_display(display); break;}
                if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                bouton = event.mouse.button;
                if(bouton == 1) {// si jamais c'est clique gauche
                   Point p(event.mouse.x, event.mouse.y);
                   win = clic(objet, p, mat, mat_adj); // si win est true la game continue et les flags on été mis a jours
                   if(!win){al_clear_to_color(BLEU);game_over();
                   return false;}
                   //map(objet);
                   //if(is_win(objet)){win = false;}// faudra appeler une methode particulière pour une animation de fin
                }  
                /*else{// cas ou il a fait clic droit il faut donc placer un drapeau
                    Point p(event.mouse.x, event.mouse.y);
                     win = clic(objet, p, mat, mat_adj, true);
                } */           
                }
                // faut gérer le cas du clic droit ici
                al_clear_to_color(BLEU);
                map(objet);
                al_flip_display();

           }
           return false;
        }


};

#endif

// il faut faire une methode qui vérifie si la game est gagné ou pas elle vas aller vérifier que celle les cases contenant des bombes sont encore présente
// To do : function qui vérifie flag et fun qui edit les flags autour, dirst click
// si showed_twice faut ignore 
