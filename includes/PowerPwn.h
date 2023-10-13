#include <cstdint>

#ifndef PowerPwn
#define PowerPwn

const uint8_t CMDLINE_BUFFER = 32;
const uint16_t BUFFER = 1024;

struct commands {
    std::string exit;
    std::string help;
};

uint8_t sd(void);
void get_input(char input[BUFFER]);
void new_line(void);
void green(void);
void red(void);
void help(void);
int options(void);
void main_loop(void);
void create_connection(char ip[CMDLINE_BUFFER]);

#endif