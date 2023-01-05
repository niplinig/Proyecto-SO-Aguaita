/*
  Includes
*/

// Boolean

#include <stdbool.h>

// Threads
#include <pthread.h>

// Standar symbols
#include <unistd.h>

// Sockets
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// String
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error
#include <error.h>

// Semaphores
#include <semaphore.h>

/*
        Definitions for server
*/
#define SERV_HOST_ADDR "127.0.0.1"
#define BUF_SIZE 100
#define BACKLOG 5

/*
        Global Variables
*/

// Semaphores
// sem_t fuel_sem, distance_sem;

// Thread Mutex
pthread_mutex_t angle1_mutex, angle2_mutex, fuel_mutex, distance_mutex,
    vertical_angle1_mutex, vertical_angle2_mutex;

/*
Shared memory
*/

// Integers
int interval, fuel, distance, port;

// Floats
float angle1, angle2;

// Booleans
bool vertical_angle1, vertical_angle2 = false;

// Conditional thread
pthread_cond_t distance_cond;

// Threads
pthread_t angle1_right_thread, angle1_left_thread, angle2_right_thread,
    angle2_left_thread, gravity_thread, server_thread;

/*
    Function Declarations
*/

int fuel_func(int decrement);
void *gravity_func(void *argss);
void *angle1_right_func(void *args);
void *angle1_left_func(void *args);
void *angle2_right_func(void *args);
void *angle2_left_func(void *args);

/*
  Functions
*/

int cancel_threads() {
  // destroy
  // pthread_mutex_destroy(&angle1_mutex);
  // pthread_mutex_destroy(&angle2_mutex);
  // pthread_mutex_destroy(&fuel_mutex);
  // pthread_mutex_destroy(&distance_mutex);

  // cancel
  pthread_cancel(angle1_right_thread);
  pthread_cancel(angle1_left_thread);
  pthread_cancel(angle2_right_thread);
  pthread_cancel(angle2_left_thread);
  return 0;
}

int create_threads() {
  // init
  // pthread_mutex_init(&angle1_mutex, NULL);
  // pthread_mutex_init(&angle2_mutex, NULL);
  // pthread_mutex_init(&fuel_mutex, NULL);
  // pthread_mutex_init(&distance_mutex, NULL);

  // create
  pthread_create(&angle1_right_thread, NULL, angle1_right_func, NULL);
  pthread_create(&angle1_left_thread, NULL, angle1_left_func, NULL);
  pthread_create(&angle2_right_thread, NULL, angle2_right_func, NULL);
  pthread_create(&angle2_left_thread, NULL, angle2_left_func, NULL);
  return 0;
}

void *gravity_func(void *arg) {
	int *result = malloc(sizeof(int));
	char print_dialog[100];
  cicle:
    if (distance <= 1) {
      *result = 0;
      return (void *)result;
    }
	
    usleep(interval * 1000);

    if (distance <= 5) {

      pthread_mutex_lock(&vertical_angle1_mutex);
      bool temp1 = vertical_angle1 == false;
      pthread_mutex_unlock(&vertical_angle1_mutex);

      pthread_mutex_lock(&vertical_angle2_mutex);
      bool temp2 = vertical_angle2 == false;
      pthread_mutex_unlock(&vertical_angle2_mutex);

      if (temp1 == true || temp2 == true) {
        cancel_threads();
        distance = 30;
        printf("▲ Distance: %d\n", distance);
        create_threads();
      }
    }
    
    distance = distance - 1;
    sprintf(print_dialog,"▼ Distance: %d ", distance);
	
    if (distance <= 100) {
      pthread_cond_broadcast(&distance_cond);

      pthread_mutex_lock(&fuel_mutex);
      if (fuel == 10) {
        pthread_mutex_unlock(&fuel_mutex);
        *result = -1;
        return (void *)result;
      } else if (fuel > 10) {
        fuel = fuel - 1;
        printf("%s ⛽ Fuel %d\n", print_dialog, fuel);
      }
      pthread_mutex_unlock(&fuel_mutex);
    }

  if (distance != 0) {
		goto cicle;
	}

  *result = 0;
  return (void *)result;
}

