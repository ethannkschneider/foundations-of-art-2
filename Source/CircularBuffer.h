/*
  ==============================================================================

    CircularBuffer.h
    Created: 10 Feb 2022 9:09:27pm
    Author:  Ethan Schneider

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int bufferSize, int delayLength, int numSamples);
    float getData();
    void setData(float data, int destChannel);
    void nextSample();
    float getRMSLevel(int channel, int startSample, int numSamples);
private:
    juce::AudioSampleBuffer buffer;
    int writeIndex;
    int readIndex;
    int delayLength;
};
