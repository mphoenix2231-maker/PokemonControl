#include <switch.h>
#include <stdio.h>
#include <stdbool.h>

// --- TRAINER HEALTH CONFIGURATION ---
typedef struct {
    float current_hp;
    float max_hp;
    int   canari_plush_count; // 0, 1, 2, or 3
    bool  is_taking_damage;
    u64   last_damage_time;   // Timestamp for recharge delay
} TrainerHealthContext;

static TrainerHealthContext g_trainer_hp = {
    .current_hp = 60.0f,
    .max_hp = 60.0f,
    .canari_plush_count = 0,
    .is_taking_damage = false,
    .last_damage_time = 0
};

// --- RECHARGE & MAX HP SCALING LOGIC ---
void update_trainer_max_hp() {
    switch (g_trainer_hp.canari_plush_count) {
        case 0:  g_trainer_hp.max_hp = 60.0f;  break;
        case 1:  g_trainer_hp.max_hp = 120.0f; break;
        case 2:  g_trainer_hp.max_hp = 240.0f; break;
        case 3:  
        default: g_trainer_hp.max_hp = 400.0f; break;
    }
}

void process_trainer_hp_recharge(u64 current_time_ms) {
    // If not max health and 3 seconds passed since taking damage, begin recharge
    if (g_trainer_hp.current_hp < g_trainer_hp.max_hp) {
        if ((current_time_ms - g_trainer_hp.last_damage_time) >= 3000) { 
            g_trainer_hp.current_hp += 1.5f; // Recharge rate tick
            
            if (g_trainer_hp.current_hp > g_trainer_hp.max_hp) {
                g_trainer_hp.current_hp = g_trainer_hp.max_hp;
            }
        }
    }
}

// --- HUD OVERLAY RENDER HOOK ---
void render_trainer_hp_hud() {
    // UI HUD Bar Dimensions (Positioned directly above Pokémon HP bar)
    int hud_x = 100; // X position
    int hud_y = 650; // Y position (sitting right on top of Pokemon HP bar)
    int bar_width = 250;
    int bar_height = 12;

    float hp_percentage = g_trainer_hp.current_hp / g_trainer_hp.max_hp;
    int current_fill_width = (int)(bar_width * hp_percentage);

    // Code Hook / Tesla Draw Call:
    // 1. Draw Outer Border / Background Bar (Dark Gray/Black)
    // 2. Draw Current HP Bar (e.g., Bright Cyan/Blue for rechargeable shield)
    // 3. Render Text Overlay: "TRAINER HP: [current] / [max]"
}