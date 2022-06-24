////////////////////////////////////////////////////////////////////////////////
// $Id: SignalDisplay.h 6655 2022-04-01 17:50:10Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A SignalDisplay class that renders GenericSignal data into a
//   given DisplayContext.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2022: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef SIGNAL_DISPLAY_H
#define SIGNAL_DISPLAY_H

#ifdef __BORLANDC__
#error Borland specific code has been removed from the SignalDisplay class.
#endif // __BORLANDC__

#include "Color.h"
#include "GUI.h"
#include "GenericSignal.h"
#include "Label.h"
#include "Lockable.h"
#include <set>
#include <vector>

#include <QColor>
#include <QFont>
#include <QPaintDevice>
#include <QPen>
#include <QPoint>
#include <QRect>
#include <QRegion>

class SignalDisplay
{
  private:
    enum
    {
        cNumSamplesDefault = 128,
        cMinValueDefault = -(1 << 15),
        cMaxValueDefault = (1 << 16) - 1,

        cChannelBase = 1, // displayed number of first channel
        cSampleBase = 0,  // displayed number of first sample

        cLabelWidth = 25,
        cAxisWidth = 2,
        cTickWidth = cAxisWidth,
        cTickLength = 4,
        cInitialMaxDisplayGroups = 32,
    };

    static const RGBColor cAxisColorDefault, cChannelColorsDefault[];

  public:
    enum eDisplayMode
    {
        polyline = 0,
        field2d,
        /* ... */
        numDisplayModes
    };

  public:
    SignalDisplay();
    virtual ~SignalDisplay();

  private:
    SignalDisplay(const SignalDisplay &);
    SignalDisplay &operator=(const SignalDisplay &);

  public:
    SignalDisplay &SetContext(const GUI::DrawContext &);
    SignalDisplay &Invalidate();
    SignalDisplay &Paint(const void *RegionHandle = NULL);

    SignalDisplay &WrapForward(const GenericSignal &);
    SignalDisplay &ScrollForward(const GenericSignal &);
    SignalDisplay &ScrollBack(const GenericSignal &);

    SignalDisplay &SetNumSamples(int);
    int NumSamples() const
    {
        return mNumSamples;
    }
    SignalDisplay &SetDisplayGroups(int);
    int DisplayGroups() const
    {
        return mNumDisplayGroups;
    }
    SignalDisplay &SetChannelGroupSize(int i)
    {
        mChannelGroupSize = i;
        return Invalidate();
    }
    int ChannelGroupSize() const
    {
        return mChannelGroupSize;
    }
    SignalDisplay &SetNumMarkerChannels(int n)
    {
        mMarkerChannels = n;
        return Invalidate();
    }
    int NumMarkerChannels() const
    {
        return mMarkerChannels;
    }

    SignalDisplay &SetMinValue(float f)
    {
        mMinValue = f;
        return Invalidate();
    }
    float MinValue() const
    {
        return mMinValue;
    }
    SignalDisplay &SetMaxValue(float f)
    {
        mMaxValue = f;
        return Invalidate();
    }
    float MaxValue() const
    {
        return mMaxValue;
    }

    SignalDisplay &SetSampleOffset(float f)
    {
        mSampleUnit.SetOffset(-f);
        return Invalidate();
    }
    float SampleOffset() const
    {
        return -mSampleUnit.Offset();
    }
    SignalDisplay &SetSampleUnit(const std::string &);
    const std::string &SampleUnit() const
    {
        return mSampleUnit.Symbol();
    }
    SignalDisplay &SetValueUnit(const std::string &);
    const std::string &ValueUnit() const
    {
        return mValueUnit.Symbol();
    }

    SignalDisplay &SetValueUnits(const ValueList<std::string> &);
    ValueList<std::string> ValueUnits() const;

    SignalDisplay &SetChannelUnit(const std::string &);
    const std::string &ChannelUnit() const
    {
        return mChannelUnit.Symbol();
    }

    SignalDisplay &SetUnitsPerSample(float f)
    {
        mSampleUnit.SetGain(f);
        return Invalidate();
    }
    float UnitsPerSample() const
    {
        return mSampleUnit.Gain();
    }
    SignalDisplay &SetUnitsPerValue(float f)
    {
        mValueUnit.SetGain(f);
        return Invalidate();
    }
    float UnitsPerValue() const
    {
        return mValueUnit.Gain();
    }
    SignalDisplay &SetUnitsPerChannel(float f)
    {
        mChannelUnit.SetGain(f);
        return Invalidate();
    }
    float UnitsPerChannel() const
    {
        return mChannelUnit.Gain();
    }

