#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

AcidSequencerAudioProcessorEditor::AcidSequencerAudioProcessorEditor (AcidSequencerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    //Background Image
    backgroundImage = juce::ImageCache::getFromMemory (
        BinaryData::background_png,
        BinaryData::background_pngSize );
    
    //Waveform
    waveformMenu.addItem("Saw", 1);
    waveformMenu.addItem("Square", 2);
    waveformMenu.addItem("Sine", 3);
    addAndMakeVisible(waveformMenu);

    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "waveform", waveformMenu);// Connect slider to parameter
    
    //Tempo
    tempoSlider.setSliderStyle(juce::Slider::Rotary);
    tempoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0); //No visual values for aesethics and clarity
    tempoSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(tempoSlider);

    tempoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "tempo", tempoSlider);

    //Cutoff
    for (int i = 0; i < 8; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::Rotary);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        cutoffSliders.add(slider);
        
        for (int i = 0; i < cutoffSliders.size(); ++i)
            cutoffSliders[i]->setColour(juce::Slider::thumbColourId, juce::Colours::black);


        juce::String paramID = "cutoff" + juce::String(i/* + 1*/);
        cutoffAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.apvts, paramID, *slider));
    }

    
    //Distortion
    for (int i = 0; i < 8; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::Rotary);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        distortionSliders.add(slider);
        
        for (int i = 0; i < distortionSliders.size(); ++i)
            distortionSliders[i]->setColour(juce::Slider::thumbColourId, juce::Colours::black);


        juce::String paramID = "distortion" + juce::String(i/* + 1*/);
        distortionAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.apvts, paramID, *slider));
    }
    
    //Portamento
    for (int i = 0; i < 8; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::Rotary);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        portamentoSliders.add(slider);
        
        for (int i = 0; i < portamentoSliders.size(); ++i)
            portamentoSliders[i]->setColour(juce::Slider::thumbColourId, juce::Colours::black);

        juce::String paramID = "portamento" + juce::String(i/* + 1*/);
        portamentoAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.apvts, paramID, *slider));
    }
    
    //Resonance
    for (int i = 0; i < 8; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::Rotary);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        resonanceSliders.add(slider);
        
        for (int i = 0; i < resonanceSliders.size(); ++i)
            resonanceSliders[i]->setColour(juce::Slider::thumbColourId, juce::Colours::black);


        juce::String paramID = "resonance" + juce::String(i/* + 1*/);
        resonanceAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.apvts, paramID, *slider));
    }
    
    // Attack
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    attackSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(attackSlider);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "attack", attackSlider);
    
    //Decay
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    decaySlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(decaySlider);

    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "decay", decaySlider);
    
    //Sustain
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    sustainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(sustainSlider);

    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "sustain", sustainSlider);
    
    //Release
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    releaseSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(releaseSlider);

    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "release", releaseSlider);
    
    //Delay
    delaySlider.setSliderStyle(juce::Slider::Rotary);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    delaySlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(delaySlider);

    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "delay", delaySlider);
    
    //Reverb
    reverbSlider.setSliderStyle(juce::Slider::Rotary);
    reverbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    reverbSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(reverbSlider);

    reverbAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "reverb", reverbSlider);

    // Volume sliders
    for (int i = 0; i < 8; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::LinearBarVertical);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        stepSliders.add(slider);

        juce::String paramID = "stepVolume" + juce::String(i/* + 1*/);
        stepAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.apvts, paramID, *slider));
    }
    
    // Pitch sliders
    for (int i = 0; i < 8; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::LinearBarVertical);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(slider);
        pitchSliders.add(slider);
        
        juce::String paramID = "stepPitch" + juce::String(i);
        pitchAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
            audioProcessor.apvts, paramID, *slider));

    }

    // Step buttons
    for (int i = 0; i < 8; ++i)
    {
        juce::String paramID = "stepOn" + juce::String(i);
        stepButtonAttachments.add(new juce::AudioProcessorValueTreeState::ButtonAttachment(
            audioProcessor.apvts, paramID, stepButtons[i]));
        addAndMakeVisible(stepButtons[i]);
    }

    // Clear
    addAndMakeVisible(clearButton);
    clearButton.setButtonText("Clear");
    clearButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    clearButton.setColour(juce::TextButton::textColourOffId, juce::Colours::transparentBlack);
    clearButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    clearButton.setLookAndFeel(&buttonLook);

    // Start/Stop
    addAndMakeVisible(startButton);
    startButton.setButtonText("Start/Stop");
    startButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    startButton.setColour(juce::TextButton::textColourOffId, juce::Colours::transparentBlack);
    startButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    startButton.setLookAndFeel(&buttonLook);

    // Randomiser
    addAndMakeVisible(randomiserButton);
    randomiserButton.setButtonText("Randomiser");
    randomiserButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    randomiserButton.setColour(juce::TextButton::textColourOffId, juce::Colours::transparentBlack);
    randomiserButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    randomiserButton.setLookAndFeel(&buttonLook);

    // Scale
    addAndMakeVisible(scaleButton);
    scaleButton.setButtonText("Scale");
    scaleButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    scaleButton.setColour(juce::TextButton::textColourOffId, juce::Colours::transparentBlack);
    scaleButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    scaleButton.setLookAndFeel(&buttonLook);
    
    //Volume dial
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    addAndMakeVisible(volumeSlider);

    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "masterVolume", volumeSlider);

    setSize (1200, 700);
}

