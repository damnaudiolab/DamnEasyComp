#pragma once

#include "PluginProcessor.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
  explicit PluginEditor(PluginProcessor&, juce::AudioProcessorValueTreeState&);
  ~PluginEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
  CustomLookAndFeel customLookAndFeel;

  int bodyWidth = 600;
  int bodyHeight = 360;

#define wCent bodyWidth / 100;
#define hCent bodyHeight / 100;

  int headerHeight = 36 * hCent;

  int sliderTextBoxWidth = 20 * wCent;
  int sliderTextBoxHeight = 10 * hCent;

  int logoReduce = 8 * hCent;
  int sliderReduce = 8 * hCent;

  int sliderLabelHeight = 5 * hCent;

  int sliderWidth = 30 * wCent;

  int sliderPosY = headerHeight + sliderReduce;
  int sliderHeight = bodyHeight - sliderPosY - sliderLabelHeight - sliderReduce;

  int amountSliderPosX = 5 * wCent;
  int speedSliderPosX = 35 * wCent;
  int makeupSliderPosX = 65 * wCent;

  int sliderLabelPosY = sliderPosY + sliderHeight;

  std::unique_ptr<juce::Drawable> logo;

  juce::Rectangle<int> bodyArea{ 0, 0, bodyWidth, bodyHeight };

  juce::Rectangle<int> headerArea{ 0, 0, bodyWidth, headerHeight };

  // amount slider
  juce::Rectangle<int> amountSliderArea{ amountSliderPosX,
                                         sliderPosY,
                                         sliderWidth,
                                         sliderHeight };
  juce::Slider amountSlider;
  std::unique_ptr<SliderAttachment> amountSliderAttachment;
  // amount slider label
  juce::Rectangle<int> amountSliderLabelArea{ amountSliderPosX,
                                              sliderLabelPosY,
                                              sliderWidth,
                                              sliderLabelHeight };
  juce::Label amountSliderLabel;

  // speed slider
  juce::Rectangle<int> speedSliderArea{ speedSliderPosX,
                                        sliderPosY,
                                        sliderWidth,
                                        sliderHeight };
  juce::Slider speedSlider;
  std::unique_ptr<SliderAttachment> speedSliderAttachment;
  // speed slider label
  juce::Rectangle<int> speedSliderLabelArea{ speedSliderPosX,
                                             sliderLabelPosY,
                                             sliderWidth,
                                             sliderLabelHeight };
  juce::Label speedSliderLabel;

  // makeup slider
  juce::Rectangle<int> makeupSliderArea{ makeupSliderPosX,
                                         sliderPosY,
                                         sliderWidth,
                                         sliderHeight };
  juce::Slider makeupSlider;
  std::unique_ptr<SliderAttachment> makeupSliderAttachment;
  // makeup slider label
  juce::Rectangle<int> makeupSliderLabelArea{ makeupSliderPosX,
                                              sliderLabelPosY,
                                              sliderWidth,
                                              sliderLabelHeight };
  juce::Label makeupSliderLabel;

  // others
  PluginProcessor& processorRef;
  juce::AudioProcessorValueTreeState& valueTreeState;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};