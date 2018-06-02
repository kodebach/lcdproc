#ifndef CHRONO_H
#define CHRONO_H

int clock_screen(int rep, int display, int *flags_ptr, KeySet* config);
int uptime_screen(int rep, int display, int *flags_ptr, KeySet* config);
int time_screen(int rep, int display, int *flags_ptr, KeySet* config);
int big_clock_screen(int rep, int display, int *flags_ptr, KeySet* config);
int mini_clock_screen(int rep, int display, int *flags_ptr, KeySet* config);

#endif
