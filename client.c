#include <stdio.h>
#include <curl/curl.h>

// Fonction d'écriture des données reçues dans un fichier
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

// Fonction de debug pour afficher plus de détails
static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    const char *text;
    switch (type) {
        case CURLINFO_TEXT:
            fprintf(stderr, "== Info: %s", data);
            break;
        default:
            return 0;
    }
    return 0;
}

int main(void) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "http://192.168.133.133:8080/vde.txt";  // URL du fichier à télécharger
    char outfilename[] = "C:/Users/d.txt";  // Chemin du fichier de sortie

    curl = curl_easy_init();  // Initialiser cURL
    if (curl) {
        fp = fopen(outfilename, "wb");  // Ouvrir le fichier local en mode binaire
        if (!fp) {
            perror("fopen");
            return 1;
        }

        // Configurer cURL pour l'URL et la fonction d'écriture
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Activer le mode verbeux pour le débogage
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);

        // Exécuter la requête
        res = curl_easy_perform(curl);

        // Vérifier les erreurs
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Nettoyer
        fclose(fp);
        curl_easy_cleanup(curl);
    }

    return 0;
}
