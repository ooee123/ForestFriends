#include "CPFF.h"
#include "CircularBuffer.h"

static Path paths[512];
static short len = 0;
static short cur = 0;

char enqueue(short x, short y, MovementType type) {
  if (len <= cur + QUEUE_SIZE) {
    short index = len % QUEUE_SIZE;
    paths[index].x = x;
    paths[index].y = y;
    paths[index].type = type;
    len++;
    return 1;
  }
  else
  {
    return 0;
  }
}

Path dequeue() {
  if (cur < len)
  {
    return paths[cur++];
  }
  else
  {
    Path p;
    return p
  }
}
