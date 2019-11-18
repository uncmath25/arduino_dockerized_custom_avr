const int ANALOG_WRITE_MAX = 255;

const int RGB_CHANNELS = 3;

const int LED_DUTY_CYCLES = 30; // Adjusts overall LED brightness (setting too high could blow out your LED, limit to ~50)

const int COLOR_INTERVAL_MICROSECONDS = 100; // Resolution period for running through all 3 RGB colors
const float COLOR_INTERVAL_SCALING = 0.77; // Adjustment factor needed to offset pin I/O overhead (optimized for 1-2 second "on" periods)
const int COLOR_RESOLUTION_CYCLES_PER_MILLISECOND = (1000 / COLOR_INTERVAL_MICROSECONDS) * COLOR_INTERVAL_SCALING;

const int COLOR_TRANSITION_INTERVAL_MILLISECONDS = 100; // Controls smoothness of LED transitions
