#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define MESSAGE_SIZE 32

int main() {
  char *message = "Hello from Parent";

  // Create shared memory segment
  int shm_id = shmget(IPC_PRIVATE, MESSAGE_SIZE, IPC_CREAT | 0666);
  if (shm_id < 0) {
    perror("Failed to create shared memory");
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

    // Attach to shared memory
    char *shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1) {
      perror("Failed to attach shared memory in child");
      exit(1);
    }

    printf("Child Process: Received \"%s\"\n", shm_ptr);

    // Detach from shared memory
    shmdt(shm_ptr);
  } else {
    // Attach to shared memory
    char *shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1) {
      perror("Failed to attach shared memory in parent");
      exit(1);
    }

    // Write message to shared memory
    strncpy(shm_ptr, message, MESSAGE_SIZE);
    printf("Parent Process: Writing \"%s\"\n", message);

    // Detach from shared memory
    shmdt(shm_ptr);

    waitpid(pid, NULL, 0);

    // Remove shared memory segment
    shmctl(shm_id, IPC_RMID, NULL);
  }

  return 0;
}
