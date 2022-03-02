/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FoundationsofArtIIAudioProcessorEditor::FoundationsofArtIIAudioProcessorEditor (FoundationsofArtIIAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    // labels
    wetDryLabel.setText("Wet/Dry", juce::dontSendNotification);
    addAndMakeVisible(wetDryLabel);
    wetDryLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    lpfLabel.setText("LPF (Liters per Flush)", juce::dontSendNotification);
    addAndMakeVisible(lpfLabel);
    lpfLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    // sliders
    addAndMakeVisible(wetDrySlider);
    wetDryAttachment.reset(new SliderAttachment (valueTreeState, "wetDry", wetDrySlider));
    wetDrySlider.setColour(juce::Label::textColourId, juce::Colours::black);
    wetDrySlider.setColour(juce::Slider::textBoxTextColourId , juce::Colours::black);
    wetDrySlider.setColour(juce::Slider::thumbColourId, juce::Colours::brown);
    
    addAndMakeVisible(lpfSlider);
    lpfAttachment.reset(new SliderAttachment (valueTreeState, "lpf", lpfSlider));
    lpfSlider.setColour(juce::Label::textColourId, juce::Colours::black);
    lpfSlider.setColour(juce::Slider::textBoxTextColourId , juce::Colours::black);
    lpfSlider.setColour(juce::Slider::thumbColourId, juce::Colours::brown);
    
    // preset dropdown
    addAndMakeVisible (presetMenu);
    
    presetMenu.setTextWhenNothingSelected("Select a preset");
    presetMenu.addItem ("Jackson Pollack",  1);
    presetMenu.addItem ("Chile Con Carne",   2);
    presetMenu.addItem ("Street Meat", 3);
    presetMenu.addItem ("\"The Hershey Squirts\"", 4);
    presetMenu.addItem ("Fire In, Fire Out", 5);
    presetMenu.addItem ("Skidmarks", 6);
    presetMenu.addItem ("Cloudy With A Chance of Meatballs", 7);
    presetMenu.onChange = [this] { presetMenuChanged(); };
    
    // info button
    infoButton.setButtonText("?");
    infoButton.setTitle("About");
    infoButton.setHelpText("Click for more info");
    infoButton.onClick = [this]{ showAboutModal();};
    addAndMakeVisible(infoButton);
    
    // set window size
    setSize (paramSliderWidth + paramLabelWidth + 40, juce::jmax (100, paramControlHeight * 6));
    
    // show the artist statement by default
    infoButton.triggerClick();
}

FoundationsofArtIIAudioProcessorEditor::~FoundationsofArtIIAudioProcessorEditor()
{
}

//==============================================================================
void FoundationsofArtIIAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::whitesmoke);
}

void FoundationsofArtIIAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    presetMenu.setBounds (10, paramControlHeight * 4, getWidth() - 20, 20);

    infoButton.setBounds((getWidth() / 2) - 10, paramControlHeight * 5, 20, 20);

    auto wetDryRect = area.removeFromTop (paramControlHeight * 2);
    wetDryLabel .setBounds (wetDryRect.removeFromLeft (paramLabelWidth));
    wetDrySlider.setBounds (wetDryRect);
    
    auto lpfRect = area.removeFromTop (paramControlHeight);
    lpfLabel .setBounds (lpfRect.removeFromLeft (paramLabelWidth));
    lpfSlider.setBounds (lpfRect);
}

void FoundationsofArtIIAudioProcessorEditor::setJacksonPollack()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 70.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 15000.0f;
}

void FoundationsofArtIIAudioProcessorEditor::setChileConCarne()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 30.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 8000.0f;
}

void FoundationsofArtIIAudioProcessorEditor::setStreetMeat()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 50.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 5000.0f;
}

void FoundationsofArtIIAudioProcessorEditor::setHersheySquirts()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 90.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 18000.0f;
}

void FoundationsofArtIIAudioProcessorEditor::setFireInFireOut()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 100.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 20000.0f;
}

void FoundationsofArtIIAudioProcessorEditor::setSkidmarks()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 10.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 7000.0f;
}

void FoundationsofArtIIAudioProcessorEditor::setCloudy()
{
    auto wetDry = valueTreeState.getParameterAsValue("wetDry");
    wetDry = 9.0f;
    
    auto lpf = valueTreeState.getParameterAsValue("lpf");
    lpf = 800.0f;
}

void FoundationsofArtIIAudioProcessorEditor::presetMenuChanged()
{
    switch (presetMenu.getSelectedId())
    {
        case jacksonPollack:  setJacksonPollack();  break;
        case chileConCarne:   setChileConCarne();   break;
        case streetMeat: setStreetMeat(); break;
        case hersheySquirts: setHersheySquirts(); break;
        case fireInFireOut: setFireInFireOut(); break;
        case skidmarks: setSkidmarks(); break;
        case cloudy: setCloudy(); break;
        default: break;
    }

}

void FoundationsofArtIIAudioProcessorEditor::showAboutModal()
{
    const char *title = "artist statement";
    const char *aboutText = R"(
the key to good art is connecting people to one another. the key to connecting people to one another is connecting them to themselves. the more universal an experience, the more powerful. this is the foundation of ART

many thanks to ethan schneider, brian eno & refried beans

www.birdlanguage.fyi
)";
    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, title, aboutText);
}
