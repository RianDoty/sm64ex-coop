#include <stdio.h>
#include "../network.h"
#include "game/interaction.h"
#include "game/level_update.h"
#define DISABLE_MODULE_LOG 1
#include "pc/debuglog.h"

void network_send_level(struct NetworkPlayer* toNp, bool sendArea) {
    extern s16 gCurrCourseNum, gCurrActNum, gCurrLevelNum;

    packet_ordered_begin();
    {
        struct Packet p;
        packet_init(&p, PACKET_LEVEL, true, false);

        // level location
        packet_write(&p, &gCurrCourseNum, sizeof(s16));
        packet_write(&p, &gCurrActNum,    sizeof(s16));
        packet_write(&p, &gCurrLevelNum,  sizeof(s16));

        // level variables
        packet_write(&p, &gMarioStates[0].numCoins, sizeof(s16));
        packet_write(&p, &gPssSlideStarted,         sizeof(u8));
        packet_write(&p, &gHudDisplay.timer,        sizeof(u16));

        // send level packet
        network_send_to(toNp->localIndex, &p);

        // send macro deletions
        network_send_level_macro(toNp);

        // send spawn info
        network_send_level_spawn_info(toNp);

        if (sendArea) {
            // send the area
            network_send_area(toNp);
        } else {
            // send sync valid
            network_send_sync_valid(toNp);
        }
    }
    packet_ordered_end();

    LOG_INFO("tx level");
}

void network_receive_level(struct Packet* p) {
    LOG_INFO("rx level");

    // read level location
    s16 courseNum, actNum, levelNum;
    packet_read(p, &courseNum,   sizeof(s16));
    packet_read(p, &actNum,      sizeof(s16));
    packet_read(p, &levelNum,    sizeof(s16));

    extern s16 gCurrCourseNum, gCurrActNum, gCurrLevelNum;
    if (courseNum != gCurrCourseNum || actNum != gCurrActNum || levelNum != gCurrLevelNum) {
        LOG_ERROR("rx level: received an improper location");
        return;
    }

    // read level variables
    packet_write(p, &gMarioStates[0].numCoins, sizeof(s16));
    packet_write(p, &gPssSlideStarted,         sizeof(u8));
    packet_write(p, &gHudDisplay.timer,        sizeof(u16));
}
