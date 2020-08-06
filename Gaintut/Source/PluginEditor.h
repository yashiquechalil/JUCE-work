/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GaintutorialAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    GaintutorialAudioProcessorEditor (GaintutorialAudioProcessor&);
    ~GaintutorialAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	ScopedPointer<Slider> driveknob;
	ScopedPointer<Slider> rangeknob;
	ScopedPointer<Slider> blendknob;
	ScopedPointer<Slider> volumeknob;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    GaintutorialAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GaintutorialAudioProcessorEditor)
};
