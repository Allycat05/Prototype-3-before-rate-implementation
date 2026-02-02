#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;

//==============================================================================

AcidSequencerAudioProcessor::AcidSequencerAudioProcessor()
// #ifndef JucePlugin_PreferredChannelConfigurations
//     : AudioProcessor (BusesProperties()
//         #if ! JucePlugin_IsMidiEffect
//             #if ! JucePlugin_IsSynth
//                 .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
//             #endif
//             .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
//         #endif
//       )
// #else
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true))
// #endif
    , apvts(*this, nullptr, "PARAMETERS", createParameters())
{
        for (int i = 0; i < 8; ++i)
            stepOn[i] = true;   // All steps ON at startup
}

AcidSequencerAudioProcessor::~AcidSequencerAudioProcessor()
{
}

//==============================================================================

const juce::String AcidSequencerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//

bool AcidSequencerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AcidSequencerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AcidSequencerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AcidSequencerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AcidSequencerAudioProcessor::getNumPrograms()
{
    return 1;
}

int AcidSequencerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AcidSequencerAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String AcidSequencerAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AcidSequencerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================

void AcidSequencerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);
    
    // Initialize step arrays
    for (int i = 0; i < 8; ++i)
    {
        stepVolume[i] = 0.0f;
        stepCutoff[i] = 0.5f;
        stepResonance[i] = 0.0f;
        stepDistortion[i] = 0.0f;
        stepPortamento[i] = 0.0f;
    }
    
    //SamplesPerStep from rate
    double bpm = *apvts.getRawParameterValue("rate");
    double beatsPerSecond = bpm / 60.0;
    double stepsPerSecond = beatsPerSecond * 4.0; // 4 steps per beat (16th notes)
    samplesPerStep = sampleRate / stepsPerSecond;
    sampleCounter = 0.0;
}

void AcidSequencerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AcidSequencerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout AcidSequencerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    constexpr int VERSION = 1;
    
    // Row 1: Waveform and Rate
    
        params.push_back(std::make_unique<juce::AudioParameterChoice>(
            ParameterID("waveform", VERSION), "Waveform", juce::StringArray("Saw", "Square", "Sine"), 0));
    
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            ParameterID("rate", VERSION), "Rate", 80.0f, 240.0f, 140.0f));
    
    // Row 2: FX
    
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            ParameterID("cutoff" + std::to_string(i), VERSION),"Cutoff" + std::to_string(i + 1), 0.0f, 1.0f, 0.5f));
    }
    
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
           ParameterID("distortion" + std::to_string(i), VERSION),"Distortion" + std::to_string(i + 1), 0.0f, 1.0f, 0.0f));
    }
    
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("portamento" + std::to_string(i), VERSION),"Portamento" + std::to_string(i + 1), 0.0f, 1.0f, 0.0f));
    }
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("resonance" + std::to_string(i), VERSION),"Resonance" + std::to_string(i + 1), 0.0f, 1.0f, 0.0f));
    }
    
    // Row 3: ADSR and others

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("attack", VERSION), "Attack", 0.1f, 1.0f, 0.1f));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("decay", VERSION), "Decay", 0.0f, 1.0f, 0.0f));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("sustain", VERSION), "Sustain", 0.0f, 1.0f, 0.0f));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("release", VERSION), "Release", 0.0f, 1.0f, 0.0f));
    
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("pitch", VERSION), "Pitch", 0.0f, 1.0f, 0.0f));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("delay", VERSION), "Delay", 0.0f, 1.0f, 0.0f));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("reverb", VERSION), "Reverb", 0.0f, 1.0f, 0.0f));
    
    // Row 4: System features
    
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("stepVolume" + std::to_string(i), VERSION),"Step Volume " + std::to_string(i + 1), 0.0f, 1.0f, 1.0f));
    }
    
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("stepPitch" + std::to_string(i), VERSION), "Step Pitch " + std::to_string(i + 1), 36.0f, 84.0f, 60.0f));
    }
    
       params.push_back(std::make_unique<juce::AudioParameterBool>(
                                                                   ParameterID("clear", VERSION), "Clear", false));

       params.push_back(std::make_unique<juce::AudioParameterBool>(
                                                                   ParameterID("startstop", VERSION), "Start/Stop", false));

       params.push_back(std::make_unique<juce::AudioParameterBool>(
                                                                   ParameterID("randomiser", VERSION), "Randomiser", false));

       params.push_back(std::make_unique<juce::AudioParameterBool>(
                                                                   ParameterID("scale", VERSION), "Scale", false));

        params.push_back(std::make_unique<juce::AudioParameterFloat>(
                                                                     ParameterID("masterVolume", VERSION), "Master Volume", 0.0f, 1.0f, 0.5f));
    // Step logic:
    
    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            ParameterID("stepOn" + std::to_string(i), VERSION),
            "Step " + std::to_string(i) + " On",
            true
        ));
    }
    
    return { params.begin(), params.end() };
}

