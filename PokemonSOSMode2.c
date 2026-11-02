typedef struct {
    bool trainer_in_tow;      // True if Pokémon is carrying/escorting the Trainer
    bool ambulance_dispatched;// True if arriving solo, triggering rescue cutscene
    bool recovery_in_progress;// True while the healing pod is active
} SOSRescueContext;

static SOSRescueContext g_sos_rescue = {
    .trainer_in_tow = false,
    .ambulance_dispected = false,
    .recovery_in_progress = false
};

void handle_pokemon_center_arrival(bool trainer_is_with_you) {
    g_sos_rescue.trainer_in_tow = trainer_is_with_you;

    if (g_ctx.state == CONTROL_SOS_MODE && !g_sos_rescue.recovery_in_progress) {
        g_sos_rescue.recovery_in_progress = true;

        if (g_sos_rescue.trainer_in_tow) {
            // --- BRANCH A: TRAINER IN TOW ---
            // 1. Clerk expression changes to shock/urgency
            // 2. Play animation: Pokémon gently lays Trainer down / places them into healing pod
            // 3. Trigger rapid pod medical sequence
        } else {
            // --- BRANCH B: TRAINER NOT IN TOW (SOLO ARRIVAL) ---
            g_sos_rescue.ambulance_dispatched = true;
            // 1. Clerk dialogue prompt: "Oh no! Stay right here, I'm dispatching an ambulance to get your Trainer!"
            // 2. Short transition/cutscene: Emergency ambulance siren sound effect, retrieving fainted Trainer from the streets
            // 3. Trainer brought inside and placed into the healing pod
        }

        // --- COMMON RECOVERY SEQUENCE ---
        // 1. Healing pod lights up and hums
        // 2. Restore Trainer HP back to full (60 - 400 HP based on Canari Plushies)
        // 3. Reset SOS mode flags and return control to the Trainer
        g_trainer_hp.current_hp = g_trainer_hp.max_hp;
        g_trainer_hp.is_blacked_out = false;
        g_ctx.state = CONTROL_TRAINER;
        g_sos_menu.satchel_enabled = true;
        g_sos_menu.pokeballs_enabled = true;
        g_sos_rescue.recovery_in_progress = false;
    }
}