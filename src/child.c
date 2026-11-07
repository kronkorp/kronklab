#include "private.h"
#include <signal.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

int kl_run_child(struct __kl_funcStruct *f, int *sig)
{
    char exe[MAX_PATH];
    char cmd[MAX_PATH + 32];
    STARTUPINFOA si = {.cb = sizeof si};
    PROCESS_INFORMATION pi;
    DWORD code = 0;

    fflush(stdout);
    if (!GetModuleFileNameA(NULL, exe, sizeof exe)) {
        return -1;
    }
    snprintf(cmd, sizeof cmd, "\"%s\" " kl_CHILD_FLAG " %zu", exe, (size_t)(f - KL_BEGIN));
    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        return -1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    *sig = 0;
    /* Windows has no signals: crashes surface as NTSTATUS exit codes, abort() as 3. */
    if (code == 0xC0000005) {
        *sig = SIGSEGV;
    } else if (code == 3) {
        *sig = SIGABRT;
    } else if (code == 0xC0000094 || code == 0xC000008E) {
        *sig = SIGFPE;
    } else if (code >= 0xC0000000) {
        *sig = SIGTERM;
    }
    return *sig ? 0 : (int)code;
}
#else
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int kl_run_child(struct __kl_funcStruct *f, int *sig)
{
    int wstatus;
    int pid;

    fflush(stdout);
    pid = fork();
    if (pid == -1) {
        return -1;
    }
    if (pid == 0) {
        f->func();
        exit(0);
    }
    waitpid(pid, &wstatus, 0);
    *sig = WIFSIGNALED(wstatus) ? WTERMSIG(wstatus) : 0;
    return WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : 0;
}
#endif
