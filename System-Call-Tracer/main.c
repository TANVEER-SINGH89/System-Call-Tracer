#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --------- FILTER SUPPORT ---------
char *filters[10];
int filter_count = 0;

int should_print(const char *name) {
    if (filter_count == 0) return 1;

    for (int i = 0; i < filter_count; i++) {
        if (strcmp(name, filters[i]) == 0)
            return 1;
    }
    return 0;
}

// --------- SYSCALL NAME MAPPING ---------
const char* get_syscall_name(long syscall) {
    switch (syscall) {
        case 0:   return "read";
        case 1:   return "write";
        case 2:   return "open";
        case 3:   return "close";
        case 5:   return "fstat";
        case 9:   return "mmap";
        case 12:  return "brk";
        case 16:  return "ioctl";
        case 21:  return "access";
        case 60:  return "exit";
        case 217: return "getdents64";
        case 231: return "exit_group";
        case 257: return "openat";
        // Add more syscalls here when needed
        default:  return "unknown";
    }
}

// --------- MAIN ---------
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program> [--filter=sys1,sys2]\n", argv[0]);
        return 1;
    }

    // Parse filters
    for (int i = 2; i < argc; i++) {
        if (strncmp(argv[i], "--filter=", 9) == 0) {
            char *token = strtok(argv[i] + 9, ",");
            while (token != NULL) {
                filters[filter_count++] = token;
                token = strtok(NULL, ",");
            }
        }
    }

    pid_t child = fork();

    if (child == 0) {
        // Child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
        perror("exec failed");
        exit(1);
    }
    else {
        int status;
        struct user_regs_struct regs;
        int in_syscall = 0;

        wait(&status);

        while (1) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);

            if (WIFEXITED(status)) {
                printf("Child process exited\n");
                break;
            }

            ptrace(PTRACE_GETREGS, child, NULL, &regs);

            if (in_syscall == 0) {
                // syscall entry
                in_syscall = 1;
            }
            else {
                // syscall exit
                long syscall = regs.orig_rax;
                long ret = regs.rax;
                const char *name = get_syscall_name(syscall);

                if (should_print(name)) {
                    // Basic argument decoding
                    if (syscall == 1) { // write
                        printf("write(fd=%lld, size=%lld) = %ld\n",
                               regs.rdi, regs.rdx, ret);
                    }
                    else if (syscall == 0) { // read
                        printf("read(fd=%lld, size=%lld) = %ld\n",
                               regs.rdi, regs.rdx, ret);
                    }
                    else if (syscall == 257) { // openat
                        printf("openat(dirfd=%lld, pathname=..., flags=%lld) = %ld\n",
                               regs.rdi, regs.rdx, ret);
                    }
                    else {
                        printf("%s() = %ld\n", name, ret);
                    }
                }

                in_syscall = 0;
            }
        }
    }

    return 0;
}