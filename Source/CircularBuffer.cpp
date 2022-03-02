/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 10 Feb 2022 9:09:27pm
    Author:  Ethan Schneider

  ==============================================================================
*/

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
    buffer = juce::AudioSampleBuffer();
    writeIndex = readIndex = delayLength = 0;
}

CircularBuffer::CircularBuffer(int bufferSize, int delayLength, int numChannels = 1)
{
    buffer = juce::AudioSampleBuffer(numChannels, bufferSize);
    buffer.clear();
    writeIndex = delayLength;
    readIndex = 0;
    this->delayLength = delayLength;
}

float CircularBuffer::getData()
{
    return buffer.getSample(0, readIndex);
}

void CircularBuffer::setData(float data, int destChannel = 0)
{
    buffer.setSample(destChannel, writeIndex, data);
    writeIndex = (writeIndex + 1) % buffer.getNumSamples();
}

void CircularBuffer::nextSample()
{
    int bufferLength = buffer.getNumSamples();
    readIndex = ((bufferLength + writeIndex) - delayLength) % bufferLength;
    writeIndex = (writeIndex + 1) % bufferLength;
}

float CircularBuffer::getRMSLevel(int channel, int startSample, int numSamples)
{
    return buffer.getRMSLevel(channel, startSample, numSamples);
}