void *angle1_right_func(void *args) {
	char print_dialog[100];
  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle1_mutex);
  if (angle1 > 0.0) { // positive
    pthread_mutex_unlock(&angle1_mutex);
    *result = 0;
    printf("Right engine returning angle1 is positive\n");
    return (void *)result;
  }
  pthread_mutex_unlock(&angle1_mutex);

  pthread_mutex_lock(&angle1_mutex);
  if (angle1 == 0.0) {
    pthread_mutex_unlock(&angle1_mutex);

    pthread_mutex_lock(&vertical_angle1_mutex);
    if (vertical_angle1 == false)
      vertical_angle1 = true;
    pthread_mutex_unlock(&vertical_angle1_mutex);

    *result = 0;
    return (void *)result;
  }
  pthread_mutex_unlock(&angle1_mutex);

	pthread_mutex_lock(&fuel_mutex);
  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d ✅ Right engine ON\n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
  }

	cicle:
	  pthread_mutex_lock(&angle1_mutex);
	  if (angle1 < 0.0) { // negative
	    pthread_mutex_unlock(&angle1_mutex);
	
	    pthread_mutex_lock(&fuel_mutex);
	    if (fuel == 10) {
	      pthread_mutex_unlock(&fuel_mutex);
	      *result = -1;
	      return (void *)result;
	    }
	    if (fuel > 10) {
	      fuel = fuel - 1;
	      sprintf(print_dialog,"⛽ Fuel %d ", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle1_mutex);
	    angle1 = angle1 + 0.5;
	    printf("%s Plane 1 | 🫷  Tilting left | ✅ Right engine ON | Angle %2.2f\n"
	           , print_dialog, angle1);
	    pthread_mutex_unlock(&angle1_mutex);
	  }
	  pthread_mutex_unlock(&angle1_mutex);
		sleep(1);

  pthread_mutex_lock(&angle1_mutex);
  if (angle1 != 0.0) {
    pthread_mutex_unlock(&angle1_mutex);
    goto cicle;
  }
  pthread_mutex_unlock(&angle1_mutex);

  pthread_mutex_lock(&vertical_angle1_mutex);
  if (vertical_angle1 == false)
		vertical_angle1 = true;
  pthread_mutex_unlock(&vertical_angle1_mutex);

  *result = 0;
  return (void *)result;
}

void *angle1_left_func(void *args) {
	char print_dialog[100];
  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle1_mutex);
  if (angle1 < 0.0) { // negative
    pthread_mutex_unlock(&angle1_mutex);
    *result = 0;
    printf("Left engine returning angle1 is negative\n");
    return (void *)result;
  }
  pthread_mutex_unlock(&angle1_mutex);

  pthread_mutex_lock(&angle1_mutex);
  if (angle1 == 0.0) {
    pthread_mutex_unlock(&angle1_mutex);

    pthread_mutex_lock(&vertical_angle1_mutex);
    if (vertical_angle1 == false)
      vertical_angle1 = true;
    pthread_mutex_unlock(&vertical_angle1_mutex);

    *result = 0;
    return (void *)result;
  }
  pthread_mutex_unlock(&angle1_mutex);

	pthread_mutex_lock(&fuel_mutex);
  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d ✅ Left engine ON \n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
  }

	cicle:
	  pthread_mutex_lock(&angle1_mutex);
	  if (angle1 > 0.0) { // positive
	    pthread_mutex_unlock(&angle1_mutex);
	
	    pthread_mutex_lock(&fuel_mutex);
	    if (fuel == 10) {
	      pthread_mutex_unlock(&fuel_mutex);
	      *result = -1;
	      return (void *)result;
	    }
	    if (fuel > 10) {
	      fuel = fuel - 1;
	      sprintf(print_dialog, "⛽ Fuel %d ", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle1_mutex);
	    angle1 = angle1 - 0.5;
	    printf("%s Plane 1 | 🫷  Tilting right | ✅ Left engine ON | Angle %2.2f\n"
	           , print_dialog, angle1);
	    pthread_mutex_unlock(&angle1_mutex);
	  }
	  pthread_mutex_unlock(&angle1_mutex);
		sleep(1);

  pthread_mutex_lock(&angle1_mutex);
  if (angle1 != 0.0) {
    pthread_mutex_unlock(&angle1_mutex);
    goto cicle;
  }
  pthread_mutex_unlock(&angle1_mutex);

  pthread_mutex_lock(&vertical_angle1_mutex);
  if (vertical_angle1 == false)
		vertical_angle1 = true;
  pthread_mutex_unlock(&vertical_angle1_mutex);

  *result = 0;
  return (void *)result;
}

