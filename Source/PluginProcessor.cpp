#include "PluginProcessor.h"

#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
  : AudioProcessor(
      BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
        )
  , parameters(*this,
               nullptr,
               juce::Identifier(JucePlugin_Name),
               {
                 std::make_unique<juce::AudioParameterFloat>(
                   "amount",
                   "Amount",
                   juce::NormalisableRange<float>(0.0f, 200.0f, 1.0f),
                   100.0f),
                 std::make_unique<juce::AudioParameterFloat>(
                   "speed",
                   "Speed",
                   juce::NormalisableRange<float>(25.0f, 2000.0f, 1.0f, 0.5f),
                   100.0f),
                 std::make_unique<juce::AudioParameterFloat>(
                   "makeup",
                   "Make Up",
                   juce::NormalisableRange<float>(0.0f, 12.f, 0.1f),
                   6.0f),
               })
{
  amount = parameters.getRawParameterValue("amount");
  speed = parameters.getRawParameterValue("speed");
  makeup = parameters.getRawParameterValue("makeup");
}

PluginProcessor::~PluginProcessor() {}

//==============================================================================
const juce::String
PluginProcessor::getName() const
{
  return JucePlugin_Name;
}

bool
PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool
PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool
PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double
PluginProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int
PluginProcessor::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not
            // really implementing programs.
}

int
PluginProcessor::getCurrentProgram()
{
  return 0;
}

void
PluginProcessor::setCurrentProgram(int index)
{
  juce::ignoreUnused(index);
}

const juce::String
PluginProcessor::getProgramName(int index)
{
  juce::ignoreUnused(index);
  return {};
}

void
PluginProcessor::changeProgramName(int index, const juce::String& newName)
{
  juce::ignoreUnused(index, newName);
}

//==============================================================================
void
PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(samplesPerBlock);

  const auto numChannels = (unsigned)getTotalNumInputChannels();

  msMem = new float[numChannels]();
  envMem = new float[numChannels]();
  postMem = new float[numChannels]();

  currentSampleRate = (float)sampleRate;
}

void
PluginProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool
PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void
PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                              juce::MidiBuffer& midiMessages)
{
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  const auto numSamples = buffer.getNumSamples();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, numSamples);

  for (auto ch = 0; ch < totalNumInputChannels; ++ch) {
    auto* inputBuffer = buffer.getReadPointer(ch);
    auto* outputBuffer = buffer.getWritePointer(ch);

    for (auto i = 0; i < numSamples; ++i) {
      auto x = inputBuffer[i];

      auto rms = sqrtf(msMem[ch]);

      auto pole = t2p(expf(-1.0f * rms * *amount * 0.01f) * (*speed * 0.001f));

      auto ms = (1.0f - pole) * (postMem[ch] * postMem[ch]) + pole * msMem[ch];
      msMem[ch] = ms;

      auto gain = juce::Decibels::decibelsToGain(-1.0f * rms * *amount);

      auto y =
        x * gain * juce::Decibels::decibelsToGain(*makeup * *amount * 0.01f);
      postMem[ch] = y;

      outputBuffer[i] = y;
    }
  }
}

//==============================================================================
bool
PluginProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor*
PluginProcessor::createEditor()
{
  return new PluginEditor(*this, parameters);
}

//==============================================================================
void
PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

  auto state = parameters.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void
PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr<juce::XmlElement> xmlState(
    getXmlFromBinary(data, sizeInBytes));

  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(parameters.state.getType()))
      parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
  return new PluginProcessor();
}
