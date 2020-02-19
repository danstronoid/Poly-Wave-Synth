/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PolyWaveSynthAudioProcessor::PolyWaveSynthAudioProcessor() :
	parameters(*this, nullptr, "PolyWaveSynth",
		{
			std::make_unique<AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.6f),
			std::make_unique<AudioParameterFloat>("amp_attack", "Attack", 0.001f, 2.0f, 0.01f),
			std::make_unique<AudioParameterFloat>("amp_decay", "Decay", 0.001f, 2.0f, 0.01f),
			std::make_unique<AudioParameterFloat>("amp_sustain", "Sustain", 0.0f, 1.0f, 1.0f),
			std::make_unique<AudioParameterFloat>("amp_release", "Release", 0.01f, 2.0f, 0.05f),
			std::make_unique<AudioParameterInt>("oscType", "OscType", 0, 3, 0),
            std::make_unique<AudioParameterFloat>("cutoff", "Cufoff", 40.0f, 10000.0f, 4000.0f),
            std::make_unique<AudioParameterFloat>("q", "Q", 0.01f, 5.0f, 1.0f)
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
	parameters.addParameterListener("gain", this);
	parameters.addParameterListener("amp_attack", this);
	parameters.addParameterListener("amp_decay", this);
	parameters.addParameterListener("amp_sustain", this);
	parameters.addParameterListener("amp_release", this);
	parameters.addParameterListener("oscType", this);
    parameters.addParameterListener("cutoff", this);
    parameters.addParameterListener("q", this);
}

PolyWaveSynthAudioProcessor::~PolyWaveSynthAudioProcessor()
{
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
	if (parameterID == "amp_attack")
		synthEngine.setAttack(newValue);
	else if (parameterID == "amp_decay")
		synthEngine.setDecay(newValue);
	else if (parameterID == "amp_sustain")
		synthEngine.setSustain(newValue);
	else if (parameterID == "amp_release")
		synthEngine.setRelease(newValue);
	else if (parameterID == "gain")
		currentGain = newValue;
	else if (parameterID == "oscType")
	{
        int index = newValue;
		synthEngine.setOscType(static_cast<WaveType>(index));

		auto* attack = parameters.getRawParameterValue("amp_attack");
		auto* decay = parameters.getRawParameterValue("amp_decay");
		auto* sustain = parameters.getRawParameterValue("amp_sustain");
		auto* release = parameters.getRawParameterValue("amp_release");
		synthEngine.setAttack(*attack);
		synthEngine.setDecay(*decay);
		synthEngine.setSustain(*sustain);
		synthEngine.setRelease(*release);
	}
    else if (parameterID == "cutoff")
    {
        auto* q = parameters.getRawParameterValue("q");

        for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
            svf[channel].setParameters(State::Low_Pass, newValue, *q);
    }
    else if (parameterID == "q")
    {
        auto* cutoff = parameters.getRawParameterValue("cutoff");

        for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
            svf[channel].setParameters(State::Low_Pass, *cutoff, newValue);
    }
}

void PolyWaveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synthEngine.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        svf[channel].setSampleRate(sampleRate);
        svf[channel].setParameters(State::Low_Pass, 4000, 1);
    }
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

    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        auto* samples = buffer.getWritePointer(channel);
        svf[channel].renderBlock(samples, buffer.getNumSamples());
    }

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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PolyWaveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PolyWaveSynthAudioProcessor();
}
