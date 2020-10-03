// falling_rising_platform.c.inc

void bhv_squishable_platform_loop(void) {
    if (!network_sync_object_initialized(o)) {
        struct SyncObject* so = network_init_object(o, 4000.0f);
        so->minUpdateRate = 5.0f;
        network_init_object_field(o, &o->oPlatformTimer);
        network_init_object_field(o, &o->header.gfx.scale[1]);
    }

    if ((((o->oPlatformTimer / 0x80) % 300) == 0) && network_owns_object(o)) {
        network_send_object(o);
    }

    o->header.gfx.scale[1] = (sins(o->oPlatformTimer) + 1.0) * 0.3 + 0.4;
    o->oPlatformTimer += 0x80;
}

void bhv_bitfs_sinking_platform_loop(void) {
    if (!network_sync_object_initialized(o)) {
        struct SyncObject* so = network_init_object(o, 4000.0f);
        so->minUpdateRate = 5.0f;
        network_init_object_field(o, &o->oPlatformTimer);
    }

    o->oPosY -=
        sins(o->oPlatformTimer)
        * 0.58; //! f32 double conversion error accumulates on Wii VC causing the platform to rise up
    o->oPlatformTimer += 0x100;
}

// TODO: Named incorrectly. fix
void bhv_ddd_moving_pole_loop(void) {
    obj_copy_pos_and_angle(o, o->parentObj);
}

void bhv_bitfs_sinking_cage_platform_loop(void) {
    if (!network_sync_object_initialized(o)) {
        struct SyncObject* so = network_init_object(o, 4000.0f);
        so->minUpdateRate = 5.0f;
        network_init_object_field(o, &o->oPlatformTimer);
    }

    if ((((o->oPlatformTimer / 0x100) % 60) == 0) && network_owns_object(o)) {
        network_send_object(o);
    }

    if (o->oBehParams2ndByte != 0) {
        if (o->oTimer == 0)
            o->oPosY -= 300.0f;
        o->oPosY += sins(o->oPlatformTimer) * 7.0f;
    } else
        o->oPosY -= sins(o->oPlatformTimer) * 3.0f;
    o->oPlatformTimer += 0x100;

}
