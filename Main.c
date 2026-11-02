#include <switch.h>
#include <stdio.h>
#include <stdbool.h>

// ==========================================
// 1. DATA STRUCTURES & STATES
// ==========================================

typedef enum {
    CONTROL_TRAINER,
    CONTROL_POKEMON,
    CONTROL_LAST_STAND // Triggered when Trainer HP reaches 0
} ControlState;

typedef struct {
    ControlState state;
    bool is_flying_type;
    bool is_flying_airborne;
    bool moveset_overlay_active;
} PokemonControlContext;

typedef struct {
    float current_hp;
    float max_hp;
    int   canari_plush_count;        // 0, 1, 2, or 3
    bool  is_blacked_out;
    u64   last_damage_time;          // Timestamp for recharge delay
    u64   blackout_timestamp;        // Timestamp when Last Stand started
    u32   last_stand_duration_ms;   // Timer duration (45 seconds = 45000 ms)
} TrainerHealthContext;

// Global Contexts
static PokemonControlContext g_ctx = {
    .state = CONTROL_TRAINER,
    .is_flying_type = false,
    .is_flying_airborne = false,
    .moveset_overlay_active = false
};

static TrainerHealthContext g_trainer_hp = {
    .current_hp = 60.0f,
    .max_hp = 60.0f,
    .canari_plush_count = 0,
    .is_blacked_out = false,
    .last_damage_time = 0,
    .blackout_timestamp = 0,
    .last_stand_duration_ms = 45000 // 45-second last stand timer
};

// ==========================================
// 2. HELPER & RECHARGE LOGIC
// ==========================================

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
    // Only recharge if alive and under max capacity
    if (!g_trainer_hp.is_blacked_out && g_trainer_hp.current_hp < g_trainer_hp.max_hp) {
        // 3-second delay after taking damage before shield recharges
        if ((current_time_ms - g_trainer_hp.last_damage_time) >= 3000) { 
            g_trainer_hp.current_hp += 1.5f; // Recharge tick speed
            
            if (g_trainer_hp.current_hp > g_trainer_hp.max_hp) {
                g_trainer_hp.current_hp = g_trainer_hp.max_hp;
            }
        }
    }
}

void check_trainer_blackout_state(u64 current_time_ms) {
    // 1. Trigger Last Stand when Trainer HP drops to 0
    if (g_trainer_hp.current_hp <= 0.0f && !g_trainer_hp.is_blacked_out) {
        g_trainer_hp.current_hp = 0.0f;
        g_trainer_hp.is_blacked_out = true;
        g_trainer_hp.blackout_timestamp = current_time_ms;
        
        // Switch control state to Last Stand mode
        g_ctx.state = CONTROL_LAST_STAND;
    }

    // 2. Handle Last Stand Countdown
    if (g_ctx.state == CONTROL_LAST_STAND) {
        u64 elapsed = current_time_ms - g_trainer_hp.blackout_timestamp;
        
        if (elapsed >= g_trainer_hp.last_stand_duration_ms) {
            // Time expired! Trigger final blackout/defeat
            // Code Hook: Trigger native Game Over sequence
        }
    }
}

// ==========================================
// 3. INPUT HANDLING LOGIC
// ==========================================

void handle_pokemon_control_inputs(u64 kDown, u64 kHeld, bool is_flying) {
    g_ctx.is_flying_type = is_flying;

    // --- TOGGLE CONTROL MODE (ZL Button) ---
    if (kDown & KEY_ZL) {
        if (g_ctx.state == CONTROL_TRAINER) {
            g_ctx.state = CONTROL_POKEMON;
            g_ctx.moveset_overlay_active = true; // Auto-enable moveset UI
        } else if (g_ctx.state == CONTROL_POKEMON) {
            g_ctx.state = CONTROL_TRAINER;
            g_ctx.moveset_overlay_active = false;
            g_ctx.is_flying_airborne = false;
        }
    }

    // --- CONTROLLER LOGIC WHILE IN POKEMON OR LAST STAND MODE ---
    if (g_ctx.state == CONTROL_POKEMON || g_ctx.state == CONTROL_LAST_STAND) {
        
        // Dash / Air Speed Boost (ZR Button at full speed)
        if (kHeld & KEY_ZR) {
            // Code Hook: Apply speed vector multiplier
        }

        // Flying Type Controls
        if (g_ctx.is_flying_type) {
            // Takeoff / Land Toggle (Left Stick Press - L3)
            if (kDown & KEY_LSTICK) {
                g_ctx.is_flying_airborne = !g_ctx.is_flying_airborne;
            }

            // Banking / Dodging
            if (kDown & KEY_L) { /* Code Hook: Bank Left */ }
            if (kDown & KEY_R) { /* Code Hook: Bank Right */ }
        } 
        // Non-Flying Type Controls
        else {
            // Block (L Button) - Requires Protect/Detect
            if (kHeld & KEY_L) { /* Code Hook: Shield Stance */ }

            // Directional Dodge (R Button + Left Stick)
            if ((kHeld & KEY_R) && (kDown & KEY_LSTICK)) {
                /* Code Hook: Execute Directional Roll */
            }
        }
    }
}

// ==========================================
// 4. HUD OVERLAY & LAST STAND RENDER HOOK
// ==========================================

void render_trainer_hp_hud(u64 current_time_ms) {
    int hud_x = 100;
    int hud_y = 650; // Positioned directly above Pokémon HP bar
    int bar_width = 250;
    int bar_height = 12;

    update_trainer_max_hp();

    // Standard HUD Bar Rendering
    if (!g_trainer_hp.is_blacked_out) {
        float hp_percentage = g_trainer_hp.current_hp / g_trainer_hp.max_hp;
        int current_fill_width = (int)(bar_width * hp_percentage);

        // Draw HUD:
        // 1. Draw Background Frame at (hud_x, hud_y)
        // 2. Draw Current HP Bar (Cyan/Blue Fill) with width = current_fill_width
        // 3. Draw Text: "TRAINER HP: [current] / [max]"
    } 
    // Last Stand Emergency HUD Rendering
    else {
        u64 elapsed = current_time_ms - g_trainer_hp.blackout_timestamp;
        u32 remaining_ms = (elapsed < g_trainer_hp.last_stand_duration_ms) 
                           ? (g_trainer_hp.last_stand_duration_ms - elapsed) : 0;
        
        float seconds_remaining = remaining_ms / 1000.0f;

        // Draw Emergency HUD:
        // 1. Flash Trainer Bar Red / Warning Border
        // 2. Draw Text: "TRAINER BLACKED OUT - LAST STAND: XX.X s"
    }
}