//==============================================================================
void AcidSequencerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);

    const int numChannels = buffer.getNumChannels();
    const int numSamples  = buffer.getNumSamples();
    const double sampleRate = getSampleRate();

    constexpr double twoPi = 2.0 * juce::MathConstants<double>::pi;

   // if (auto* playHead = getPlayHead())
    {
       // if (auto pos = playHead->getPosition())
        {
           // if (pos->getPpqPosition().hasValue())
                //ppqPosition = *pos->getPpqPosition();

           // if (pos->getBpm().hasValue())
                //currentBpm = *pos->getBpm();

           // isPlaying = pos->getIsPlaying();
        }
    }

    if (!isPlaying)
        //lastPpqStepIndex = -1.0;

    for (int i = 0; i < numSamples; ++i)
    {
        //if (isPlaying && ppqPosition >= 0.0)
        {
           // const double ppqStepIndex = std::floor(ppqPosition / ppqPerStep);

           // if (ppqStepIndex != lastPpqStepIndex)
            {
               // lastPpqStepIndex = ppqStepIndex;

                float pitch  = *apvts.getRawParameterValue("stepPitch" + std::to_string(currentStep));
                float volume = *apvts.getRawParameterValue("stepVolume" + std::to_string(currentStep));
                bool stepOn  = *apvts.getRawParameterValue("stepOn" + std::to_string(currentStep));

                if (stepOn)
                {
                    const float frequency =
                        juce::MidiMessage::getMidiNoteInHertz((int)pitch);

                    phaseInc = frequency * twoPi / sampleRate;
                    gain = volume;
                }
                else
                {
                    gain = 0.0f;
                }

                currentStep++;
                if (currentStep >= 8)
                    currentStep = 0;
            }

           // ppqPosition += currentBpm / (60.0 * sampleRate);
        }

        float sample = std::sin(phasePos) * 0.1f;
        sample *= gain;
        
        phasePos += phaseInc;
        if (phasePos >= twoPi)
            phasePos -= twoPi;

        for (int ch = 0; ch < numChannels; ++ch)
            buffer.setSample(ch, i, sample);
        
    }
}
    //==============================================================================
    
    void AcidSequencerAudioProcessor::clearSequence()
    {
        for (int i = 0; i < 8; i++)
        {
            stepVolume[i]     = 0.0f;
            stepCutoff[i]     = 0.5f;
            stepResonance[i]  = 0.0f;
            stepDistortion[i] = 0.0f;
            stepPortamento[i] = 0.0f;
        }
    }
    
    void AcidSequencerAudioProcessor::randomiseSequence()
    {
        for (int i = 0; i < 8; i++)
        {
            stepVolume[i]     = juce::Random::getSystemRandom().nextFloat();
            stepCutoff[i]     = juce::Random::getSystemRandom().nextFloat();
            stepResonance[i]  = juce::Random::getSystemRandom().nextFloat();
            stepDistortion[i] = juce::Random::getSystemRandom().nextFloat();
            stepPortamento[i] = juce::Random::getSystemRandom().nextFloat();
        }
    }
    
    void AcidSequencerAudioProcessor::cycleScale()
    {
        currentScale = (currentScale + 1) % 4;
    }
    
    //==============================================================================
    
    bool AcidSequencerAudioProcessor::hasEditor() const
    {
        return true;
    }
    
    juce::AudioProcessorEditor* AcidSequencerAudioProcessor::createEditor()
    {
        return new AcidSequencerAudioProcessorEditor (*this);
    }
    
    //==============================================================================
    
    void AcidSequencerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
    {
        juce::ignoreUnused(destData);
    }
    
    void AcidSequencerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
    {
        juce::ignoreUnused(data, sizeInBytes);
    }
    
    //==============================================================================
    
    juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
    {
        return new AcidSequencerAudioProcessor();
    }
