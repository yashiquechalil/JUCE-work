/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GaintutorialAudioProcessor::GaintutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	state = new AudioProcessorValueTreeState(*this,nullptr);
	using parameter = AudioProcessorValueTreeState::Parameter;
	state->createAndAddParameter(std::make_unique <parameter>("drive", "Drive", "Drive", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.1f, nullptr, nullptr));
    state->createAndAddParameter(std::make_unique <parameter>("range", "Range", "Range", NormalisableRange<float>(0.f, 3000.f, 0.0001f), 0.1f, nullptr, nullptr));
	state->createAndAddParameter(std::make_unique <parameter>("blend", "Blend", "Blend", NormalisableRange<float>(0.f, 1.f, 0.0001f), 0.1f, nullptr, nullptr));
	state->createAndAddParameter(std::make_unique <parameter>("volume", "Volume", "Volume", NormalisableRange<float>(0.f, 3.f, 0.0001f), 0.1f, nullptr, nullptr));

	state->state = ValueTree("drive");
	state->state = ValueTree("range");
	state->state = ValueTree("blend");
	state->state = ValueTree("volume");
}

GaintutorialAudioProcessor::~GaintutorialAudioProcessor()
{
}

//==============================================================================
const String GaintutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GaintutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GaintutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GaintutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GaintutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GaintutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GaintutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GaintutorialAudioProcessor::setCurrentProgram (int index)
{
	(void)index;
}

const String GaintutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void GaintutorialAudioProcessor::changeProgramName (int index, const String& newName)
{
	(void)index;
	(void)newName;
}

//==============================================================================
void GaintutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	(void)sampleRate;
	(void)samplesPerBlock;
}

void GaintutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GaintutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GaintutorialAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	(void)midiMessages;
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	float drive = *state->getRawParameterValue("drive");
	float range = *state->getRawParameterValue("range");
	float blend = *state->getRawParameterValue("blend");
	float volume = *state->getRawParameterValue("volume");

    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			float cleanSig = *channelData;
			*channelData *= drive*range;
			*channelData =(((((2.f / float_Pi) * atan(*channelData))*blend)+(cleanSig*(1.f/blend)))/2)*volume;
			channelData ++;
		}

        
    }
}
AudioProcessorValueTreeState& GaintutorialAudioProcessor ::getState()
{
	return *state;
}

//==============================================================================
bool GaintutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GaintutorialAudioProcessor::createEditor()
{
    return new GaintutorialAudioProcessorEditor (*this);
}

//==============================================================================
void GaintutorialAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	  MemoryOutputStream stream(destData, false);
	    state->state.writeToStream(stream);
}

void GaintutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
     //whose contents will have been created by the getStateInformation() call.
	ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
	if (tree.isValid())
	{
	      state-> state = tree;
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GaintutorialAudioProcessor();
}
