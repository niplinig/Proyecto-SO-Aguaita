
#include <stdio.h>

void *angle1_right_func(void *args) {

  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle1_mutex);
  if (angle1 > 0.0) { // positive
    pthread_mutex_unlock(&angle1_mutex);
    *result = 0;
    printf("angle1_right_func returning angle1 is positive\n");
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

  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d\n", fuel);
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
	      fuel = fuel - 5;
	      printf("⛽ Fuel %d\n", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle1_mutex);
	    angle1 = angle1 + 0.5;
	    printf("Plane 1 | 🫷  Tilting left | ✅ Right engine ON | Angle %2.2f\n"
	           , angle1);
	    pthread_mutex_unlock(&angle1_mutex);
	  }
	  pthread_mutex_unlock(&angle1_mutex);

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

  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle1_mutex);
  if (angle1 < 0.0) { // negative
    pthread_mutex_unlock(&angle1_mutex);
    *result = 0;
    printf("angle1_left_func returning angle1 is negative\n");
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

  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d\n", fuel);
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
	      fuel = fuel - 5;
	      printf("⛽ Fuel %d\n", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle1_mutex);
	    angle1 = angle1 - 0.5;
	    printf("Plane 1 | 🫷  Tilting right | ✅ Left engine ON | Angle %2.2f\n"
	           , angle1);
	    pthread_mutex_unlock(&angle1_mutex);
	  }
	  pthread_mutex_unlock(&angle1_mutex);

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

  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle2_mutex);
  if (angle2 > 0.0) { // positive
    pthread_mutex_unlock(&angle2_mutex);
    *result = 0;
    printf("angle2_right_func returning angle2 is positive\n");
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

  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d\n", fuel);
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
	      fuel = fuel - 5;
	      printf("⛽ Fuel %d\n", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle2_mutex);
	    angle2 = angle2 + 0.5;
	    printf("Plane 2 | 🤚 Tilting forward | ✅ Foward engine ON | Angle %2.2f\n"
	           , angle2);
	    pthread_mutex_unlock(&angle2_mutex);
	  }
	  pthread_mutex_unlock(&angle2_mutex);

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

  int *result = malloc(sizeof(int));

  pthread_cond_wait(&distance_cond, &angle2_mutex);
  if (angle2 < 0.0) { // negative
    pthread_mutex_unlock(&angle2_mutex);
    *result = 0;
    printf("angle2_left_func returning angle2 is negative\n");
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

  if (fuel == 10) {
    pthread_mutex_unlock(&fuel_mutex);
    *result = -1;
    return (void *)result;
  } else if (fuel > 10) {
    fuel = fuel - 5;
    printf("⛽ Fuel %d\n", fuel);
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
	      fuel = fuel - 5;
	      printf("⛽ Fuel %d\n", fuel);
	      pthread_mutex_unlock(&fuel_mutex);
	    }
	
	    pthread_mutex_lock(&angle2_mutex);
	    angle2 = angle2 - 0.5;
	    printf("Plane 2 | ✋ Tilting backwards | ✅ Backward engine ON | Angle %2.2f\n"
	           , angle2);
	    pthread_mutex_unlock(&angle2_mutex);
	  }
	  pthread_mutex_unlock(&angle2_mutex);

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