#include <SerialFlash.h>

#define REPORT_PERIOD_MS 5

#define FRAME_SIZE 50
#define REF_FREQUENCY 718.0f
#define CONSTANT_VEL_THRESHOLD 100.0f

#define GLISS_DURATION_THRESHOLD 5
#define GLISS_EXTENT_THRESHOLD 50

typedef struct
{
    bool trend;   // Direction of the pitch slide (up, down, stable)
    int length;   // Length of the detected trend (number of frequencies)
    float extent; // Extent of the pitch slide (max frequency - min frequency)
} PitchSlideResult;

void detect_pitch_slide(float frequencies[FRAME_SIZE], PitchSlideResult *result);