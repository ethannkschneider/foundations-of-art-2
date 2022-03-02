/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FoundationsofArtIIAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FoundationsofArtIIAudioProcessorEditor (FoundationsofArtIIAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~FoundationsofArtIIAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    enum
    {
        paramControlHeight = 40,
        paramLabelWidth    = 80,
        paramSliderWidth   = 300
    };
    
    enum Presets
    {
        jacksonPollack = 1,
        chileConCarne,
        streetMeat,
        hersheySquirts,
        fireInFireOut,
        skidmarks,
        cloudy
    };

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FoundationsofArtIIAudioProcessor& audioProcessor;
    
    void presetMenuChanged();
    void setCloudy();
    void setSkidmarks();
    void setFireInFireOut();
    void setHersheySquirts();
    void setStreetMeat();
    void setChileConCarne();
    void setJacksonPollack();
    
    void showAboutModal();
    
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Label wetDryLabel;
    juce::Slider wetDrySlider;
    std::unique_ptr<SliderAttachment> wetDryAttachment;
    
    juce::Label lpfLabel;
    juce::Slider lpfSlider;
    std::unique_ptr<SliderAttachment> lpfAttachment;
    
    juce::ComboBox presetMenu;
    
    juce::TextButton infoButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FoundationsofArtIIAudioProcessorEditor)
};
