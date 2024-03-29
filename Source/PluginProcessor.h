#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "BinaryData.h"
#include "CustomLookAndFeel.h"

class PluginProcessor : public juce::AudioProcessor
{
public:
  PluginProcessor();
  ~PluginProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

private:
  juce::AudioProcessorValueTreeState parameters;

  std::atomic<float>* amount = nullptr;
  std::atomic<float>* speed = nullptr;
  std::atomic<float>* makeup = nullptr;

  float* msMem;
  float* envMem;
  float* postMem;

  float currentSampleRate = 44100.0f;

  float t2p(float t) { return expf(-1.0f / (t * currentSampleRate)); }

  float sum(float* x, int numValues)
  {
    float y = 0.0f;
    for (auto i = 0; i < numValues; ++i) {
      y += x[i];
    }
    return y;
  }

  float avg(float* x, int numValues)
  {
    return sum(x, numValues) / (float)numValues;
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
