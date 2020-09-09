#ifndef liquid_control_h
#define liquid_control_h

#define LIQUID_STATE_DISABLE  0  // Must be zero
#define LIQUID_STATE_1        bit(0)
#define LIQUID_STATE_2        bit(1)
#define LIQUID_STATE_3        bit(2)
#define LIQUID_STATE_4        bit(3)

// Initializes liquid control pins.
void liquid_init();

// Returns current liquid output state.
uint8_t liquid_get_state();

// Immediately disables liquid pins.
void liquid_stop();

// Sets the liquid pins according to state specified.
void liquid_set_state(uint16_t mode);

// G-code parser entry-point for setting liquid states. Checks for and executes additional conditions.
void liquid_sync(uint16_t mode);

#endif