void *angle2_right_func(void *args) {
  char print_dialog[100];
  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle2_mutex);
  if (angle2 > 0.0) { // positive
    pthread_mutex_unlock(&angle2_mutex);
    *result = 0;
    printf("Foward engine returning angle2 is positive\n");
    return (void *)result;
  }
  pthread_mutex_unlock(&angle2_mutex);

  pthread_mutex_lock(&angle2_mutex);
  if (angle2 == 0.0) {
    pthread_mutex_unlock(&angle2_mutex);

    pthread_mutex_lock(&vertical_angle2_mutex);
    if (vertical_angle2 == false)
      vertical_angle2 = true;
    pthread_mutex_unlock(&vertical_angle2_mutex);

    *result = 0;
    return (void *)result;
  }
  pthread_mutex_unlock(&angle2_mutex);

	pthread_mutex_lock(&fuel_mutex);
  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d ✅ Foward engine ON\n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
  }

	cicle:
	  pthread_mutex_lock(&angle2_mutex);
	  if (angle2 < 0.0) { // negative
	    pthread_mutex_unlock(&angle2_mutex);
	
	    pthread_mutex_lock(&fuel_mutex);
	    if (fuel == 10) {
	      pthread_mutex_unlock(&fuel_mutex);
	      *result = -1;
	      return (void *)result;
	    }
	    if (fuel > 10) {
	      fuel = fuel - 1;
	      sprintf(print_dialog,"⛽ Fuel %d ", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle2_mutex);
	    angle2 = angle2 + 0.5;
	    printf("%s Plane 2 | 🤚 Tilting forward | ✅ Foward engine ON | Angle %2.2f\n"
	           , print_dialog , angle2);
	    pthread_mutex_unlock(&angle2_mutex);
	  }
	  pthread_mutex_unlock(&angle2_mutex);
		sleep(1);

  pthread_mutex_lock(&angle2_mutex);
  if (angle2 != 0.0) {
    pthread_mutex_unlock(&angle2_mutex);
    goto cicle;
  }
  pthread_mutex_unlock(&angle2_mutex);

  pthread_mutex_lock(&vertical_angle2_mutex);
  if (vertical_angle2 == false)
		vertical_angle2 = true;
  pthread_mutex_unlock(&vertical_angle2_mutex);

  *result = 0;
  return (void *)result;
}

void *angle2_left_func(void *args) {
  char print_dialog[100];
  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle2_mutex);
  if (angle2 < 0.0) { // negative
    pthread_mutex_unlock(&angle2_mutex);
    *result = 0;
    printf("Backward engine returning angle2 is negative\n");
    return (void *)result;
  }
  pthread_mutex_unlock(&angle2_mutex);

  pthread_mutex_lock(&angle2_mutex);
  if (angle2 == 0.0) {
    pthread_mutex_unlock(&angle2_mutex);

    pthread_mutex_lock(&vertical_angle2_mutex);
    if (vertical_angle2 == false)
      vertical_angle2 = true;
    pthread_mutex_unlock(&vertical_angle2_mutex);

    *result = 0;
    return (void *)result;
  }
  pthread_mutex_unlock(&angle2_mutex);

	pthread_mutex_lock(&fuel_mutex);
  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d ✅ Backward engine ON\n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
  }

	cicle:
	  pthread_mutex_lock(&angle2_mutex);
	  if (angle2 > 0.0) { // positive
	    pthread_mutex_unlock(&angle2_mutex);
	
	    pthread_mutex_lock(&fuel_mutex);
	    if (fuel == 10) {
	      pthread_mutex_unlock(&fuel_mutex);
	      *result = -1;
	      return (void *)result;
	    }
	    if (fuel > 10) {
	      fuel = fuel - 1;
	      sprintf(print_dialog, "⛽ Fuel %d ", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle2_mutex);
	    angle2 = angle2 - 0.5;
	    printf("%s Plane 2 | ✋ Tilting backwards | ✅ Backward engine ON | Angle %2.2f\n"
	           , print_dialog, angle2);
	    pthread_mutex_unlock(&angle2_mutex);
	  }
	  pthread_mutex_unlock(&angle2_mutex);
	  sleep(1);

  pthread_mutex_lock(&angle2_mutex);
  if (angle2 != 0.0) {
    pthread_mutex_unlock(&angle2_mutex);
    goto cicle;
  }
  pthread_mutex_unlock(&angle2_mutex);

  pthread_mutex_lock(&vertical_angle2_mutex);
  if (vertical_angle2 == false)
		vertical_angle2 = true;
  pthread_mutex_unlock(&vertical_angle2_mutex);

  *result = 0;
  return (void *)result;
}

