/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

    Tiny Table
    Author:  Daniel Schwartz

    Tiny Table is a simple wave table synthesizer.  Each voice has 2 oscillators,
    2 ADSR envelopes, and one filter.  There is also a single LFO which controls
    the cutoff frequency of all of the filters.  There are a total of 12 voices.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PolyWaveSynthAudioProcessor::PolyWaveSynthAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
    parameters(*this, nullptr, "PolyWaveSynth", createParameterLayout())
#endif
{
    addParameterListeners();
    initParameters();
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

void PolyWaveSynthAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String PolyWaveSynthAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void PolyWaveSynthAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================

void PolyWaveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synthEngine.prepareToPlay(sampleRate, samplesPerBlock);    
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

    meter = buffer.getMagnitude(0, buffer.getNumSamples());
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

// this is a helper function to create a range with a skew for frequency
NormalisableRange<float> frequencyRange(float min, float max, float interval)
{
    return { min, max, interval, 1.0f / std::log2(1.0f + std::sqrt(max / min)) };
}


// create the parameters for the constructor
AudioProcessorValueTreeState::ParameterLayout PolyWaveSynthAudioProcessor::createParameterLayout()
{
    std::vector< std::unique_ptr<RangedAudioParameter> > params;

    // master parameters
    params.push_back(std::make_unique<AudioParameterFloat>("gain", "Gain", NormalisableRange<float>(0.0f, 1.0f), 1.0f));

    // osciallator parameters
    params.push_back(std::make_unique<AudioParameterInt>("osc_oscType", "OscType", 0, 3, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("osc_level", "OscLevel", NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("osc_noise", "Noise", NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("osc_freq", "OscFreq", 
        frequencyRange(20.0f, 12000.0f, 1.0f), 20.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("ampEnv_attack", "Attack", 0.001f, 2.0f, 0.001f));
    params.push_back(std::make_unique<AudioParameterFloat>("ampEnv_decay", "Decay", 0.001f, 2.0f, 0.01f));
    params.push_back(std::make_unique<AudioParameterFloat>("ampEnv_sustain", "Sustain", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("ampEnv_release", "Release", 0.01f, 2.0f, 0.05f));
    params.push_back(std::make_unique<AudioParameterInt>("fm_oscType", "FMOscType", 0, 3, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("fm_multi", "Multi", 0.0f, 4.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("fm_freq", "FMFreq", 
        frequencyRange(20.0f, 12000.0f, 1.0f), 20.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("fm_depth", "Depth", 0.0f, 1.0f, 0.0f));

    // filter parameters
    params.push_back(std::make_unique<AudioParameterInt>("filter_type", "FilterType", 0, 2, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("filter_cutoff", "Cufoff", 
        frequencyRange(40.0f, 19000.0f, 1.0f), 19000.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("filter_q", "Q", 0.1f, 5.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("filter_envAmt", "Env", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("filterEnv_attack", "Attack", 0.001f, 2.0f, 0.01f));
    params.push_back(std::make_unique<AudioParameterFloat>("filterEnv_decay", "Decay", 0.001f, 2.0f, 0.01f));
    params.push_back(std::make_unique<AudioParameterFloat>("filterEnv_sustain", "Sustain", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("filterEnv_release", "Release", 0.01f, 2.0f, 0.05f));
    params.push_back(std::make_unique<AudioParameterInt>("lfo_type", "LFOType", 0, 3, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("lfo_rate", "LFORate", 0.01f, 20.0f, 2.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("lfo_depth", "LFODepth", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<AudioParameterBool>("lfo_trigger", "LFOTrigger", false));

    return { params.begin(), params.end() };
}

// helper function to add parameter listeners in one place
void PolyWaveSynthAudioProcessor::addParameterListeners()
{
    // master parameter listeners
    parameters.addParameterListener("gain", this);

    // Osc parameter listeners
    parameters.addParameterListener("osc_oscType", this);
    parameters.addParameterListener("osc_level", this);
    parameters.addParameterListener("osc_noise", this);
    parameters.addParameterListener("osc_freq", this);
    parameters.addParameterListener("ampEnv_attack", this);
    parameters.addParameterListener("ampEnv_decay", this);
    parameters.addParameterListener("ampEnv_sustain", this);
    parameters.addParameterListener("ampEnv_release", this);
    parameters.addParameterListener("fm_oscType", this);
    parameters.addParameterListener("fm_multi", this);
    parameters.addParameterListener("fm_freq", this);
    parameters.addParameterListener("fm_depth", this);

    // Filter parameter listeners
    parameters.addParameterListener("filter_type", this);
    parameters.addParameterListener("filter_cutoff", this);
    parameters.addParameterListener("filter_q", this);
    parameters.addParameterListener("filter_envAmt", this);
    parameters.addParameterListener("filterEnv_attack", this);
    parameters.addParameterListener("filterEnv_decay", this);
    parameters.addParameterListener("filterEnv_sustain", this);
    parameters.addParameterListener("filterEnv_release", this);
    parameters.addParameterListener("lfo_type", this);
    parameters.addParameterListener("lfo_rate", this);
    parameters.addParameterListener("lfo_depth", this);
    parameters.addParameterListener("lfo_trigger", this);
}

// initialize all parameters
void PolyWaveSynthAudioProcessor::initParameters()
{
    // this is a cheeky way of initializing values, probably a better way

    // update the output gain
    auto* output_level = parameters.getRawParameterValue("gain");
    currentGain = *output_level;

    // call all other update functions
    updateOsc();
    updateFM();
    updateAmpEnv();
    updateFilter();
    updateFilterEnv();
    updateFilterLFO();
}

// callback for when a parameter is changed
void PolyWaveSynthAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    // master
    if (parameterID == "gain")
        currentGain = newValue;

    // osc
    else if (parameterID.containsWholeWord("osc"))
    {
        updateOsc();
    }
    else if (parameterID.containsWholeWord("fm"))
    {
        updateFM();
    }
    else if (parameterID.containsWholeWord("ampEnv"))
    {
        updateAmpEnv();
    }

    // filter
    else if (parameterID.containsWholeWord("filter"))
    {
        updateFilter();
    }
    else if (parameterID.containsWholeWord("filterEnv"))
    {
        updateFilterEnv();
    }
    else if (parameterID.containsWholeWord("lfo"))
    {
        updateFilterLFO();
    }
}

void PolyWaveSynthAudioProcessor::updateOsc()
{
    // This is a modest level scale to the osc levels
    const float oscLevelScale = 0.6f;

    auto* osc_oscType = parameters.getRawParameterValue("osc_oscType");
    synthEngine.setOscType(static_cast<WaveType>((int)*osc_oscType));

    auto* osc_level = parameters.getRawParameterValue("osc_level");
    auto* osc_noise = parameters.getRawParameterValue("osc_noise");
    auto* osc_freq = parameters.getRawParameterValue("osc_freq");

    // this creates a boolean to test if the osc freq is fixed 
    // or if it should respond to note on
    auto oscRange = parameters.getParameterRange("osc_freq");
    bool oscFixed = oscRange.convertTo0to1(*osc_freq);

    synthEngine.setOscParameters(*osc_level * oscLevelScale, *osc_noise * oscLevelScale, *osc_freq, oscFixed);
}

void PolyWaveSynthAudioProcessor::updateFM() 
{
    auto* fm_oscType = parameters.getRawParameterValue("fm_oscType");
    synthEngine.setFMOscType(static_cast<WaveType>((int)*fm_oscType));

    auto* fm_multi = parameters.getRawParameterValue("fm_multi");
    auto* fm_freq = parameters.getRawParameterValue("fm_freq");
    auto* fm_depth = parameters.getRawParameterValue("fm_depth");

    // this creates a boolean to test if the osc freq is fixed 
    // or if it should respond to note on
    auto fmRange = parameters.getParameterRange("fm_freq");
    bool fmFixed = fmRange.convertTo0to1(*fm_freq);

    synthEngine.setFMParameters(*fm_freq, *fm_depth, *fm_multi, fmFixed);
}
void PolyWaveSynthAudioProcessor::updateAmpEnv() 
{
    auto* ampEnv_attack = parameters.getRawParameterValue("ampEnv_attack");
    auto* ampEnv_decay = parameters.getRawParameterValue("ampEnv_decay");
    auto* ampEnv_sustain = parameters.getRawParameterValue("ampEnv_sustain");
    auto* ampEnv_release = parameters.getRawParameterValue("ampEnv_release");
    synthEngine.setAmpADSR(*ampEnv_attack, *ampEnv_decay, *ampEnv_sustain, *ampEnv_release);
}
void PolyWaveSynthAudioProcessor::updateFilter() 
{
    auto* filter_type = parameters.getRawParameterValue("filter_type");
    auto* filter_cutoff = parameters.getRawParameterValue("filter_cutoff");
    auto* filter_q = parameters.getRawParameterValue("filter_q");
    auto* filter_envAmt = parameters.getRawParameterValue("filter_envAmt");
    synthEngine.setFilterParameters(static_cast<State>((int)*filter_type), *filter_cutoff, *filter_q, *filter_envAmt);
}
void PolyWaveSynthAudioProcessor::updateFilterEnv() 
{
    auto* filterEnv_attack = parameters.getRawParameterValue("filterEnv_attack");
    auto* filterEnv_decay = parameters.getRawParameterValue("filterEnv_decay");
    auto* filterEnv_sustain = parameters.getRawParameterValue("filterEnv_sustain");
    auto* filterEnv_release = parameters.getRawParameterValue("filterEnv_release");
    synthEngine.setFilterADSR(*filterEnv_attack, *filterEnv_decay, *filterEnv_sustain, *filterEnv_release);
}
void PolyWaveSynthAudioProcessor::updateFilterLFO() 
{
    auto* lfo_type = parameters.getRawParameterValue("lfo_type");
    synthEngine.setFilterLFOType(static_cast<WaveType>((int)*lfo_type));

    auto* lfo_rate = parameters.getRawParameterValue("lfo_rate");
    auto* lfo_depth = parameters.getRawParameterValue("lfo_depth");
    auto* lfo_trigger = parameters.getRawParameterValue("lfo_trigger");
    synthEngine.setFilterLFO(*lfo_rate, *lfo_depth, *lfo_trigger);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PolyWaveSynthAudioProcessor();
}
