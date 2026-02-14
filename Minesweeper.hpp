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
#define ROUGE al_map_rgb(255, 0, 0)
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

bool contient(const std::vector<Point>& v, const Point& p) {// si la liste v contient le point p
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
    Point coor;//coor dans la mat
    Point p1;//coor en haut a gauche
    Point p2;//coor en bas a droite
    bool bombe;
    bool show;
    bool show_twice;
    bool flag;
    int nombre;
    
    Carre() : bombe(false), show(false), show_twice(false), flag(false), nombre(0) {}
    
    Carre(short int a, short int o, int b, int c, Point p) 
        : bombe(false), show(false), show_twice(false), flag(false), nombre(0) {
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
        al_draw_filled_rectangle(p1.x, p1.y, p2.x, p2.y, ROUGE);
        al_draw_rectangle(p1.x, p1.y, p2.x, p2.y, NOIR, 2.0);
    }
    
    void game_over() {
        al_draw_filled_rectangle(p1.x, p1.y, p2.x, p2.y, NOIR);
    }
    
    void showed(ALLEGRO_FONT* font) {
        int center_x = (p1.x + p2.x) / 2;
        int center_y = (p1.y + p2.y) / 2;
        
        al_draw_filled_rectangle(p1.x, p1.y, p2.x, p2.y, BEIGE);
        al_draw_rectangle(p1.x, p1.y, p2.x, p2.y, NOIR, 2.0);
        
        if(nombre > 0) {
            al_draw_textf(font, NOIR, center_x, center_y, ALLEGRO_ALIGN_CENTER, "%d", nombre);
        }
    }
};

struct GameData {
    std::vector<std::vector<Carre>> objets;
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

void map(std::vector<std::vector<Carre>>& grille, ALLEGRO_FONT* font) {
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 12; j++) {
            Carre& p = grille[i][j];
            if(p.show) {
                p.showed(font);
            }
            else if(p.flag) {
                p.flags();//if(p.coor.x == 1 and p.coor.y == 1){std::cout <<"attributs " <<p.flag<<p.bombe<<p.show<<std::endl; }
            }
            else {
                p.draw();
                //if(p.coor.x == 1 and p.coor.y == 1){std::cout <<"attributs " <<p.flag<<p.bombe<<p.show<<std::endl; }
            }
        }
    }
}

std::vector<Point> cases_autour(Point p) {// cette fonction me renvoie une liste des coordonée valid autour d'une case(fait parn le mid attention)
    std::vector<Point> voisins;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1,  0,  1,-1, 1,-1, 0, 1};
    
    for (int k = 0; k < 8; k++) {
        int nx = p.x + dx[k];
        int ny = p.y + dy[k];
        if (nx >= 0 && nx < 12 && ny >= 0 && ny < 12) {
            voisins.push_back(Point(nx, ny));
        }
    }
    return voisins;
}

std::vector<std::vector<int>> adjacant(std::vector<std::vector<int>> res) {
    std::vector<std::vector<int>> matrix(12, std::vector<int>(12, 0));
    
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1,  0,  1,-1, 1,-1, 0, 1};
    
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (res[i][j] == 1) {
                matrix[i][j] = 9;
                continue;
            }
            
            int count = 0;
            for (int d = 0; d < 8; d++) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                
                if (ni >= 0 && ni < 12 && nj >= 0 && nj < 12) {
                    if (res[ni][nj] == 1)
                        count++;
                }
            }
            matrix[i][j] = count;
        }
    }
    return matrix;
}

GameData liste_objets(std::vector<Point> bombes) {
    int x1, x2, y1{100}, y2{150};
    std::vector<std::vector<Carre>> obj;
    std::vector<std::vector<int>> matrix;
    
    obj.resize(12);
    matrix.resize(12);
    for (int i = 0; i < 12; ++i) {
        obj[i].resize(12);
        matrix[i].resize(12);
    }
    
    for(int i = 0; i < 12; i++) {
        x1 = 200; x2 = 250;
        
        for(int j = 0; j < 12; j++) {
            Point p(i, j);
            Carre c(x1, y1, x2, y2, Point(i, j));
            
            if(contient(bombes, p)) {
                c.bombe = true;
                matrix[i][j] = 1;
            }
            
            obj[i][j] = c;
            x1 += 50; 
            x2 += 50;
        }
        y1 += 50; 
        y2 += 50;
    }
    
    GameData tup; 
    tup.objets = obj; 
    tup.matrix = matrix;
    return tup; 
}

bool autour_flag(std::vector<std::vector<Carre>>& objet, Point p, std::vector<std::vector<int>>& adj) { 
    short flags_number = 0;
    std::vector<Point> liste = cases_autour(p);
    
    for(auto& point : liste) {
        if(objet[point.x][point.y].flag) {
            flags_number++;
        }
    }
    return adj[p.x][p.y] == flags_number;
}

bool autour(std::vector<std::vector<Carre>>& objet, Point p, std::vector<std::vector<int>>& adj) {
    std::vector<Point> liste = cases_autour(p);
    
    if(autour_flag(objet, p, adj)) {
        for(auto& point : liste) {
            Carre& c = objet[point.x][point.y];
            if(c.bombe && !c.flag) {
                return false;
            }
            else if(c.bombe && c.flag){}
            else {
                c.show = true;
            }
        }
    }
    return true;
}