void *server_func(void *args) {
  int *result = malloc(sizeof(int));

  // Server variables
  int socket_fd, connection_fd;
  unsigned int len;
  struct sockaddr_in server_addr, client_addr;

  int len_rx, len_tx = 0;
  char buff_tx[BUF_SIZE] = "Server succes 👋";
  char buff_rx[BUF_SIZE];

  /*
    Server initialization
  */

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Could not create the socket 🔴");
    *result = -1;
    return (void *)result;
  }
  puts("Socket created succesfully 🟢");

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  server_addr.sin_port = htons(port);

  /*
      Bind socket
*/
  if ((bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) ==
      -1) {
    perror("Server Biding Error 🔴");
    *result = -1;
    return (void *)result;
  }
  puts("Server bided succesfully 🟢");

  /*
    Listen socket
  */

  if ((listen(socket_fd, BACKLOG)) == -1) {
    perror("Server listen failed 🔴");
    *result = -1;
    return (void *)result;
  }
  puts("Server listening 🟢\n");

  len = sizeof(client_addr);

  while (1) {
    connection_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &len);
    if (connection_fd < 0) {
      perror("Server accept failed");
      *result = -1;
      return (void *)result;
    } else {
      while (1) {
        len_rx = read(connection_fd, buff_rx, sizeof(buff_rx));
        if (len_rx == -1) {
          perror("Nothing to read");
        } else if (len_rx == 0) {
          puts("[SERVER] Bye Client 👋");
          close(connection_fd);
          break;
        } else {
          int command = strtol(buff_rx, NULL, 10);
          //memset(buff_tx, 0, BUF_SIZE);
          switch (command) {
          case 101: // Genaral fail Restart Engines
            // restart_engines();
            strncpy(buff_tx, "[SERVER] Restart engines", BUF_SIZE);

            if (pthread_cancel(gravity_thread) != 0) {
              perror("Error cancelling the gravity_thread");
            }
            if (pthread_cancel(angle1_right_thread) != 0) {
              perror("Error cancelling the angle1_right_thread");
            }
            if (pthread_cancel(angle1_left_thread) != 0) {
              perror("Error cancelling the angle1_left_thread");
            }
            if (pthread_cancel(angle2_right_thread) != 0) {
              perror("Error cancelling the angle2_right_thread");
            }
            if (pthread_cancel(angle2_left_thread) != 0) {
              perror("Error cancelling the angle2_left_thread");
            }

            if (pthread_create(&gravity_thread, NULL, gravity_func, NULL) !=
                0) {
              perror("Error creating the gravity_thread");
            }
            if (pthread_create(&angle1_right_thread, NULL, angle1_right_func,
                               NULL) != 0) {
              perror("Error creating the angle1_right_thread");
            }
            if (pthread_create(&angle1_left_thread, NULL, angle1_left_func,
                               NULL) != 0) {
              perror("Error creating the angle1_left_thread");
            }
            if (pthread_create(&angle2_right_thread, NULL, angle2_right_func,
                               NULL) != 0) {
              perror("Error creating the angle2_right_thread");
            }
            if (pthread_create(&angle2_left_thread, NULL, angle2_left_func,
                               NULL) != 0) {
              perror("Error creating the angle2_left_thread");
            }

            write(connection_fd, buff_tx, BUF_SIZE);
            printf("[CLIENT] %s", buff_rx);
            break;
          case 102: // Principal engine fail Restart Principal engine
            strncpy(buff_tx, "[SERVER] Restart Principal engine", BUF_SIZE);

            if (pthread_cancel(gravity_thread) != 0) {
              perror("Error cancelling the gravity_thread");
            }

            if (pthread_create(&gravity_thread, NULL, gravity_func, NULL) !=
                0) {
              perror("Error creating the gravity_thread");
            }

            write(connection_fd, buff_tx, BUF_SIZE);
            printf("[CLIENT] %s", buff_rx);
            break;
          case 103: // Orientation engines fail Restart all Orientation engines
            strncpy(buff_tx, "[SERVER] Restart Orientation engines", BUF_SIZE);

            if (pthread_cancel(angle1_right_thread) != 0) {
              perror("Error cancelling the angle1_right_thread");
            }
            if (pthread_cancel(angle1_left_thread) != 0) {
              perror("Error cancelling the angle1_left_thread");
            }
            if (pthread_cancel(angle2_right_thread) != 0) {
              perror("Error cancelling the angle2_right_thread");
            }
            if (pthread_cancel(angle2_left_thread) != 0) {
              perror("Error cancelling the angle2_left_thread");
            }

            if (pthread_create(&angle1_right_thread, NULL, angle1_right_func,
                               NULL) != 0) {
              perror("Error creating the angle1_right_thread");
            }
            if (pthread_create(&angle1_left_thread, NULL, angle1_left_func,
                               NULL) != 0) {
              perror("Error creating the angle1_left_thread");
            }
            if (pthread_create(&angle2_right_thread, NULL, angle2_right_func,
                               NULL) != 0) {
              perror("Error creating the angle2_right_thread");
            }
            if (pthread_create(&angle2_left_thread, NULL, angle2_left_func,
                               NULL) != 0) {
              perror("Error creating the angle2_left_thread");
            }

            write(connection_fd, buff_tx, BUF_SIZE);
            printf("[CLIENT] %s", buff_rx);

            break;
          case 104: // Abort Mission
            strncpy(buff_tx, "[SERVER] Abort Mission", BUF_SIZE);

            if (pthread_cancel(gravity_thread) != 0) {
              perror("Error cancelling the gravity_thread");
            }
            if (pthread_cancel(angle1_right_thread) != 0) {
              perror("Error cancelling the angle1_right_thread");
            }
            if (pthread_cancel(angle1_left_thread) != 0) {
              perror("Error cancelling the angle1_left_thread");
            }
            if (pthread_cancel(angle2_right_thread) != 0) {
              perror("Error cancelling the angle2_right_thread");
            }
            if (pthread_cancel(angle2_left_thread) != 0) {
              perror("Error cancelling the angle2_left_thread");
            }
            printf("All threads successfuly cancelled\n");

            write(connection_fd, buff_tx, BUF_SIZE);
            printf("[CLIENT] %s", buff_rx);

            break;
          default:
            memset(buff_tx, 0, 100);
            strcpy(buff_tx, "[SERVER] Invalid command");
            write(connection_fd, buff_tx, BUF_SIZE);
            printf("[CLIENT] %s", buff_rx);
            break;
          }
        }
      }
    }
  }

  *result = 0;
  return (void *)result;
}

