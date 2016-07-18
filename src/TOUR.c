#include "TOUR.h"

unsigned char TOUR_enabled;
unsigned char TOUR_current_octave = TOUR_DEFAULT_OCTAVE;
unsigned long TOUR_current_ticks_per_64th_note = TOUR_GET_TICKS_PER_64TH_NOTE_FROM_BPM(TOUR_DEFAULT_BPM);

unsigned long TOUR_tick;

unsigned char TOUR_remained_duration = 1;
unsigned int TOUR_current_width;

unsigned int TOUR_uwaaang_width_start;
unsigned int TOUR_uwaaang_width_end;
unsigned int TOUR_uwaaang_duration;

unsigned char *TOUR_current_score;
unsigned char *TOUR_current_data;
