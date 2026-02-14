Parfait, un chronomètre avec Allegro c’est très simple.
Je te propose une solution propre en 3 étapes : démarrer le timer au premier clic, l’afficher en haut de l’écran, et le mettre en pause quand la partie est finie.

✅ 1) Déclarer une variable temps

Dans ton main ou dans ta structure de partie :

double start_time = 0;
double current_time = 0;
bool started = false;

✅ 2) Démarrer le chrono au premier clic

Dans ta boucle d’évènements, au moment où le joueur fait son premier clic gauche, ajoute :

if (!started && event.mouse.button == 1) {
    start_time = al_get_time(); // démarre le chrono
    started = true;
}

✅ 3) Calculer le temps écoulé

À chaque frame (juste avant ton al_flip_display()), fais :

if (started) {
    current_time = al_get_time() - start_time;
}


al_get_time() renvoie le nombre de secondes depuis le début du programme.

✅ 4) Afficher le chrono

Partout où tu fais tes affichages :

al_draw_textf(font, al_map_rgb(255,255,255), 100, 30,
              ALLEGRO_ALIGN_CENTER, "%d s", (int)current_time);


Tu peux évidemment changer la position (100, 30) et mettre une couleur.

📌 Option : arrêter le chrono quand la partie est perdue/gagnée

Quand tu fais game_over() ou fin_de_partie(), ajoute :

started = false;


Comme ça le temps reste affiché mais ne continue plus.

📌 Si tu veux plus propre : un format mm:ss

Je peux aussi te donner une version qui affiche :

02:37


(2 minutes et 37 secondes)

Dis-moi si tu veux que je te l’écrive.

Si tu veux je peux carrément t’intégrer le chrono directement dans ton code actuel — tu me colles juste ta fonction partie() et je te fais la version avec timer clean.