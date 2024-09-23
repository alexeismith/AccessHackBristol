#include "math.h"
#include "Team1_glissandoDetect.h"
#include <SerialFlash.h>

void computeRateOfChange(float arr[FRAME_SIZE], float rates[FRAME_SIZE - 1])
{
    for (int i = 1; i < FRAME_SIZE; i++)
    {
        rates[i - 1] = arr[i] - arr[i - 1];
    }
}

void findLargestConstantChunk(float arr[FRAME_SIZE], float threshold, PitchSlideResult *result)
{
    int startIdx = 0;
    int length = 0;
    int currentStart = 0;
    int currentLength = 0;

    for (int i = 1; i < FRAME_SIZE; i++)
    {

        // if rateOfChange value is "constant" and "non-zero"
        if ((fabs(arr[i] - arr[currentStart]) <= threshold) && ((arr[i] > 0.5) || (arr[i] < -0.5)))
        {
            // Serial.printf("test\n");
            currentLength++;
        }
        else
        {

            // check if current segment is longer than longest segment we have so far
            if (currentLength > length)
            {
                startIdx = currentStart;
                length = currentLength;
            }

            currentStart = i;
            currentLength = 0;
        }
    }

    // Final check at the end of the loop
    if (currentLength > length)
    {
        startIdx = currentStart;
        length = currentLength;
    }

    result->length = length;
}

PitchSlideResult Gliss::detect_pitch_slide(float frequencies[FRAME_SIZE])
{

    PitchSlideResult result = {NO_SLIDE, 0, 0.0f}; // Initialize result
    float min_freq = 0;
    float max_freq = 0;
    float rates[FRAME_SIZE];
    int *glissStart;
    int *glissLength;

    computeRateOfChange(frequencies, rates);

    // Serial.printf("frequencies: %g %g %g %g %g\n", frequencies[0], frequencies[1], frequencies[2], frequencies[3], frequencies[4]);
    // Serial.printf("rateOfChange: %g %g %g %g %g\n", rates[0], rates[1], rates[2], rates[3], rates[4]);

    findLargestConstantChunk(rates, 50.0f, &result);

    return result;
}