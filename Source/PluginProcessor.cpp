/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

NormalisableRange<float> frequencyRange(float min, float max, float interval)
{
    return { min, max, interval, 1.0f / std::log2(1.0f + std::sqrt(max / min)) };
}

//==============================================================================
PolyWaveSynthAudioProcessor::PolyWaveSynthAudioProcessor() :
    parameters(*this, nullptr, "PolyWaveSynth",
        {
            // osciallator parameters
            std::make_unique<AudioParameterInt>("oscType", "OscType", 0, 3, 0),
            std::make_unique<AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.6f),
            std::make_unique<AudioParameterFloat>("amp_attack", "Attack", 0.001f, 2.0f, 0.01f),
            std::make_unique<AudioParameterFloat>("amp_decay", "Decay", 0.001f, 2.0f, 0.01f),
            std::make_unique<AudioParameterFloat>("amp_sustain", "Sustain", 0.0f, 1.0f, 1.0f),
            std::make_unique<AudioParameterFloat>("amp_release", "Release", 0.01f, 2.0f, 0.05f),

            // filter parameters
            std::make_unique<AudioParameterInt>("filterType", "FilterType", 0, 2, 0),
            std::make_unique<AudioParameterFloat>("cutoff", "Cufoff", frequencyRange(40.0f, 19000.0f, 0.01), 1000.0f),
            std::make_unique<AudioParameterFloat>("q", "Q", 0.01f, 5.0f, 1.0f),
            std::make_unique<AudioParameterFloat>("envAmt", "Env", 0.0f, 1.0f, 1.0f),
            std::make_unique<AudioParameterFloat>("filter_attack", "Attack", 0.001f, 2.0f, 0.01f),
            std::make_unique<AudioParameterFloat>("filter_decay", "Decay", 0.001f, 2.0f, 0.01f),
            std::make_unique<AudioParameterFloat>("filter_sustain", "Sustain", 0.0f, 1.0f, 1.0f),
            std::make_unique<AudioParameterFloat>("filter_release", "Release", 0.01f, 2.0f, 0.05f)
		}),
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
    parameters.addParameterListener("oscType", this);
	parameters.addParameterListener("gain", this);
	parameters.addParameterListener("amp_attack", this);
	parameters.addParameterListener("amp_decay", this);
	parameters.addParameterListener("amp_sustain", this);
	parameters.addParameterListener("amp_release", this);

    parameters.addParameterListener("filterType", this);
    parameters.addParameterListener("cutoff", this);
    parameters.addParameterListener("q", this);
    parameters.addParameterListener("envAmt", this);
    parameters.addParameterListener("filter_attack", this);
    parameters.addParameterListener("filter_decay", this);
    parameters.addParameterListener("filter_sustain", this);
    parameters.addParameterListener("filter_release", this);

    initParameters();
}

PolyWaveSynthAudioProcessor::~PolyWaveSynthAudioProcessor()
{
}

void PolyWaveSynthAudioProcessor::initParameters()
{
    // this is a cheeky way of initializing values, probably a better way
    auto* amp_attack = parameters.getRawParameterValue("amp_attack");
    auto* amp_decay = parameters.getRawParameterValue("amp_decay");
    auto* amp_sustain = parameters.getRawParameterValue("amp_sustain");
    auto* amp_release = parameters.getRawParameterValue("amp_release");
    synthEngine.setAmpADSR(*amp_attack, *amp_decay, *amp_sustain, *amp_release);

    auto* filterType = parameters.getRawParameterValue("filterType");
    auto* cutoff = parameters.getRawParameterValue("cutoff");
    auto* q = parameters.getRawParameterValue("q");
    auto* envAmt = parameters.getRawParameterValue("envAmt");
    synthEngine.setFilterParameters(static_cast<State>((int)*filterType), *cutoff, *q, *envAmt);

    auto* filter_attack = parameters.getRawParameterValue("filter_attack");
    auto* filter_decay = parameters.getRawParameterValue("filter_decay");
    auto* filter_sustain = parameters.getRawParameterValue("filter_sustain");
    auto* filter_release = parameters.getRawParameterValue("filter_release");
    synthEngine.setFilterADSR(*filter_attack, *filter_decay, *filter_sustain, *filter_release);
}

//==============================================================================
const String PolyWaveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PolyWaveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PolyWaveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PolyWaveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PolyWaveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PolyWaveSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PolyWaveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PolyWaveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String PolyWaveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void PolyWaveSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PolyWaveSynthAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == "amp_attack" || parameterID == "amp_decay"
        || parameterID == "amp_sustain" || parameterID == "amp_release")
    {
        auto* amp_attack = parameters.getRawParameterValue("amp_attack");
        auto* amp_decay = parameters.getRawParameterValue("amp_decay");
        auto* amp_sustain = parameters.getRawParameterValue("amp_sustain");
        auto* amp_release = parameters.getRawParameterValue("amp_release");
        synthEngine.setAmpADSR(*amp_attack, *amp_decay, *amp_sustain, *amp_release);
    }
	else if (parameterID == "gain")
		currentGain = newValue;
	else if (parameterID == "oscType")
	{
        int index = newValue;
		synthEngine.setOscType(static_cast<WaveType>(index));

        initParameters();
	}
    else if (parameterID == "filterType" || parameterID == "cutoff" 
        || parameterID == "q" || parameterID == "envAmt")
    {
        auto* filterType = parameters.getRawParameterValue("filterType");
        auto* cutoff = parameters.getRawParameterValue("cutoff");
        auto* q = parameters.getRawParameterValue("q");
        auto* envAmt = parameters.getRawParameterValue("envAmt");
        synthEngine.setFilterParameters(static_cast<State>((int)*filterType), *cutoff, *q, *envAmt);
    }
    else if (parameterID == "filter_attack" || parameterID == "filter_decay"
        || parameterID == "filter_sustain" || parameterID == "filter_release")
    {
        auto* filter_attack = parameters.getRawParameterValue("filter_attack");
        auto* filter_decay = parameters.getRawParameterValue("filter_decay");
        auto* filter_sustain = parameters.getRawParameterValue("filter_sustain");
        auto* filter_release = parameters.getRawParameterValue("filter_release");
        synthEngine.setFilterADSR(*filter_attack, *filter_decay, *filter_sustain, *filter_release);
    }
}

void PolyWaveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synthEngine.setCurrentPlaybackSampleRate(sampleRate);
    
}

void PolyWaveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PolyWaveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void PolyWaveSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	synthEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	if (previousGain == currentGain)
		buffer.applyGain(currentGain);
	else
	{
		buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
		previousGain = currentGain;
	}
	
}

//==============================================================================
bool PolyWaveSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PolyWaveSynthAudioProcessor::createEditor()
{
    return new PolyWaveSynthAudioProcessorEditor (*this, parameters);
	//return new GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PolyWaveSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PolyWaveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PolyWaveSynthAudioProcessor();
}
