#define FRAME_SIZE 512

typedef enum
{
    SLIDE_UP,
    SLIDE_DOWN,
    STABLE,
    NO_SLIDE
} PitchTrend;

typedef struct
{
    PitchTrend trend; // Direction of the pitch slide (up, down, stable)
    int length;       // Length of the detected trend (number of frequencies)
    float extent;     // Extent of the pitch slide (max frequency - min frequency)
} PitchSlideResult;

PitchSlideResult detect_pitch_slide(float frequencies[FRAME_SIZE]);