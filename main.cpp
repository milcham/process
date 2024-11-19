#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <vector>

int heap() {
    // Definiamo la dimensione in byte che vogliamo allocare
    std::size_t size = 1024 * 1024 * 1024; // 1 GB in byte

    try {
        // Allocazione dinamica di 1 GB di memoria
        char* buffer = new char[size];

        for (std::size_t i = 0; i < size; ++i) {
            buffer[i] = 0;  // Scrivi in ogni byte del buffer
        }

        std::cout << "Allocazione di 1 GB di memoria avvenuta con successo." << std::endl;

        std::this_thread::sleep_for(std::chrono::minutes(10));

        delete[] buffer;
    } catch (std::bad_alloc& e) {
        std::cerr << "Errore di allocazione della memoria: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int stack()
{
    // Definiamo la dimensione in byte che vogliamo allocare
    std::size_t size = 1024 * 1024 * 1024; // 1 GB in byte

    try {
        // Allocazione dinamica di 1 GB di memoria
        char buffer  [size];

        std::cout << "Allocazione di 1 GB di memoria avvenuta con successo." << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(10));
    } catch (std::bad_alloc& e) {
        std::cerr << "Errore di allocazione della memoria: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int process() {
    for (int i = 0; i < 10; ++i) {
        pid_t pid = fork();  // Crea un nuovo processo

        if (pid == 0) {
            // Codice eseguito solo dal processo figlio
            std::cout << "Processo figlio " << i << ": PID = " << getpid() << std::endl;
            std::this_thread::sleep_for(std::chrono::minutes(10));
        } else if (pid > 0) {
            // Codice eseguito solo dal processo padre
            std::cout << "Processo padre ha creato il figlio " << i << ": PID = " << pid << std::endl;
        } else {
            // Errore nella creazione del processo
            std::cerr << "Errore nella fork!" << std::endl;
            return 1;
        }
    }

    // Attende che tutti i processi figlio terminino
    for (int i = 0; i < 10; ++i) {
        wait(NULL);  // Attende un processo figlio qualsiasi
    }

    std::cout << "Tutti i processi figlio hanno terminato\n";
    return 0;
}

int zombie()
{
    pid_t pid = fork();  // Crea un nuovo processo

    if (pid == 0) {
        // Processo figlio
        std::cout << "Figlio: PID = " << getpid() << std::endl;
        _exit(0);  // Il processo figlio termina immediatamente
    } else if (pid > 0) {
        // Processo padre
        std::cout << "Padre: PID = " << getpid() << ", Figlio: PID = " << pid << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(10)); // Il processo padre attende senza fare la wait() sul processo figlio
        std::cout << "Padre: Ora sto facendo la wait()" << std::endl;
        wait(NULL);  // Finalmente recupera lo stato del figlio
    } else {
        std::cerr << "Fork fallita!" << std::endl;
        return 1;
    }

    return 0;
}

void threadFunction(int thread_id) {
    std::cout << "Thread " << thread_id << " in esecuzione\n";
}

int thread() {
    // Vettore per conservare i thread
    std::vector<std::thread> threads;

    // Creiamo 10 thread
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(threadFunction, i));
    }

    std::this_thread::sleep_for(std::chrono::minutes(10));

    // Uniamo i thread per aspettare che terminino
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Tutti i thread hanno terminato\n";
    return 0;
}

int main()
{
    // commentare o decomentare uno alla volta per verificare gli effetti
    // heap();
    // stack();
    // process();
    //zombie();
    thread();
    return 0;
}
