#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MESSAGE_SIZE 32

int main() {
  char input_buffer[16];
  char *message = "Hello from Parent";
  int pipe_file_descriptor[2];
  if (pipe(pipe_file_descriptor) < 0) {
    perror("Failed to initialize pipe");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("Failed at fork");
    exit(1);
  }

  pid_t current_pid = getpid();
  uint8_t is_child = pid == 0;
  if (is_child) {
    sleep(2);
    read(pipe_file_descriptor[0], input_buffer, MESSAGE_SIZE);
    printf("Child Process: Received \"%s\"\n", input_buffer);
  } else {
    write(pipe_file_descriptor[1], message, MESSAGE_SIZE);
    printf("Parent Process: Writing \"%s\"\n", message);
    waitpid(pid, NULL, 0);
  }
  return 0;
}
