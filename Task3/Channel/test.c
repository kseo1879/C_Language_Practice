#include "channel.c"
#include "channel.h"
#include "stdio.h"
#include "stdlib.h"

int main() {

    struct receiver *recv = (struct receiver *)malloc(sizeof(struct receiver));
    struct sender *sender = (struct sender *)malloc(sizeof(struct sender));

    channel_init(recv, sender, 10);

    char *hello = "Hello World1234567891011";

    channel_send(sender, hello);

    void *string = (void *)malloc(sizeof(char) * 30);

    channel_get(recv, string);

    char *string2 = (char *)string;

    printf("The reciever recieved %s\n", string2);

    return 0;
}