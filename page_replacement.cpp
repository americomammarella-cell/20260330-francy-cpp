#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t fifo_page_faults(size_t capacity, const int requests[], size_t request_count) {

    if (capacity == 0) return request_count;
    if (request_count == 0) return 0;


    int* posizione = (int*)malloc(capacity * sizeof(int));
    for (size_t i = 0; i < capacity; i++) {
        posizione[i] = -1; // vuoto
    }

    size_t t = 0;
    size_t sostituire = 0;

    for (size_t i = 0; i < request_count; i++) {
        int pagina_corrente = requests[i];
        int check = 0; // imposto falso ossia 0

        // vedo se la pagina è già presente
        for (size_t j = 0; j < capacity; j++) {
            if (posizione[j] == pagina_corrente) {
                check = 1;
                break;
            }
        }

        // se la pagina non è stata trovata
        if (check == 0) {
            t++; // Registriamo un Page Fault
            posizione[sostituire] = pagina_corrente;
            sostituire++;

            // ricomincio da zero se arrivo alla fine
            if (sostituire == capacity) {
                sostituire = 0;
            }
        }
    }

    free(posizione);

    return t;
}

size_t lru_page_faults(size_t capacity, const int requests[], size_t request_count) {
    if (capacity == 0) return request_count;
    if (request_count == 0) return 0;

    int* posizione = (int*)malloc(capacity * sizeof(int));
    size_t* cronologia = (size_t*)malloc(capacity * sizeof(size_t));

    for (size_t i = 0; i < capacity; i++) {
        posizione[i] = -1;
        cronologia[i] = 0;
    }

    size_t t = 0; 

    for (size_t i = 0; i < request_count; i++) {
        int pagina_corrente = requests[i]; 
        int check = 0;
        int indice_trovato = -1;

        // pagina già presente
        for (size_t j = 0; j < capacity; j++) {
            if (posizione[j] == pagina_corrente) {
                check = 1;
                indice_trovato = (int)j;
                break;
            }
        }

        if (check == 1) {
            // Se trovata, aggiorniamo solo quando l'abbiamo usata
            cronologia[indice_trovato] = i;
        } else {
            // 2. Se non è stata trovata
            t++; 

            size_t sostituire = 0; // Inizializzo l'indice da sostituire

            // Cerco l'indice della pagina usata meno recentemente
            for (size_t j = 0; j < capacity; j++) {
                // Se trovo un posto vuoto lo scelgo e mi fermo
                if (posizione[j] == -1) {
                    sostituire = j;
                    break;
                }
                // Altrimenti cerco chi ha la cronologia più vecchia
                if (cronologia[j] < cronologia[sostituire]) {
                    sostituire = j;
                }
            }

            // 3. Sostituisco
            posizione[sostituire] = pagina_corrente;
            cronologia[sostituire] = i;
        }
    }

    free(posizione);
    free(cronologia);
    return t;
}

#ifdef __cplusplus
}
#endif

#endif