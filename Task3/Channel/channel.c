#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "channel.h"

void channel_init(struct receiver* recv, struct sender* sender, 
  size_t msg_sz) {
    
  if(NULL == recv || NULL == sender) {
      return;
  }

  int res = pipe(recv->fd);
  if(0 > res) {
    perror("Init could not produce pipe\n");
  }

  sender -> fd[0] = recv -> fd[0];
  sender -> fd[1] = recv -> fd[1];
  recv -> data = (void*)malloc(msg_sz);
  sender -> msg_size = msg_sz;
  recv -> msg_size = msg_sz;
}

void channel_get(struct receiver* channel, void* data) {
  if(NULL == data || NULL == channel) {
    return;
  }
  read(channel -> fd[0], channel -> data, channel -> msg_size);
  memcpy(data, channel -> data,  channel -> msg_size);
  return;   
}

void channel_send(struct sender* channel, void* data) {
  if(NULL == data || NULL == channel) {
    return;
  }
  write(channel -> fd[1], data, channel -> msg_size);
}

void sender_dup(struct sender* dest, struct sender* src) {
  if (dest == NULL || src == NULL) {
    return;
  }
  dest -> fd[0] = src -> fd[0];
  dest -> fd[1] = src -> fd[1];
  dest -> msg_size = src -> msg_size;
}

void channel_destroy(struct receiver* recv, struct sender* sender) {
    if(NULL == recv && NULL == sender) {
      return;
    }

    if(NULL != recv) {
      if(NULL != recv -> data) {
        free(recv -> data);
        recv -> data = NULL;
      }
    }
}   