#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // Pobranie PID bieżącego procesu
    pid_t pid = getpid();

    // Pobranie UID, GID, PPID oraz PGID dla procesu o danym PID
    uid_t uid = getuid();
    gid_t gid = getgid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(pid);

    // Wyświetlenie wyników
    printf("UID: %d\n", uid);
    printf("GID: %d\n", gid);
    printf("PID: %d\n", pid);
    printf("PPID: %d\n", ppid);
    printf("PGID: %d\n", pgid);

    return 0;
}

