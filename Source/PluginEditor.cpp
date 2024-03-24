#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p,
                           juce::AudioProcessorValueTreeState& vts)
  : AudioProcessorEditor(&p)
  , processorRef(p)
  , valueTreeState(vts)
{
  juce::ignoreUnused(processorRef);

  logo = juce::Drawable::createFromImageData(BinaryData::logo_svg,
                                             BinaryData::logo_svgSize);
  logo->setTransformToFit(headerArea.reduced(logoReduce).toFloat(),
                          juce::RectanglePlacement::centred);
  addAndMakeVisible(logo.get());

  setSize(bodyWidth, bodyHeight);

  // amount slider
  addAndMakeVisible(amountSlider);
  amountSliderAttachment.reset(
    new SliderAttachment(valueTreeState, "amount", amountSlider));

  amountSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  amountSlider.setTextBoxStyle(
    juce::Slider::TextBoxBelow, false, sliderTextBoxWidth, sliderTextBoxHeight);
  amountSlider.setNumDecimalPlacesToDisplay(1);
  amountSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                         juce::Colours::transparentBlack);
  amountSlider.setBounds(amountSliderArea);
  // amount slider label
  addAndMakeVisible(amountSliderLabel);
  amountSliderLabel.setText("Amount",
                            juce::NotificationType::dontSendNotification);
  amountSliderLabel.setJustificationType(juce::Justification::centred);
  amountSliderLabel.setBounds(amountSliderLabelArea);

  // speed slider
  addAndMakeVisible(speedSlider);
  speedSliderAttachment.reset(
    new SliderAttachment(valueTreeState, "speed", speedSlider));

  speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  speedSlider.setTextBoxStyle(
    juce::Slider::TextBoxBelow, false, sliderTextBoxWidth, sliderTextBoxHeight);
  speedSlider.setNumDecimalPlacesToDisplay(1);
  speedSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                        juce::Colours::transparentBlack);
  speedSlider.setBounds(speedSliderArea);
  // speed slider label
  addAndMakeVisible(speedSliderLabel);
  speedSliderLabel.setText("Speed",
                           juce::NotificationType::dontSendNotification);
  speedSliderLabel.setJustificationType(juce::Justification::centred);
  speedSliderLabel.setBounds(speedSliderLabelArea);

  // makeup slider
  addAndMakeVisible(makeupSlider);
  makeupSliderAttachment.reset(
    new SliderAttachment(valueTreeState, "makeup", makeupSlider));

  makeupSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  makeupSlider.setTextBoxStyle(
    juce::Slider::TextBoxBelow, false, sliderTextBoxWidth, sliderTextBoxHeight);
  makeupSlider.setNumDecimalPlacesToDisplay(2);
  makeupSlider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId,
                         juce::Colours::transparentBlack);
  makeupSlider.setBounds(makeupSliderArea);
  // makeup slider label
  addAndMakeVisible(makeupSliderLabel);
  makeupSliderLabel.setText("Make Up",
                            juce::NotificationType::dontSendNotification);
  makeupSliderLabel.setJustificationType(juce::Justification::centred);
  makeupSliderLabel.setBounds(makeupSliderLabelArea);
}

PluginEditor::~PluginEditor() {}

void
PluginEditor::paint(juce::Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(
    getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setColour(customLookAndFeel.colourPalette[CustomLookAndFeel::black]);
  g.fillRect(headerArea);
}

void
PluginEditor::resized()
{
}