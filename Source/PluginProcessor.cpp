/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FoundationsofArtIIAudioProcessor::FoundationsofArtIIAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr, juce::Identifier ("FoundationsOfArt"),
              {
                  std::make_unique<juce::AudioParameterFloat> ("wetDry",            // parameterID
                                                               "WetDry",            // parameter name
                                                               0.0f,                // minimum value
                                                               100.0f,              // maximum value
                                                               50.0f),              // default value
                  std::make_unique<juce::AudioParameterFloat> ("lpf",               // ID
                                                               "Lpf",               // name
                                                               20.0f,                // min
                                                               20000.0f,               // max
                                                               20000.0f),              // default
              }),
    lpf(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f))
{
    audioFormatManager.registerBasicFormats();
    loadFiles();
    
    wetDryParameter = parameters.getRawParameterValue ("wetDry");
    lpfParameter = parameters.getRawParameterValue("lpf");
}

FoundationsofArtIIAudioProcessor::~FoundationsofArtIIAudioProcessor()
{
    fart1AudioFormatReader = nullptr;
    fart2AudioFormatReader = nullptr;
    fart3AudioFormatReader = nullptr;
    fart4AudioFormatReader = nullptr;
    fart5AudioFormatReader = nullptr;
    fart6AudioFormatReader = nullptr;
}

//==============================================================================
const juce::String FoundationsofArtIIAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FoundationsofArtIIAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FoundationsofArtIIAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FoundationsofArtIIAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FoundationsofArtIIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FoundationsofArtIIAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FoundationsofArtIIAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FoundationsofArtIIAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FoundationsofArtIIAudioProcessor::getProgramName (int index)
{
    return {};
}

void FoundationsofArtIIAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FoundationsofArtIIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // prep the audio sources
    fart1AudioFormatReaderSource->prepareToPlay(samplesPerBlock, sampleRate);
    fart2AudioFormatReaderSource->prepareToPlay(samplesPerBlock, sampleRate);
    fart3AudioFormatReaderSource->prepareToPlay(samplesPerBlock, sampleRate);
    fart4AudioFormatReaderSource->prepareToPlay(samplesPerBlock, sampleRate);
    fart5AudioFormatReaderSource->prepareToPlay(samplesPerBlock, sampleRate);
    fart6AudioFormatReaderSource->prepareToPlay(samplesPerBlock, sampleRate);
    
    // prep the circular buffer
    float lowestFreq = 20.0;
    circularBuffer = CircularBuffer(sampleRate / lowestFreq, 0, 1);
    
    // prep the dsp chain
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    lpf.prepare(spec);
    lpf.reset();
    
}

void FoundationsofArtIIAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FoundationsofArtIIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FoundationsofArtIIAudioProcessor::updateLpf()
{
    float freq = *parameters.getRawParameterValue("lpf");
    
    *lpf.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, freq);
}

void FoundationsofArtIIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto wetDry = wetDryParameter;
    
    for (int sampleIdx = 0; sampleIdx < buffer.getNumSamples(); sampleIdx++)
    {
        float sampleData = buffer.getSample(0, sampleIdx);
        circularBuffer.setData(sampleData, 0);
    }
    
    float level = circularBuffer.getRMSLevel(0, 0, buffer.getNumSamples());
    
    if (level > threshold && *wetDry >= 1.0f)
    {
        juce::AudioSourceChannelInfo channelInfo(buffer);
        if (*wetDry >= 1.0f && *wetDry < 21.0f)
        {
            fart1AudioFormatReaderSource->getNextAudioBlock(channelInfo);
        } else if (*wetDry >= 21.0f && *wetDry < 41.0f) {
            fart2AudioFormatReaderSource->getNextAudioBlock(channelInfo);
        } else if (*wetDry >= 41.0f && *wetDry < 61.0f) {
            fart3AudioFormatReaderSource->getNextAudioBlock(channelInfo);
        } else if (*wetDry >= 61.0f && *wetDry < 81.0f) {
            fart4AudioFormatReaderSource->getNextAudioBlock(channelInfo);
        } else if (*wetDry >= 81.0f && *wetDry < 99.0f) {
            fart5AudioFormatReaderSource->getNextAudioBlock(channelInfo);
        } else if (*wetDry >= 99.0f) {
            fart6AudioFormatReaderSource->getNextAudioBlock(channelInfo);
        }
    } else {
        buffer.clear();
    }
    
    juce::dsp::AudioBlock<float> block(buffer);
    
    lpf.process(juce::dsp::ProcessContextReplacing<float> (block));
    updateLpf();
}

