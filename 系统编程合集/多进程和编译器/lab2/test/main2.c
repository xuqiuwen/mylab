#include <spawn.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    char *argv[] = {"echo", "hello", NULL};
    char *envp[] = {NULL};

    int status = posix_spawn(&pid, "/bin/echo", NULL, NULL, argv, envp);
    if (status == 0) {
        waitpid(pid, &status, 0);
    } else {
        printf("posix_spawn error\n");
    }

    return 0;
}
