/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/**
*/
class AcidSequencerAudioProcessor  : public juce::AudioProcessor
{
public:
    
public:
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts; // Volume sliders
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    //==============================================================================
    AcidSequencerAudioProcessor();
    ~AcidSequencerAudioProcessor() override;

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

private:
    //===============================================================================
    //Step button features
        void clearSequence();
        void randomiseSequence();
        void cycleScale();

        //FX
        float stepVolume[8];
        float stepPitch[8];
        float stepCutoff[8];
        float stepResonance[8];
        float stepDistortion[8];
        float stepPortamento[8];
        
        // Step ON/OFF switches
        bool stepOn[8];
        bool isPlaying = false;
        int currentScale = 0;
        
        //Sequencer clock
        int currentStep = 0;
        double samplesPerStep = 0.0;
        double sampleCounter = 0.0;

        // current tone
        double phasePos = 0, phaseInc = 0.0, gain = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AcidSequencerAudioProcessor)
    };
