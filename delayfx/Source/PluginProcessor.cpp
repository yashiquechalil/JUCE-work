/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayfxAudioProcessor::DelayfxAudioProcessor()
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
}

DelayfxAudioProcessor::~DelayfxAudioProcessor()
{
}

//==============================================================================
const String DelayfxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayfxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayfxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayfxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayfxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayfxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayfxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayfxAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayfxAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayfxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayfxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	const int numInputChannels = getTotalNumInputChannels();
	const int delayBufferSize = 2 * (sampleRate + samplesPerBlock);
	mSampleRate = sampleRate;
	mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DelayfxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayfxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DelayfxAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

 
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = mDelayBuffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = mDelayBuffer.getReadPointer(channel);
		fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
		getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
    }
	mWritePosition += bufferLength;
	mWritePosition %= delayBufferLength;
}


void DelayfxAudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData) 
{
	if (delayBufferLength > bufferLength + mWritePosition)
	{
		mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, 0.8, 0.8);

	}
	else
	{
		const int bufferRemaining = delayBufferLength - mWritePosition;

		mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, 0.8, 0.8);
		mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, 0.8, 0.8);


	}

}
void DelayfxAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
	int delayTime = 250;
	const int readPosition = static_cast<int> (delayBufferLength + mWritePosition - (mSampleRate * delayTime / 1000)) % delayBufferLength;
	if (delayBufferLength > bufferLength + readPosition)
	{
		buffer.addFrom(channel, 0,delayBufferData + readPosition,bufferLength);
	}
	else
	{
		const int bufferRemaining = delayBufferLength - readPosition;
		buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
		buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
	}
}

//==============================================================================
bool DelayfxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayfxAudioProcessor::createEditor()
{
    return new DelayfxAudioProcessorEditor (*this);
}

//==============================================================================
void DelayfxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayfxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayfxAudioProcessor();
}
