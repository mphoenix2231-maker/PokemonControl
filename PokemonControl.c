#include <switch.h>
#include <stdio.h>
#include <stdbool.h>

// Control state definitions
typedef enum {
    CONTROL_TRAINER,
    CONTROL_POKEMON
} ControlState;

typedef struct {
    ControlState state;
    bool is_flying_type;
    bool is_flying_airborne;
    bool moveset_overlay_active;
} PokemonControlContext;

static PokemonControlContext g_ctx = {
    .state = CONTROL_TRAINER,
    .is_flying_type = false,
    .is_flying_airborne = false,
    .moveset_overlay_active = false
};

void handle_pokemon_control_inputs(u64 kDown, u64 kHeld, bool is_flying) {
    g_ctx.is_flying_type = is_flying;

    // --- TOGGLE CONTROL MODE (ZL Button) ---
    if (kDown & KEY_ZL) {
        if (g_ctx.state == CONTROL_TRAINER) {
            g_ctx.state = CONTROL_POKEMON;
            g_ctx.moveset_overlay_active = true; // Always enable moveset list automatically
            // Code hook: Transfer camera & player input mapping to active Pokemon entity
        } else {
            g_ctx.state = CONTROL_TRAINER;
            g_ctx.moveset_overlay_active = false;
            g_ctx.is_flying_airborne = false;
            // Code hook: Return camera & player input mapping to Trainer entity
        }
    }

    // --- CONTROLLER LOGIC WHILE IN POKEMON MODE ---
    if (g_ctx.state == CONTROL_POKEMON) {
        
        // --- ALL TYPES: DASH / SPEED BOOST ---
        if (kHeld & KEY_ZR) {
            // Check if moving forward / full speed
            // Code hook: Apply speed multiplier to Pokemon movement vector
        }

        // --- FLYING TYPES CONTROLS ---
        if (g_ctx.is_flying_type) {
            
            // Toggle Takeoff / Landing (Left Thumbstick Button - L3)
            if (kDown & KEY_LSTICK) {
                g_ctx.is_flying_airborne = !g_ctx.is_flying_airborne;
                // Code hook: Trigger Takeoff/Landing flight animation & altitude physics
            }

            // Banking / Dodging
            if (kDown & KEY_L) {
                // Code hook: Execute Left Bank & Dodge vector
            }
            if (kDown & KEY_R) {
                // Code hook: Execute Right Bank & Dodge vector
            }

        } 
        // --- NON-FLYING TYPES CONTROLS ---
        else {
            
            // Block (L Button) - Requires Protect or Detect in current moveset
            if (kHeld & KEY_L) {
                // Code hook: Check if active moveset contains Protect or Detect
                // Code hook: Trigger Guard/Shield state animation
            }

            // Directional Dodge (R + Left Stick Input)
            if ((kHeld & KEY_R) && (kDown & KEY_LSTICK)) {
                // Code hook: Read Left Stick X/Y analog vectors to execute directional roll/dodge
            }
        }
    }
}