    SignalDisplay &SetAxisColor(const RGBColor &c)
    {
        mAxisColor = c;
        return Invalidate();
    }
    const RGBColor &AxisColor() const
    {
        return mAxisColor;
    }
    SignalDisplay &SetChannelColors(const ColorList &l)
    {
        mChannelColors = l;
        return Invalidate();
    }
    const ColorList &ChannelColors() const
    {
        return mChannelColors;
    }
    SignalDisplay &SetChannelStyles(const std::string &s)
    {
        mChannelStyles = s;
        return Invalidate();
    }
    const std::string &ChannelStyles() const
    {
        return mChannelStyles;
    }
    SignalDisplay &SetChannelLabels(const LabelList &l)
    {
        mChannelLabels = l;
        SyncLabelWidth();
        return Invalidate();
    }
    const LabelList &ChannelLabels() const
    {
        return mChannelLabels;
    }
    SignalDisplay &SetChannelLabelWidth(const std::string &s)
    {
        mChannelLabelWidth = s;
        SyncLabelWidth();
        return Invalidate();
    }
    const std::string &ChannelLabelWidth() const
    {
        return mChannelLabelWidth;
    }
    SignalDisplay &SetXAxisMarkers(const LabelList &l)
    {
        mXAxisMarkers = l;
        return Invalidate();
    }
    const LabelList &XAxisMarkers() const
    {
        return mXAxisMarkers;
    }

    SignalDisplay &SetValueColors(std::string);
    std::string ValueColors() const;

    SignalDisplay &SetTopGroup(int);
    int TopGroup() const
    {
        return mTopGroup;
    }

    SignalDisplay &SetDisplayMode(int);
    int DisplayMode() const
    {
        return mDisplayMode;
    }

    SignalDisplay &SetNumericValuesVisible(bool b)
    {
        mShowNumericValues = b;
        SyncLabelWidth();
        return Invalidate();
    }
    SignalDisplay &SetBaselinesVisible(bool b)
    {
        mShowBaselines = b;
        return Invalidate();
    }
    bool NumericValuesVisible() const
    {
        return mShowNumericValues;
    }
    bool BaselinesVisible() const
    {
        return mShowBaselines;
    }
    SignalDisplay &SetChannelLabelsVisible(bool b)
    {
        mShowChannelLabels = b;
        SyncLabelWidth();
        return Invalidate();
    }
    bool ChannelLabelsVisible() const
    {
        return mShowChannelLabels;
    }
    SignalDisplay &SetValueUnitVisible(bool b)
    {
        mShowValueUnit = b;
        mNumericValueWidth = 0;
        return Invalidate();
    }
    bool ValueUnitVisible() const
    {
        return mShowValueUnit;
    }

    SignalDisplay &SetColorDisplay(bool b)
    {
        mDisplayColors = b;
        return Invalidate();
    }
    bool ColorDisplay() const
    {
        return mDisplayColors;
    }

    SignalDisplay &SetInverted(bool b)
    {
        mInverted = b;
        return Invalidate();
    }
    bool Inverted() const
    {
        return mInverted;
    }

    SignalDisplay &SetFontSize(int i)
    {
        mFontSize = std::max(0, i);
        SyncLabelWidth();
        return Invalidate();
    }
    int FontSize() const
    {
        return mFontSize;
    }

    // Functions that centralize sample/channel -> pixel conversion in painting
    // and invalidating contexts.
  private:
    int mDataWidth, mDataHeight, mDataLeft, mLabelWidth, mNumericValueWidth, mMarkerHeight, mTickLength;

    void SyncLabelWidth();
    void SyncGraphics();
    void AdaptTo(const GenericSignal &);

  public:
    int SampleLeft(int s) const
    {
        return mDataLeft + (mNumSamples ? (s * mDataWidth) / int(mNumSamples) : 0);
    }
    int SampleRight(int s) const
    {
        return SampleLeft(s + 1);
    }
    int PosToSample(int p) const
    {
        return mDataWidth ? ((p - mDataLeft) * mNumSamples) / int(mDataWidth) : 0;
    }

  private:
    int MarkerChannelTop(int ch) const
    {
        return mMarkerChannels != 0 ? mMarkerHeight * ch + cAxisWidth : 0;
    }
    int MarkerChannelBottom(int ch) const
    {
        return mMarkerChannels != 0 ? MarkerChannelTop(ch) + mMarkerHeight : 0;
    }

