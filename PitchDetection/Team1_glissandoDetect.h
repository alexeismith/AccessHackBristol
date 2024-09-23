#define FRAME_SIZE 32

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
    int length;    // Length of the detected trend (number of frequencies)
    float extent;     // Extent of the pitch slide (max frequency - min frequency)
} PitchSlideResult;

// Sensitivity threshold to ignore small fluctuations in frequency (e.g., noise)
const float SENSITIVITY_THRESHOLD = 0.1f;

class Gliss
{
private:
public:
    int counter;
    
    PitchSlideResult Gliss::detect_pitch_slide(float frequencies[FRAME_SIZE]);
};