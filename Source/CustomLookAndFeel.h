class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
  CustomLookAndFeel()
  {
    juce::LookAndFeel::setDefaultLookAndFeel(this);
    setColour(juce::ResizableWindow::backgroundColourId, colourPalette[grey]);
  }
  ~CustomLookAndFeel() override {}

  static const juce::Font getCustomFont()
  {
    static auto typeface = juce::Typeface::createSystemTypefaceFor(
      BinaryData::BarlowRegular_ttf, BinaryData::BarlowRegular_ttfSize);
    return juce::Font(typeface);
  }

  juce::Typeface::Ptr getTypefaceForFont(const juce::Font&) override
  {
    return getCustomFont().getTypefacePtr();
  }

  void drawRotarySlider(juce::Graphics& g,
                        int x,
                        int y,
                        int width,
                        int height,
                        float sliderPos,
                        float rotaryStartAngle,
                        float rotaryEndAngle,
                        juce::Slider& sliderRef) override
  {
    auto sliderMin = (float)sliderRef.getMinimum();
    auto sliderMax = (float)sliderRef.getMaximum();
    auto sliderCenter =
      juce::MathConstants<float>::pi * sliderMin / (sliderMin - sliderMax);
    auto toAngle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto lineW = juce::jmin(14.0f, radius * 0.5f);
    auto knobRadius = radius - lineW * 2;
    auto arcRadius = radius - lineW * 0.5f;

    auto outlineColour = colourPalette[black];
    auto valueFillColour = colourPalette[green];
    auto knobFillColour = colourPalette[white];
    auto thumbColour = colourPalette[black];

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                rotaryEndAngle,
                                true);
    g.setColour(outlineColour);
    g.strokePath(backgroundArc,
                 juce::PathStrokeType(lineW,
                                      juce::PathStrokeType::curved,
                                      juce::PathStrokeType::butt));

    juce::Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(),
                           bounds.getCentreY(),
                           arcRadius,
                           arcRadius,
                           sliderCenter,
                           rotaryStartAngle,
                           toAngle,
                           true);
    g.setColour(valueFillColour);
    g.strokePath(valueArc,
                 juce::PathStrokeType(lineW,
                                      juce::PathStrokeType::curved,
                                      juce::PathStrokeType::butt));

    g.setColour(knobFillColour);
    g.fillEllipse(bounds.getCentreX() - knobRadius,
                  bounds.getCentreY() - knobRadius,
                  knobRadius * 2,
                  knobRadius * 2);

    auto pointerDistance = knobRadius * 0.8f;
    auto pointerLength = knobRadius * 0.4f;
    juce::Path pointer;
    pointer.startNewSubPath(0.0f, -pointerLength);
    pointer.lineTo(0.0f, -pointerDistance);
    pointer.closeSubPath();
    pointer.applyTransform(juce::AffineTransform::rotation(toAngle).translated(
      bounds.getCentreX(), bounds.getCentreY()));
    g.setColour(thumbColour);
    g.strokePath(pointer,
                 juce::PathStrokeType(lineW * 0.5f,
                                      juce::PathStrokeType::curved,
                                      juce::PathStrokeType::rounded));
  }

  virtual juce::Label* createSliderTextBox(juce::Slider& slider) override
  {
    auto* l = juce::LookAndFeel_V4::createSliderTextBox(slider);
    l->setFont(getCustomFont().withHeight(fontSize));
    l->setJustificationType(juce::Justification::centred);
    l->setColour(juce::Label::textWhenEditingColourId, colourPalette[white]);
    l->setColour(juce::Label::backgroundWhenEditingColourId,
                 colourPalette[grey]);
    return l;
  }

  virtual void drawLabel(juce::Graphics& g, juce::Label& label) override
  {
    g.fillAll(label.findColour(juce::Label::backgroundColourId));
    g.setColour(label.findColour(juce::Label::textColourId));
    g.setFont(getCustomFont().withHeight(fontSize));
    g.drawFittedText(
      label.getText(), label.getLocalBounds(), label.getJustificationType(), 1);
  }

  enum colourIdx
  {
    grey,
    black,
    green,
    white,
  };

  juce::Array<juce::Colour> colourPalette = {
    juce::Colour(0xff2a2e33),
    juce::Colour(0xff1e2326),
    juce::Colour(0xff00cc99),
    juce::Colour(0xfff0f8ff),
  };

private:
  float fontSize = 24.0f;
};