/*void degats(std::vector<std::vector<Carre>>& objet, Point& p, ){
    Carre& c = objet[p.x][p.y];
    std::vector<Point> around = cases_autour(p);
    if(c.show || c.bombe || c.flag){return;}//si il contient une bombe on arrête là pour lui pareil flag ou show
    c.show = true;
    if(c.nombre==0){c.show;}
    for(auto& i : around){// si une des cases contient une bombe c'est bon on revient
        degats(objet, i);     
    }

}
    */
void degats(std::vector<std::vector<Carre>>& objet, Point p) {
    Carre& c = objet[p.x][p.y];
    
    // Conditions d'arrêt
    if(c.show || c.bombe || c.flag) {
        return;  // Déjà révélée, ou bombe, ou flag
    } 
    c.show = true; 
    // Si la case a des bombes autour, on s'arrête (ne pas propager)
    if(c.nombre > 0) {
        return;
    }
    
    // Si nombre == 0, propager récursivement aux 8 cases adjacentes
    std::vector<Point> around = cases_autour(p);
    for(auto& voisin : around) {
        degats(objet, voisin);
    }
}

bool clic(std::vector<std::vector<Carre>>& grille, Point point, std::vector<std::vector<int>>& adj) {
    // Convertir les coordonnées pixel en indices de grille
    int j = (point.x - 200) / 50;
    int i = (point.y - 100) / 50;
    
    // Vérifier les limites
    if(i < 0 || i >= 12 || j < 0 || j >= 12) {
        return true;
    }
    
    Carre& c = grille[i][j];
    
    if(c.bombe) {
        return false;
    }
    else if(c.show && !c.show_twice) {
        return autour(grille, c.coor, adj);
    }
    else if(c.flag) {
        c.flag = false;
    }
    else if(c.nombre == 0){degats(grille, c.coor);}
    else {
        c.show = true;
        // faudra rajouter la fonction qui révèle les bails autour ici
    }
    
    return true;
}

bool is_win(std::vector<std::vector<Carre>>& objet) {
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 12; j++) {
            if(!objet[i][j].bombe && !objet[i][j].show) {
                return false;
            }
        }
    }
    return true;
}

class Game {
public:
    std::vector<std::vector<int>> mat;
    std::vector<std::vector<int>> mat_adj;
    std::vector<Point> bombe;
    std::vector<std::vector<Carre>> objet;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    
    Game(short int level, ALLEGRO_DISPLAY* disp) {
        bombe = Bombe(level);
        display = disp;
        font = al_create_builtin_font();
        
        GameData tuple = liste_objets(bombe);
        objet = tuple.objets; 
        mat = tuple.matrix;
        mat_adj = adjacant(mat);
        number();
        
        map(objet, font);
        grille(mat);
    }
    
    ~Game() {
        if(font) al_destroy_font(font);
    }
    
    void number() {
        for(int i = 0; i < 12; i++) {// fonction qui met le nombre de bombes adj dans chaque obj
            for(int j = 0; j < 12; j++) {
                if(!objet[i][j].bombe) {
                    objet[i][j].nombre = mat_adj[i][j];
                }
            }
        }
    }
    
    void game_over() {
        for(int i = 0; i < 12; i++) {
            for(int j = 0; j < 12; j++) {
                if(!objet[i][j].bombe) {
                    objet[i][j].showed(font);
                }
                else {
                    objet[i][j].game_over();
                }
            }
        }
        al_flip_display(); 
        al_rest(5.0);
    }
    
    bool partie() {
        bool win = true;
        ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
        al_register_event_source(queue, al_get_mouse_event_source());
        al_register_event_source(queue, al_get_display_event_source(display));
        
        al_clear_to_color(BLEU);
        map(objet, font);
        al_flip_display();
        
        while(win) {
            ALLEGRO_EVENT event;
            al_wait_for_event(queue, &event); 
            
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                win = false;
                break;
            }
            
            if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                int bouton = event.mouse.button;
                
                if(bouton == 1) {
                    Point p(event.mouse.x, event.mouse.y);
                    win = clic(objet, p, mat_adj);
                    
                    
                    if(!win) {
                        al_clear_to_color(BLEU);
                        game_over();
                        al_destroy_event_queue(queue);
                        //degats(objet, p);// renvoyer au menu
                        return false;
                    }
                    
                    if(is_win(objet)) {
                        //Faudra mettre une animation de victoire et puis renvoyer au menu
                        win = false;
                    }
                }
                else if(bouton == 2) {
                    // Clic droit - ajouter flag
                    int j = (event.mouse.x - 200) / 50;
                    int i = (event.mouse.y - 100) / 50;
                    std::cout<<"Le flag est mis "<<std::endl;
                    if(i >= 0 && i < 12 && j >= 0 && j < 12) {
                        if(!objet[i][j].show) {
                            objet[i][j].flag = !objet[i][j].flag;
                        }
                    }
                }
            }
            
            al_clear_to_color(BLEU);
            map(objet, font);
            al_flip_display();
        }
        
        al_destroy_event_queue(queue);
        return false;
    }
};

/* on vas faire le menu ici
objectif présenter le menu donc un bouton pour choisir le level et un autre pour appuyer play
il faut aussi que j'affiche le nombre de flags restant et le chrono
*/
#endif
