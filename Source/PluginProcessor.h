/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

//==============================================================================
/**
*/
class FoundationsofArtIIAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FoundationsofArtIIAudioProcessor();
    ~FoundationsofArtIIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float threshold = 0.05f;

private:
    //==============================================================================
    std::atomic<float>* wetDryParameter;
    std::atomic<float>* lpfParameter;
    juce::AudioProcessorValueTreeState parameters;
    void loadFiles();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoundationsofArtIIAudioProcessor)
    
    //==============================================================================
    juce::AudioFormatManager audioFormatManager;
    
    // 1-20
    juce::AudioFormatReader* fart1AudioFormatReader { nullptr };
    juce::MemoryInputStream* fart1Input;
    juce::AudioFormatReaderSource* fart1AudioFormatReaderSource;
    
    // 21-40
    juce::AudioFormatReader* fart2AudioFormatReader { nullptr };
    juce::MemoryInputStream* fart2Input;
    juce::AudioFormatReaderSource* fart2AudioFormatReaderSource;
    
    // 41-60
    juce::AudioFormatReader* fart3AudioFormatReader { nullptr };
    juce::MemoryInputStream* fart3Input;
    juce::AudioFormatReaderSource* fart3AudioFormatReaderSource;
    
    // 61-80
    juce::AudioFormatReader* fart4AudioFormatReader { nullptr };
    juce::MemoryInputStream* fart4Input;
    juce::AudioFormatReaderSource* fart4AudioFormatReaderSource;
    
    // 81-99
    juce::AudioFormatReader* fart5AudioFormatReader { nullptr };
    juce::MemoryInputStream* fart5Input;
    juce::AudioFormatReaderSource* fart5AudioFormatReaderSource;
    
    // 100
    juce::AudioFormatReader* fart6AudioFormatReader { nullptr };
    juce::MemoryInputStream* fart6Input;
    juce::AudioFormatReaderSource* fart6AudioFormatReaderSource;
    
    int position;
    CircularBuffer circularBuffer;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lpf;
    void updateLpf();
};