//==============================================================================
bool FoundationsofArtIIAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FoundationsofArtIIAudioProcessor::createEditor()
{
    return new FoundationsofArtIIAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void FoundationsofArtIIAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void FoundationsofArtIIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

void FoundationsofArtIIAudioProcessor::loadFiles()
{
    fart1Input = new juce::MemoryInputStream (BinaryData::fart_120_mp3, BinaryData::fart_120_mp3Size, false);
    fart1AudioFormatReader = audioFormatManager.createReaderFor(std::unique_ptr<juce::InputStream> (fart1Input));
    if (fart1AudioFormatReader != nullptr)
    {
        fart1AudioFormatReaderSource = new juce::AudioFormatReaderSource(fart1AudioFormatReader, true);
        fart1AudioFormatReaderSource->setLooping(true);
    }
    
    fart2Input = new juce::MemoryInputStream (BinaryData::fart_2140_mp3, BinaryData::fart_2140_mp3Size, false);
    fart2AudioFormatReader = audioFormatManager.createReaderFor(std::unique_ptr<juce::InputStream> (fart2Input));
    if (fart2AudioFormatReader != nullptr)
    {
        fart2AudioFormatReaderSource = new juce::AudioFormatReaderSource(fart2AudioFormatReader, true);
        fart2AudioFormatReaderSource->setLooping(true);
    }
    
    fart3Input = new juce::MemoryInputStream (BinaryData::fart_4160_mp3, BinaryData::fart_4160_mp3Size, false);
    fart3AudioFormatReader = audioFormatManager.createReaderFor(std::unique_ptr<juce::InputStream> (fart3Input));
    if (fart3AudioFormatReader != nullptr)
    {
        fart3AudioFormatReaderSource = new juce::AudioFormatReaderSource(fart3AudioFormatReader, true);
        fart3AudioFormatReaderSource->setLooping(true);
    }
    
    fart4Input = new juce::MemoryInputStream (BinaryData::fart_6180_mp3, BinaryData::fart_6180_mp3Size, false);
    fart4AudioFormatReader = audioFormatManager.createReaderFor(std::unique_ptr<juce::InputStream> (fart4Input));
    if (fart4AudioFormatReader != nullptr)
    {
        fart4AudioFormatReaderSource = new juce::AudioFormatReaderSource(fart4AudioFormatReader, true);
        fart4AudioFormatReaderSource->setLooping(true);
    }
    
    fart5Input = new juce::MemoryInputStream (BinaryData::fart_8199_mp3, BinaryData::fart_8199_mp3Size, false);
    fart5AudioFormatReader = audioFormatManager.createReaderFor(std::unique_ptr<juce::InputStream> (fart5Input));
    if (fart5AudioFormatReader != nullptr)
    {
        fart5AudioFormatReaderSource = new juce::AudioFormatReaderSource(fart5AudioFormatReader, true);
        fart5AudioFormatReaderSource->setLooping(true);
    }
    
    fart6Input = new juce::MemoryInputStream (BinaryData::fart_100_mp3, BinaryData::fart_100_mp3Size, false);
    fart6AudioFormatReader = audioFormatManager.createReaderFor(std::unique_ptr<juce::InputStream> (fart6Input));
    if (fart6AudioFormatReader != nullptr)
    {
        fart6AudioFormatReaderSource = new juce::AudioFormatReaderSource(fart6AudioFormatReader, true);
        fart6AudioFormatReaderSource->setLooping(true);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FoundationsofArtIIAudioProcessor();
}