AcidSequencerAudioProcessorEditor::~AcidSequencerAudioProcessorEditor()
{
    clearButton.setLookAndFeel(nullptr);
    startButton.setLookAndFeel(nullptr);
    randomiserButton.setLookAndFeel(nullptr);
    scaleButton.setLookAndFeel(nullptr);
}

//==============================================================================

void AcidSequencerAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (backgroundImage.isValid())
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    else
        g.fillAll (juce::Colours::black);
}

//==============================================================================

void AcidSequencerAudioProcessorEditor::resized()
{

    // =============================
    // ===== TOP SECTION ===========
    // =============================

    // Waveform and Tempo
    waveformMenu.setBounds(15, 40, 80, 25);
    tempoSlider.setBounds(100, 10, 60, 80);

    // =============================
    // ===== FILTER / DRIVE ROW ====
    // =============================

    // Cutoff
    int cutoffButtonSize = 60;
    int cutoffSpacing = 20;

    int cutoffStartX = 90;
    int cutoffY = 120;

    for (int i = 0; i < cutoffSliders.size(); ++i)
    {
        int sliderX = cutoffStartX + i * (cutoffButtonSize + cutoffSpacing);
        cutoffSliders[i]->setBounds(sliderX, cutoffY, cutoffButtonSize, cutoffButtonSize);
    }
    
    //Distortion
    int distortionButtonSize = 60;
    int distortionSpacing = 20;

    int distortionStartX = 90;
    int distortionY = 200;

    for (int i = 0; i < distortionSliders.size(); ++i)
    {
        int sliderX = distortionStartX + i * (distortionButtonSize + distortionSpacing);
        distortionSliders[i]->setBounds(sliderX, distortionY, distortionButtonSize, distortionButtonSize);
    }
    
    //Portamento
    int portamentoButtonSize = 60;
    int portamentoSpacing = 20;

    int portamentoStartX = 90;
    int portamentoY = 280;

    for (int i = 0; i < portamentoSliders.size(); ++i)
    {
        int sliderX = portamentoStartX + i * (portamentoButtonSize + portamentoSpacing);
        portamentoSliders[i]->setBounds(sliderX, portamentoY, portamentoButtonSize, portamentoButtonSize);
    }
    
    //Resonance
    int resonanceButtonSize = 60;
    int resonanceSpacing = 20;
    
    int resonanceY = 360;

    for (int i = 0; i < resonanceSliders.size(); ++i)
    {
        int sliderX = portamentoStartX + i * (resonanceButtonSize + resonanceSpacing);
        resonanceSliders[i]->setBounds(sliderX, resonanceY, resonanceButtonSize, resonanceButtonSize);
    }

    // =============================
    // ===== VOLUME + PITCH ROW ======
    // =============================

        int stepButtonSize = 60;
        int stepSpacing    = 20;
        int stepStartX     = 90;

        int stepButtonY    = 600;
        int sliderHeight   = 160;
        int sliderWidth    = stepButtonSize / 2; 
        int sliderY        = stepButtonY - sliderHeight - 10;

        for (int i = 0; i < 8; ++i)
        {
            int baseX = stepStartX + i * (stepButtonSize + stepSpacing);

            // Volume slider
            stepSliders[i]->setBounds(baseX, sliderY, sliderWidth, sliderHeight);

            // Pitch slider
            pitchSliders[i]->setBounds(baseX + sliderWidth, sliderY, sliderWidth, sliderHeight);

            // Step buttons
            stepButtons[i].setBounds(baseX, stepButtonY, stepButtonSize, stepButtonSize);
        }

    // =============================
    // ===== RIGHT SECTION =========
    // =============================

    // ADSR
    attackSlider.setBounds(815, 320, 60, 80);
    decaySlider.setBounds(905, 320, 60, 80);
    sustainSlider.setBounds(995, 320, 60, 80);
    releaseSlider.setBounds(1076, 320, 60, 80);

    // Single effects
    //pitchSlider.setBounds(859, 410, 60, 80);
    delaySlider.setBounds(949, 410, 60, 80);
    reverbSlider.setBounds(1036, 410, 60, 80);
    
    //Control buttons
    auto scaleX = getWidth()  / 1122.0f;
    auto scaleY = getHeight() / 597.0f;

    clearButton.setBounds(int(754 * scaleX), int(469 * scaleY), int(99 * scaleX), int(38 * scaleY));
    startButton.setBounds(int(754 * scaleX), int(525 * scaleY), int(99 * scaleX), int(38 * scaleY));
    randomiserButton.setBounds(int(872 * scaleX), int(469 * scaleY), int(99 * scaleX), int(38 * scaleY));
    scaleButton.setBounds(int(872 * scaleX), int(525 * scaleY), int(99 * scaleX), int(38 * scaleY));

    // Master Volume
    volumeSlider.setBounds(1073, 548, 100, 80);
}
