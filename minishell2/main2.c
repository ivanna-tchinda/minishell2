#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

typedef struct t_struct{
    char tab[1024];
}              s_struct;

int main() 
{
    struct t_struct var;
    DIR *directory;
    struct dirent *dirr;

    // Obtenir le répertoire de travail actuel
    if (getcwd(var.tab, sizeof(var.tab)) != NULL) {
        directory = opendir(var.tab);
        if (directory) {
            while ((dirr = readdir(directory)) != NULL) {
                printf("fichier : %s\n", dirr->d_name);
            }
            closedir(directory);
        } else {
            perror("Erreur lors de l'ouverture du répertoire");
            return 1;
        }
    } else {
        perror("Erreur lors de l'obtention du répertoire de travail actuel");
        return 1;
    }
    return 0;
}
