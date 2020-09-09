/*
  liquid_control.c - liquid control methods
*/

#include "grbl.h"


void liquid_init()
{
  LIQUID_1_DDR |= (1 << LIQUID_1_BIT); // Configure as output pin.
  LIQUID_2_DDR |= (1 << LIQUID_2_BIT); // Configure as output pin.
  LIQUID_3_DDR |= (1 << LIQUID_3_BIT); // Configure as output pin.
  LIQUID_4_DDR |= (1 << LIQUID_4_BIT); // Configure as output pin.
  liquid_stop();
}


// Returns current liquid output state. Overrides may alter it from programmed state.
uint8_t liquid_get_state()
{
  uint8_t cl_state = LIQUID_STATE_DISABLE;
  #ifdef INVERT_LIQUID_1_PIN
    if (bit_isfalse(LIQUID_1_PORT,(1 << LIQUID_1_BIT)))
  #else
    if (bit_istrue(LIQUID_1_PORT,(1 << LIQUID_1_BIT)))
  #endif
  {
    cl_state |= LIQUID_STATE_1;
  }
  #ifdef INVERT_LIQUID_2_PIN
    if (bit_isfalse(LIQUID_2_PORT,(1 << LIQUID_2_BIT)))
  #else
    if (bit_istrue(LIQUID_2_PORT,(1 << LIQUID_2_BIT)))
  #endif
  {
    cl_state |= LIQUID_STATE_2;
  }
  #ifdef INVERT_LIQUID_3_PIN
    if (bit_isfalse(LIQUID_3_PORT,(1 << LIQUID_3_BIT)))
  #else
    if (bit_istrue(LIQUID_3_PORT,(1 << LIQUID_3_BIT)))
  #endif
  {
    cl_state |= LIQUID_STATE_3;
  }
  #ifdef INVERT_LIQUID_4_PIN
    if (bit_isfalse(LIQUID_4_PORT,(1 << LIQUID_4_BIT)))
  #else
    if (bit_istrue(LIQUID_4_PORT,(1 << LIQUID_4_BIT)))
  #endif
  {
    cl_state |= LIQUID_STATE_4;
  }
  return(cl_state);
}


// Directly called by liquid_init(), liquid_set_state(), and mc_reset(), which can be at
// an interrupt-level. No report flag set, but only called by routines that don't need it.
void liquid_stop()
{
  #ifdef INVERT_LIQUID_1_PIN
    LIQUID_1_PORT |= (1 << LIQUID_1_BIT);
  #else
    LIQUID_1_PORT &= ~(1 << LIQUID_1_BIT);
  #endif
  #ifdef INVERT_LIQUID_2_PIN
    LIQUID_2_PORT |= (1 << LIQUID_2_BIT);
  #else
    LIQUID_2_PORT &= ~(1 << LIQUID_2_BIT);
  #endif
  #ifdef INVERT_LIQUID_3_PIN
    LIQUID_3_PORT |= (1 << LIQUID_3_BIT);
  #else
    LIQUID_3_PORT &= ~(1 << LIQUID_3_BIT);
  #endif
  #ifdef INVERT_LIQUID_4_PIN
    LIQUID_4_PORT |= (1 << LIQUID_4_BIT);
  #else
    LIQUID_4_PORT &= ~(1 << LIQUID_4_BIT);
  #endif
}


// Main program only. Immediately sets l running state, 
// if enabled. Also sets a flag to report an update to a liquid state.
// Called by g-code parser liquid_sync().
void liquid_set_state(uint16_t mode)
{
  if (sys.abort) { return; } // Block during abort.  
  
  if (mode == LIQUID_DISABLE) {
  
    liquid_stop(); 
  
  } else {
  
    if (mode & LIQUID_1_ENABLE) {
      #ifdef INVERT_LIQUID_1_PIN
        LIQUID_1_PORT &= ~(1 << LIQUID_1_BIT);
      #else
        LIQUID_1_PORT |= (1 << LIQUID_1_BIT);
      #endif
    }
  
    if (mode & LIQUID_2_ENABLE) {
      #ifdef INVERT_LIQUID_2_PIN
        LIQUID_2_PORT &= ~(1 << LIQUID_2_BIT);
      #else
        LIQUID_2_PORT |= (1 << LIQUID_2_BIT);
      #endif
    }
  
    if (mode & LIQUID_3_ENABLE) {
      #ifdef INVERT_LIQUID_3_PIN
        LIQUID_3_PORT &= ~(1 << LIQUID_3_BIT);
      #else
        LIQUID_3_PORT |= (1 << LIQUID_3_BIT);
      #endif
    }
  
    if (mode & LIQUID_4_ENABLE) {
      #ifdef INVERT_LIQUID_4_PIN
        LIQUID_4_PORT &= ~(1 << LIQUID_4_BIT);
      #else
        LIQUID_4_PORT |= (1 << LIQUID_4_BIT);
      #endif
    }
  
  
  
    if (mode & LIQUID_1_DISABLE) {
      #ifdef INVERT_LIQUID_1_PIN
        LIQUID_1_PORT |= (1 << LIQUID_1_BIT);
      #else
        LIQUID_1_PORT &= ~(1 << LIQUID_1_BIT);
      #endif
    }
  
    if (mode & LIQUID_2_DISABLE) {
      #ifdef INVERT_LIQUID_2_PIN
        LIQUID_2_PORT |= (1 << LIQUID_2_BIT);
      #else
        LIQUID_2_PORT &= ~(1 << LIQUID_2_BIT);
      #endif
    }
  
    if (mode & LIQUID_3_DISABLE) {
      #ifdef INVERT_LIQUID_3_PIN
        LIQUID_3_PORT |= (1 << LIQUID_3_BIT);
      #else
        LIQUID_3_PORT &= ~(1 << LIQUID_3_BIT);
      #endif
    }
  
    if (mode & LIQUID_4_DISABLE) {
      #ifdef INVERT_LIQUID_4_PIN
        LIQUID_4_PORT |= (1 << LIQUID_4_BIT);
      #else
        LIQUID_4_PORT &= ~(1 << LIQUID_4_BIT);
      #endif
    }
  
  }
  sys.report_ovr_counter = 0; // Set to report change immediately
}


// G-code parser entry-point for setting liquid state. Forces a planner buffer sync and bails 
// if an abort or check-mode is active.
void liquid_sync(uint16_t mode)
{
  if (sys.state == STATE_CHECK_MODE) { return; }
  protocol_buffer_synchronize(); // Ensure liquid turns on when specified in program.
  liquid_set_state(mode);
}
