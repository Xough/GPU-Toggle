#include <windows.h>
#include <stdio.h>

void executeCommand(const char *command) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; // Cache la fenêtre cmd

    ZeroMemory(&pi, sizeof(pi));

    // Crée un processus pour exécuter la commande
    if (!CreateProcess(
            NULL,   // Nom de l'application
            (LPSTR)command, // Commande à exécuter
            NULL,   // Sécurité du processus
            NULL,   // Sécurité du thread
            FALSE,  // Héritage des handles
            0,      // Flags de création
            NULL,   // Variables d'environnement
            NULL,   // Répertoire courant
            &si,    // Infos de démarrage
            &pi)) { // Infos du processus
        printf("Erreur: Impossible de créer le processus (%lu).\n", GetLastError());
        return;
    }

    // Attente de la fin de l'exécution du processus
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Ferme les handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const char *gpuID = "PCI\\VEN_XXXX&DEV_XXXX"; // Remplacez par l'identifiant matériel de votre GPU
    const char *devconPath = "C:\\path\\to\\devcon.exe"; // Chemin complet vers devcon.exe

    // Activer le GPU
    char enableCommand[512];
    snprintf(enableCommand, sizeof(enableCommand), "\"%s\" enable \"%s\"", devconPath, gpuID);
    executeCommand(enableCommand);

    // Désactiver le GPU
    char disableCommand[512];
    snprintf(disableCommand, sizeof(disableCommand), "\"%s\" disable \"%s\"", devconPath, gpuID);
    executeCommand(disableCommand);

    return 0;
}
