#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

enum Event_type {
    KILL,
    DEATH,
    DRAGON,
    BARON,
    TOWER
};

enum Team {
    BLUE,
    RED
};

struct Event {
    uint16_t time;
    enum Event_type evt;
    enum Team team;
    uint16_t flags;
};

int main () {
    uint16_t value = 0x1234;

    uint8_t *value_adr = (uint8_t *)&value;

    printf("memory[0] = %x\n", value_adr[0]);
    printf("memory[1] = %x\n", value_adr[1]);

    return 0;
}