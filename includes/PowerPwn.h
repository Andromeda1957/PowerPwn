#include <cstdint>

#ifndef PowerPwn
#define PowerPwn

const uint8_t CMDLINE_BUFFER = 32;
const uint16_t BUFFER = 1024;

struct commands {
    std::string shell;
    std::string exit;
    std::string help;
};

class Formatting {
  public:
    static void new_line(void);
    static void green(void);
    static void red(void);
    static void seperator(void);
};

class Modules {
  public:
    static void help(void);
    static void shell(void);
};

class Connection {
  public:
    static uint8_t sd(void);
    static void create_connection(char ip[CMDLINE_BUFFER]);
};

void get_input(char input[BUFFER]);
int options(char *option, struct commands *cmd);
void main_loop(void);

#endif