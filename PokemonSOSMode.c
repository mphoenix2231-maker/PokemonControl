typedef enum {
    CONTROL_TRAINER,
    CONTROL_POKEMON,
    CONTROL_SOS_MODE // New state: Trainer fainted, Pokémon leading rescue
} ControlState;

typedef struct {
    bool satchel_enabled;         // False during SOS Mode (Trainer only)
    bool party_summary_active;    // Replaces Satchel slot on the right side
    bool pokeballs_enabled;       // False during SOS Mode
    bool menu_access_limited;     // True: Only Moves, Party Summary, and Boxes allowed
} SOSMenuRestrictions;

static SOSMenuRestrictions g_sos_menu = {
    .satchel_enabled = true,
    .party_summary_active = false,
    .pokeballs_enabled = true,
    .menu_access_limited = false
};

void enter_pokemon_sos_mode() {
    g_ctx.state = CONTROL_SOS_MODE;
    
    // Apply menu restrictions per your design
    g_sos_menu.satchel_enabled = false;       // Satchel greyed out
    g_sos_menu.party_summary_active = true;   // Replaces Satchel slot on the right
    g_sos_menu.pokeballs_enabled = false;     // Pokéballs greyed out
    g_sos_menu.menu_access_limited = true;    // Restrict actions to Party Summary, Boxes, & Moves
    
    // Code Hook: Lock camera onto the Pokémon escorting/carrying the fainted Trainer or navigating solo to safety
}

void handle_sos_menu_inputs(u64 kDown) {
    if (g_ctx.state == CONTROL_SOS_MODE) {
        
        // Attempting to open Satchel
        if (kDown & KEY_SATCHEL_INPUT) {
            // Play error buzz sound: Satchel is greyed out!
        }
        
        // Attempting to throw / use Pokéball
        if (kDown & KEY_POKEBALL_INPUT) {
            // Play error buzz sound: Pokéballs disabled during SOS Mode!
        }
        
        // Selecting Party Summary (which replaced Satchel on the right)
        if (kDown & KEY_PARTY_SUMMARY_INPUT) {
            // Open full party stat/detail view without item-use permissions
        }
    }
}