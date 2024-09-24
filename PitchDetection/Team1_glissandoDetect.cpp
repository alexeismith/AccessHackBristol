#include "math.h"
#include "Team1_glissandoDetect.h"

#define LOG2 0.69314718056 // Natural log of 2

// Function to convert a frequency to cents relative to a reference frequency
static void frequencies_to_cents(const float *frequencies, float *cents, int size, float f_ref)
{
    for (int i = 0; i < size; i++)
    {
        if (frequencies[i] > 0)
        {
            // Calculate the number of cents relative to the reference frequency
            cents[i] = 1200.0f * (log(frequencies[i]) - log(f_ref)) / LOG2;
        }
        else
        {
            // Handle case where frequency is zero or negative (undefined cents)
            cents[i] = -INFINITY; // Use -infinity to signify undefined value
        }
    }
}

static void computeRateOfChange(float arr[FRAME_SIZE], float rates[FRAME_SIZE])
{

    rates[0] = 0.0f;
    for (int i = 1; i < FRAME_SIZE; i++)
    {
        rates[i - 1] = arr[i] - arr[i - 1];
    }
}

static void findLargestConstantChunk(float arr[FRAME_SIZE], float threshold, PitchSlideResult *result)
{
    int length = 0;
    float extent = 0.0f;
    int currentStart = 0;
    int currentLength = 0;
    float currentAvgChange = 0.0f;

    for (int i = 1; i < FRAME_SIZE; i++)
    {

        // if rateOfChange value is "constant" and "non-zero"
        if ((fabs(arr[i] - arr[currentStart]) <= threshold) && ((arr[i] > 1.0f) || (arr[i] < -1.0f)))
        {
            // Serial.printf("test\n");
            currentLength++;
            currentAvgChange += arr[i];
        }
        else
        {

            // check if current segment is longer than longest segment we have so far
            if (currentLength > length)
            {
                length = currentLength;
                extent = currentAvgChange / (float)length;
            }

            currentStart = i;
            currentLength = 0;
        }
    }

    // Final check at the end of the loop
    if (currentLength > length)
    {
        length = currentLength;
        extent = currentAvgChange / (float)length;
    }

    result->extent = extent;
    result->length = length;
}

PitchSlideResult detect_pitch_slide(float frequencies[FRAME_SIZE])
{

    PitchSlideResult result = {NO_SLIDE, 0, 0.0f}; // Initialize result
    float rates[FRAME_SIZE];
    float cents[FRAME_SIZE];

    frequencies_to_cents(frequencies, cents, FRAME_SIZE, REF_FREQUENCY);
    computeRateOfChange(cents, rates);
    findLargestConstantChunk(rates, CONSTANT_VEL_THRESHOLD, &result);

    // Serial.print("cents:");
    // Serial.print(cents[FRAME_SIZE - 1]);
    // Serial.print("\n");

    // Serial.printf("frequencies: %g %g %g %g %g\n", frequencies[0], frequencies[1], frequencies[2], frequencies[3], frequencies[4]);
    // Serial.printf("cents: %g %g %g %g %g\n", cents[0], cents[1], cents[2], cents[3], cents[4]);
    // Serial.printf("rateOfChange: %g %g %g %g %g\n", rates[0], rates[1], rates[2], rates[3], rates[4]);

    return result;
}