#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================

class StepButton : public juce::ToggleButton
{
public:
    StepButton() { setClickingTogglesState (true); }

    void paintButton (juce::Graphics& g,
                      bool /*shouldDrawButtonAsHighlighted*/,
                      bool /*shouldDrawButtonAsDown*/) override
    {
        auto area = getLocalBounds().toFloat();
        g.setColour (getToggleState() ? juce::Colours::black : juce::Colours::grey);
        g.fillRect (area);
    }
};

//==============================================================================

struct ButtonLookAndFeel : public juce::LookAndFeel_V4
{
    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override
    {
        g.fillAll(juce::Colours::transparentBlack);
        g.setColour(juce::Colours::black);
        g.drawRect(button.getLocalBounds(), shouldDrawButtonAsDown ? 2.0f : 1.0f);
    }
};

//==============================================================================

class AcidSequencerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AcidSequencerAudioProcessorEditor (AcidSequencerAudioProcessor&);
    ~AcidSequencerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    AcidSequencerAudioProcessor& audioProcessor;
    
    juce::Image backgroundImage;
    
    //Waveform
    juce::ComboBox waveformMenu;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformAttachment;
    
    //Rate
    juce::Slider rateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    
    //Cutoff
    juce::OwnedArray<juce::Slider> cutoffSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachments;
    
    //Distortion
    juce::OwnedArray<juce::Slider> distortionSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> distortionAttachments;
    
    //Portamento
    juce::OwnedArray<juce::Slider> portamentoSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> portamentoAttachments;
    
    //Resonance
    juce::OwnedArray<juce::Slider> resonanceSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachments;
    
    //Attack
    juce::Slider attackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    
    //Decay
    juce::Slider decaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    
    //Sustain
    juce::Slider sustainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    
    //Release
    juce::Slider releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    //Delay
    juce::Slider delaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;
    
    //Pitch
    juce::Slider pitchSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchAttachment;
    
    
    //Reverb
    juce::Slider reverbSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbAttachment;
    
    //Volume sliders
    juce::OwnedArray<juce::Slider> stepSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> stepAttachments;
    
    //Pitch sliders
    juce::OwnedArray<juce::Slider> pitchSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> pitchAttachments;
    
    //Clear
    juce::TextButton clearButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> clearAttachment;

    //Start/Stop
    juce::TextButton startButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> startAttachment;
    
    //Randomiser
    juce::TextButton randomiserButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> randomiserAttachment;
    
    //Scale
    juce::TextButton scaleButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> scaleAttachment;
    
    //Volume dial
    juce::Slider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    //Step Logic
    StepButton stepButtons[8]; 
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> stepButtonAttachments;
    
    //Look and feel
    ButtonLookAndFeel buttonLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AcidSequencerAudioProcessorEditor)
};