/*
  Main program
*/

int main(int argc, char *argv[]) {
  /*
          Validate Input
                          */
  if (argc != 7) {
    perror("Usage: interval, angle1, angle2, fuel, distance, port");
    return -1;
  }
  /*
                                  Initializing program variables
  */

  // Setting variables values
  interval = strtol(argv[1], NULL, 10);
  angle1 = strtof(argv[2], NULL);
  angle2 = strtof(argv[3], NULL);
  fuel = strtol(argv[4], NULL, 10);
  distance = strtol(argv[5], NULL, 10);
  port = strtol(argv[6], NULL, 10);

  // Initialize mutexes
  pthread_mutex_init(&angle1_mutex, NULL);
  pthread_mutex_init(&angle2_mutex, NULL);
  pthread_mutex_init(&fuel_mutex, NULL);
  pthread_mutex_init(&distance_mutex, NULL);
  pthread_mutex_init(&vertical_angle1_mutex, NULL);
  pthread_mutex_init(&vertical_angle2_mutex, NULL);

  // return variables
  int *gravity_return, *angle1_right_return, *angle1_left_return, *angle2_right_return, *angle2_left_return;

  // Input validation
  if (fuel < 0 || fuel > 100) {
    perror("Fuel must be: 0 <= fuel <= 100");
    return -1;
  }

  if (angle1 > 90 || angle1 < -90) {
    perror("Angle1 must be: -90 <= angle1 <= 90");
    return -1;
  }
  if (angle2 > 90 || angle2 < -90) {
    perror("Angle2 must be: -90 <= angle2 <= 90");
    return -1;
  }

  printf("interval: %d,\nangle1: %f,\nangle2: %f,\nfuel: %d,\ndistance: "
         "%d,\nport: %d\n\n",
         interval, angle1, angle2, fuel, distance, port);

  /*
          pthread condition creation
  */
  pthread_cond_init(&distance_cond, NULL);

  /*
                                                          Thread creations
  */

  if (pthread_create(&server_thread, NULL, server_func, NULL) != 0) {
    perror("Error creating the server_thread");
  }
  if (pthread_create(&angle1_right_thread, NULL, angle1_right_func, NULL) !=
      0) {
    perror("Error creating the angle1_right_thread");
  }
  if (pthread_create(&angle1_left_thread, NULL, angle1_left_func, NULL) != 0) {
    perror("Error creating the angle1_left_thread");
  }
  if (pthread_create(&angle2_right_thread, NULL, angle2_right_func, NULL) !=
      0) {
    perror("Error creating the angle2_right_thread");
  }
  if (pthread_create(&angle2_left_thread, NULL, angle2_left_func, NULL) != 0) {
    perror("Error creating the angle2_left_thread");
  }
  if (pthread_create(&gravity_thread, NULL, gravity_func, NULL) != 0) {
    perror("Error creating the gravity_thread");
  }

  /*
         Finishing the program
         Because landing successfully or run out of gas
 */

  pthread_join(gravity_thread, (void **)&gravity_return);

  if (*gravity_return == -1) {
    perror("\n⚠️  Low level fuel! 10%\n");

    if (pthread_cancel(angle1_right_thread) != 0) {
      perror("Error cancelling the angle1_right_thread");
    }
    if (pthread_cancel(angle1_left_thread) != 0) {
      perror("Error cancelling the angle1_left_thread");
    }
    if (pthread_cancel(angle2_right_thread) != 0) {
      perror("Error cancelling the angle2_right_thread");
    }
    if (pthread_cancel(angle2_left_thread) != 0) {
      perror("Error cancelling the angle2_left_thread");
    }

    // // Free memmory
    // free(gravity_return);

    // // Condition

    // pthread_cond_destroy(&distance_cond);

    // // Mutex

    // pthread_mutex_destroy(&angle1_mutex);
    // pthread_mutex_destroy(&angle2_mutex);
    // pthread_mutex_destroy(&fuel_mutex);
    // pthread_mutex_destroy(&distance_mutex);
    // pthread_mutex_destroy(&vertical_angle1_mutex);
    // pthread_mutex_destroy(&vertical_angle2_mutex);

    return -1;
  }

  pthread_join(angle1_right_thread, (void **)&angle1_right_return);

  if (*angle1_right_return == -1) {
    perror("\n⚠️  Low level fuel! 10%\n");

    if (pthread_cancel(angle1_left_thread) != 0) {
      perror("Error cancelling the angle1_left_thread");
    }
    if (pthread_cancel(angle2_right_thread) != 0) {
      perror("Error cancelling the angle2_right_thread");
    }
    if (pthread_cancel(angle2_left_thread) != 0) {
      perror("Error cancelling the angle2_left_thread");
    }

    // // Free memmory
    // free(angle1_right_return);

    // // Condition

    // pthread_cond_destroy(&distance_cond);

    // // Mutex

    // pthread_mutex_destroy(&angle1_mutex);
    // pthread_mutex_destroy(&angle2_mutex);
    // pthread_mutex_destroy(&fuel_mutex);
    // pthread_mutex_destroy(&distance_mutex);
    // pthread_mutex_destroy(&vertical_angle1_mutex);
    // pthread_mutex_destroy(&vertical_angle2_mutex);

    return -1;
  }

  pthread_join(angle1_left_thread, (void **)&angle1_left_return);

  if (*angle1_left_return == -1) {
    perror("\n⚠️  Low level fuel! 10%\n");

    // if (pthread_cancel(angle2_right_thread) != 0) {
    //   perror("Error cancelling the angle2_right_thread");
    // }
    // if (pthread_cancel(angle2_left_thread) != 0) {
    //   perror("Error cancelling the angle2_left_thread");
    // }

    // // Free memmory
    // free(angle1_left_return);

    // // Condition

    // pthread_cond_destroy(&distance_cond);

    // // Mutex

    // pthread_mutex_destroy(&angle1_mutex);
    // pthread_mutex_destroy(&angle2_mutex);
    // pthread_mutex_destroy(&fuel_mutex);
    // pthread_mutex_destroy(&distance_mutex);
    // pthread_mutex_destroy(&vertical_angle1_mutex);
    // pthread_mutex_destroy(&vertical_angle2_mutex);

    return -1;
  }

  pthread_join(angle2_right_thread, (void **)&angle2_right_return);

  if (*angle2_right_return == -1) {
    perror("\n⚠️  Low level fuel! 10%\n");

    if (pthread_cancel(angle2_left_thread) != 0) {
      perror("Error cancelling the angle2_left_thread");
    }

    // // Free memmory
    // free(angle2_right_return);

    // // Condition

    // pthread_cond_destroy(&distance_cond);

    // // Mutex

    // pthread_mutex_destroy(&angle1_mutex);
    // pthread_mutex_destroy(&angle2_mutex);
    // pthread_mutex_destroy(&fuel_mutex);
    // pthread_mutex_destroy(&distance_mutex);
    // pthread_mutex_destroy(&vertical_angle1_mutex);
    // pthread_mutex_destroy(&vertical_angle2_mutex);

    return -1;
  }

  pthread_join(angle2_left_thread, (void **)&angle2_left_return);

  if (*angle2_left_return == -1) {
    perror("\n⚠️  Low level fuel! 10%\n");
    
    // // Free memmory
    // free(angle2_left_return);

    // // Condition

    // pthread_cond_destroy(&distance_cond);

    // // Mutex

    // pthread_mutex_destroy(&angle1_mutex);
    // pthread_mutex_destroy(&angle2_mutex);
    // pthread_mutex_destroy(&fuel_mutex);
    // pthread_mutex_destroy(&distance_mutex);
    // pthread_mutex_destroy(&vertical_angle1_mutex);
    // pthread_mutex_destroy(&vertical_angle2_mutex);

    return -1;
  }

  if (*gravity_return == 0 && *angle1_right_return == 0 &&
      *angle1_left_return == 0 && *angle2_right_return == 0 &&
      *angle2_left_return == 0) {
    printf("\n🏳️  Perfect landing\n");
    // end routine
  }

  pthread_cancel(server_thread);

  // Destroy

  // Free memmory
  free(gravity_return);
  free(angle1_right_return);
  free(angle1_left_return);
  free(angle2_right_return);
  free(angle2_left_return);

  // Condition

  pthread_cond_destroy(&distance_cond);

  // Mutex

  pthread_mutex_destroy(&angle1_mutex);
  pthread_mutex_destroy(&angle2_mutex);
  pthread_mutex_destroy(&fuel_mutex);
  pthread_mutex_destroy(&distance_mutex);
  pthread_mutex_destroy(&vertical_angle1_mutex);
  pthread_mutex_destroy(&vertical_angle2_mutex);

  return 0;
}
