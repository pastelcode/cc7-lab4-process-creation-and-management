#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    perror("Failed at fork");
    exit(1);
  }

  pid_t current_pid = getpid();
  uint8_t is_child = pid == 0;
  if (is_child) {
    sleep(2);
    printf("Child Process: PID=%d, Parent PID=%d\n", current_pid, getppid());
  } else {
    waitpid(pid, NULL, WCONTINUED);
    printf("Parent Process: Child has finished execution.\n");
  }
  return 0;
}
