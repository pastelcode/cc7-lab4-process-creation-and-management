#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid;
  for (int index = 0; index < 3; index++) {
    pid = fork();
    if (pid < 0) {
      perror("Failed at fork");
      exit(1);
    }

    pid_t current_pid = getpid();
    uint8_t is_child = pid == 0;
    if (is_child) {
      printf("Child %d: PID=%d, Parent PID=%d\n", index + 1, current_pid,
             getppid());
      break;
    }
  }

  pid_t current_pid = getpid();
  uint8_t is_child = pid == 0;
  if (!is_child) {
    waitpid(0, NULL, 0);
    printf("Parent Process: PID=%d\n", current_pid);
  }
  return 0;
}