    int GroupTop(int g) const
    {
        return MarkerChannelBottom(mMarkerChannels - 1) + cAxisWidth +
               (mNumDisplayGroups ? (g * mDataHeight) / int(mNumDisplayGroups) : 0);
    }
    int GroupBottom(int g) const
    {
        return GroupTop(g + 1);
    }

  public:
    int ChannelTop(int ch) const
    {
        return GroupTop(ChannelToGroup(ch));
    }
    int ChannelBottom(int ch) const
    {
        return GroupBottom(ChannelToGroup(ch));
    }
    int PosToChannel(int p)
    {
        return GroupToChannel(PosToGroup(p));
    }
    int NumChannels() const
    {
        return mData.Channels();
    }
    double Data(int ch, int el) const
    {
        return mData(ch, el);
    }

  private:
    int PosToGroup(int p);
    int ChannelToGroup(int ch) const
    {
        return ch / mChannelGroupSize;
    }
    int GroupToChannel(int g) const
    {
        return g * mChannelGroupSize;
    }

    double NormData(size_t i, size_t j) const
    {
        return (mData(i, j) - mMinValue) / (mMaxValue - mMinValue);
    }

    RGBColor ChannelColor(int ch) const
    {
        return mChannelColors[ch % mChannelColors.size()];
    }

    int NumericValuesLeft() const
    {
        return NumericValuesRight() - mNumericValueWidth;
    }
    int NumericValuesRight() const
    {
        return mDataLeft - 2 * cAxisWidth - cTickLength;
    }
    std::string GetNumericValue(int ch) const;

  private:
    eDisplayMode mDisplayMode;
    bool mShowCursor, mWrapAround, mTimeLabels, mShowNumericValues, mShowBaselines, mShowChannelLabels, mShowValueUnit,
        mDisplayColors, mInverted;
    int mNumSamples, mSampleCursor, mNumDisplayGroups, mMaxDisplayGroups, mNumDisplayChannels, mTopGroup,
        mChannelGroupSize, mMarkerChannels, mFontSize;
    double mMinValue, mMaxValue;
    std::vector<double> mMinValues, mMaxValues;
    PhysicalUnit mChannelUnit, mSampleUnit, mValueUnit;
    std::vector<PhysicalUnit> mValueUnits;
    RGBColor mAxisColor;
    ColorList mChannelColors;
    std::string mChannelStyles;
    std::vector<std::string> mChannelNameCache;
    LabelList mChannelLabels, mXAxisMarkers;
    std::string mChannelLabelWidth;
    std::vector<std::pair<double, RGBColor>> mValueColors;
    GenericSignal mData;
    Lockable<std::recursive_mutex> mDataLock;

  private:
    struct PaintInfo;
    void SetupPainting(PaintInfo &, const void *);
    void ClearBackground(const PaintInfo &);
    void DrawSignalPolyline(const PaintInfo &);
    void DrawSignalField2d(const PaintInfo &);
    void DrawMarkerChannels(const PaintInfo &);
    void DrawCursor(const PaintInfo &);
    void DrawXTicks(const PaintInfo &);
    void DrawYLabels(const PaintInfo &, bool inDrawTicks);
    void DrawAxes(const PaintInfo &);
    void DrawMarkers(const PaintInfo &);
    void DrawChannelLabels(const PaintInfo &);
    void DrawValueUnit(const PaintInfo &);
    void DrawNumericValue(const PaintInfo &, const QRect &, const char *);
    void CleanupPainting(PaintInfo &);

    // Qt graphics.
    QFont AxisFont();
    QFont MonoFont();
    QFont SymbolFont();
    QPaintDevice *mParent;
    QRect mDisplayRect, mDataRect, mCursorRect;
    QRegion mDisplayRgn;
    QPoint *mpSignalPoints;
    int mSymbolWidth;

    struct PaintInfo
    {
        QPainter *painter;
        const QRegion *updateRgn;
        QColor backgroundColor, cursorColor, axisColor, markerColor, labelColor;
        QBrush backgroundBrush, cursorBrush, axisBrush, markerBrush;
        QFont labelFont, monoFont, symbolFont;
        QPen baselinePen;
        int cursorWidth, markerWidth, axisY;
        std::vector<QPen> signalPens;
        std::vector<QBrush> signalBrushes;
        QString signalStyles;
    };
};

#endif // SIGNAL_DISPLAY_H
