#include <algorithm>
#include <charconv>
#include <span>
#include <string>

#include "gtest/gtest.h"

#include "color_data.h"
#include "common/string_helper.h"
#include "faker-cxx/color.h"
#include "faker-cxx/types/hex.h"
#include "number_data.h"

using namespace ::testing;
using namespace faker;
using namespace faker::color;

std::span<const std::string_view> getColors(Locale locale)
{
    switch (locale)
    {
    case Locale::pl_PL:
        return polishColors;
    default:
        return englishColors;
    }
}

class ColorNameTest : public TestWithParam<Locale>
{
public:
};

TEST_P(ColorNameTest, shouldGenerateColorName)
{
    const auto locale = GetParam();

    const auto generatedColorName = name(locale);

    const auto exceptedColors = getColors(locale);

    ASSERT_TRUE(std::ranges::any_of(exceptedColors, [generatedColorName](const std::string_view& colorName)
                                    { return colorName == generatedColorName; }));
}

INSTANTIATE_TEST_SUITE_P(TestColorNameByLocale, ColorNameTest, ValuesIn(locales),
                         [](const TestParamInfo<Locale>& paramInfo) { return toString(paramInfo.param); });

class ColorTest : public Test
{
public:
};

TEST_F(ColorTest, shouldGenerateRgbColorWithoutAlpha)
{
    const auto generatedRgbColor = rgb();

    const auto rgbNumbers = common::split(generatedRgbColor.substr(4, generatedRgbColor.size() - 1), " ");

    int red, green, blue;
    std::from_chars(rgbNumbers[0].data(), rgbNumbers[0].data() + rgbNumbers[0].size(), red);
    std::from_chars(rgbNumbers[1].data(), rgbNumbers[1].data() + rgbNumbers[1].size(), green);
    std::from_chars(rgbNumbers[2].data(), rgbNumbers[2].data() + rgbNumbers[2].size(), blue);

    ASSERT_TRUE(generatedRgbColor.starts_with("rgb("));
    ASSERT_TRUE(generatedRgbColor.ends_with(")"));
    ASSERT_TRUE(red >= 0 && red <= 255);
    ASSERT_TRUE(green >= 0 && red <= 255);
    ASSERT_TRUE(blue >= 0 && red <= 255);
}

TEST_F(ColorTest, shouldGenerateRgbColorWithAlpha)
{
    const auto generatedRgbaColor = rgb(true);

    const auto rgbaNumbers = common::split(generatedRgbaColor.substr(5, generatedRgbaColor.size() - 1), " ");

    int red, green, blue, alpha;
    std::from_chars(rgbaNumbers[0].data(), rgbaNumbers[0].data() + rgbaNumbers[0].size(), red);
    std::from_chars(rgbaNumbers[1].data(), rgbaNumbers[1].data() + rgbaNumbers[1].size(), green);
    std::from_chars(rgbaNumbers[2].data(), rgbaNumbers[2].data() + rgbaNumbers[2].size(), blue);
    std::from_chars(rgbaNumbers[3].data(), rgbaNumbers[3].data() + rgbaNumbers[3].size(), alpha);

    ASSERT_TRUE(generatedRgbaColor.starts_with("rgba("));
    ASSERT_TRUE(generatedRgbaColor.ends_with(")"));
    ASSERT_TRUE(red >= 0 && red <= 255);
    ASSERT_TRUE(green >= 0 && red <= 255);
    ASSERT_TRUE(blue >= 0 && red <= 255);
    ASSERT_TRUE(alpha >= 0 && alpha <= 1);
}

TEST_F(ColorTest, shouldGenerateHexColorWithoutAlpha)
{
    const auto hexadecimal = hex();

    const auto prefix = hexadecimal.substr(0, 1);
    const auto hexNumber = hexadecimal.substr(1);

    ASSERT_EQ(hexadecimal.size(), 7);
    ASSERT_EQ(prefix, "#");
    ASSERT_TRUE(
        std::ranges::any_of(hexNumber, [hexNumber](char hexNumberCharacter)
                            { return number::hexLowerCharacters.find(hexNumberCharacter) != std::string::npos; }));
}

TEST_F(ColorTest, shouldGenerateHexColorWithAlpha)
{
    const auto hexadecimal = hex(HexCasing::Upper, HexPrefix::ZeroX, true);

    const auto prefix = hexadecimal.substr(0, 2);
    const auto hexNumber = hexadecimal.substr(2);

    ASSERT_EQ(hexadecimal.size(), 10);
    ASSERT_EQ(prefix, "0x");
    ASSERT_TRUE(
        std::ranges::any_of(hexNumber, [hexNumber](char hexNumberCharacter)
                            { return number::hexUpperCharacters.find(hexNumberCharacter) != std::string::npos; }));
}

TEST_F(ColorTest, shouldGenerateHslWithoutAlpha)
{
    const auto generatedHslColor = hsl();
    const auto hslValues = common::split(generatedHslColor.substr(4, generatedHslColor.size() - 1), " ");

    int hue, staturation, lightness;

    std::from_chars(hslValues[0].data(), hslValues[0].data() + hslValues[0].size(), hue);
    std::from_chars(hslValues[1].data(), hslValues[1].data() + hslValues[1].size(), staturation);
    std::from_chars(hslValues[2].data(), hslValues[2].data() + hslValues[2].size(), lightness);

    ASSERT_TRUE(generatedHslColor.starts_with("hsl("));
    ASSERT_TRUE(generatedHslColor.ends_with(")"));
    ASSERT_TRUE(hue >= 0 && hue <= 360);
    ASSERT_TRUE(staturation >= 0 && staturation <= 100);
    ASSERT_TRUE(lightness >= 0 && lightness <= 100);
}

TEST_F(ColorTest, shouldGenerateHslWithAlpha)
{
    const auto generatedHslaColor = hsl(true);
    const auto hslValues = common::split(generatedHslaColor.substr(5, generatedHslaColor.size() - 1), " ");

    int hue, saturation, lightness;

    std::from_chars(hslValues[0].data(), hslValues[0].data() + hslValues[0].size(), hue);
    std::from_chars(hslValues[1].data(), hslValues[1].data() + hslValues[1].size(), saturation);
    std::from_chars(hslValues[2].data(), hslValues[2].data() + hslValues[2].size(), lightness);

    auto offset = hslValues[3].size();
    const auto alpha = std::stod(hslValues[3], &offset);

    ASSERT_TRUE(generatedHslaColor.starts_with("hsla("));
    ASSERT_TRUE(generatedHslaColor.ends_with(")"));
    ASSERT_TRUE(hue >= 0 && hue <= 360);
    ASSERT_TRUE(saturation >= 0 && saturation <= 100);
    ASSERT_TRUE(lightness >= 0 && lightness <= 100);
    ASSERT_TRUE(alpha >= 0 && alpha <= 1);
}

TEST_F(ColorTest, shouldGenerateLchWithoutAlpha)
{
    const auto generatedLchColor = lch();
    const auto lchValues = common::split(generatedLchColor.substr(4, generatedLchColor.size() - 1), " ");

    int luminance, chroma, hue;

    std::from_chars(lchValues[0].data(), lchValues[0].data() + lchValues[0].size(), luminance);
    std::from_chars(lchValues[1].data(), lchValues[1].data() + lchValues[1].size(), chroma);
    std::from_chars(lchValues[2].data(), lchValues[2].data() + lchValues[2].size(), hue);

    ASSERT_TRUE(generatedLchColor.starts_with("lch("));
    ASSERT_TRUE(generatedLchColor.ends_with(")"));
    ASSERT_TRUE(luminance >= 0 && luminance <= 100);
    ASSERT_TRUE(chroma >= 0 && chroma <= 100);
    ASSERT_TRUE(hue >= 0 && hue <= 360);
}

TEST_F(ColorTest, shouldGenerateLchWithAlpha)
{
    const auto generatedLchaColor = lch(true);
    const auto lchValues = common::split(generatedLchaColor.substr(5, generatedLchaColor.size() - 1), " ");

    int luminance, chroma, hue;

    std::from_chars(lchValues[0].data(), lchValues[0].data() + lchValues[0].size(), luminance);
    std::from_chars(lchValues[1].data(), lchValues[1].data() + lchValues[1].size(), chroma);
    std::from_chars(lchValues[2].data(), lchValues[2].data() + lchValues[2].size(), hue);

    auto offset = lchValues[3].size();
    const auto alpha = std::stod(lchValues[3], &offset);

    ASSERT_TRUE(generatedLchaColor.starts_with("lcha("));
    ASSERT_TRUE(generatedLchaColor.ends_with(")"));
    ASSERT_TRUE(luminance >= 0 && luminance <= 100);
    ASSERT_TRUE(chroma >= 0 && chroma <= 100);
    ASSERT_TRUE(hue >= 0 && hue <= 360);
    ASSERT_TRUE(alpha >= 0 && alpha <= 1);
}

TEST_F(ColorTest, shouldGenerateCmykColor)
{
    const auto generatedCmykColor = cmyk();
    const auto cmykValues = common::split(generatedCmykColor.substr(5, generatedCmykColor.size() - 1), " ");

    auto offset = cmykValues[0].size();
    const auto cyan = std::stod(cmykValues[0], &offset);
    offset = cmykValues[1].size();
    const auto magenta = std::stod(cmykValues[1], &offset);
    offset = cmykValues[2].size();
    const auto yellow = std::stod(cmykValues[2], &offset);
    offset = cmykValues[3].size();
    const auto key = std::stod(cmykValues[3], &offset);

    ASSERT_TRUE(generatedCmykColor.starts_with("cmyk("));
    ASSERT_TRUE(generatedCmykColor.ends_with(")"));
    ASSERT_TRUE(0. <= cyan && cyan <= 1.);
    ASSERT_TRUE(0. <= magenta && magenta <= 1.);
    ASSERT_TRUE(0. <= yellow && yellow <= 1.);
    ASSERT_TRUE(0. <= key && key <= 1.);
}

TEST_F(ColorTest, shouldGenerateLabColor)
{
    const auto generatedLabColor = lab();
    const auto labValues = common::split(generatedLabColor.substr(4, generatedLabColor.size() - 1), " ");

    auto offset = labValues[0].size();
    const auto lightness = std::stod(labValues[0], &offset);
    offset = labValues[1].size();
    const auto redGreenValue = std::stod(labValues[1], &offset);
    offset = labValues[2].size();
    const auto blueYellowValue = std::stod(labValues[2], &offset);

    ASSERT_TRUE(generatedLabColor.starts_with("lab("));
    ASSERT_TRUE(generatedLabColor.ends_with(")"));
    ASSERT_TRUE(lightness >= 0. && lightness <= 100.);
    ASSERT_TRUE(redGreenValue >= -128. && redGreenValue <= 128.);
    ASSERT_TRUE(blueYellowValue >= -128. && blueYellowValue <= 128.);
}

TEST_F(ColorTest, shouldGenerateHsb)
{
    const auto generatedHsbColor = hsb();
    const auto hsbValues = common::split(generatedHsbColor.substr(4, generatedHsbColor.size() - 1), " ");

    int hue, saturation, brightness;

    std::from_chars(hsbValues[0].data(), hsbValues[0].data() + hsbValues[0].size(), hue);
    std::from_chars(hsbValues[1].data(), hsbValues[1].data() + hsbValues[1].size(), saturation);
    std::from_chars(hsbValues[2].data(), hsbValues[2].data() + hsbValues[2].size(), brightness);

    ASSERT_TRUE(generatedHsbColor.starts_with("hsb("));
    ASSERT_TRUE(generatedHsbColor.ends_with(")"));
    ASSERT_TRUE(hue >= 0 && hue <= 360);
    ASSERT_TRUE(saturation >= 0 && saturation <= 100);
    ASSERT_TRUE(brightness >= 0 && brightness <= 100);
}

TEST_F(ColorTest, shouldGenerateHsv)
{
    const auto generatedHsvColor = hsv();
    const auto hsvValues = common::split(generatedHsvColor.substr(4, generatedHsvColor.size() - 1), " ");

    int hue, saturation, brightness;

    std::from_chars(hsvValues[0].data(), hsvValues[0].data() + hsvValues[0].size(), hue);
    std::from_chars(hsvValues[1].data(), hsvValues[1].data() + hsvValues[1].size(), saturation);
    std::from_chars(hsvValues[2].data(), hsvValues[2].data() + hsvValues[2].size(), brightness);

    ASSERT_TRUE(generatedHsvColor.starts_with("hsv("));
    ASSERT_TRUE(generatedHsvColor.ends_with(")"));
    ASSERT_TRUE(hue >= 0 && hue <= 360);
    ASSERT_TRUE(saturation >= 0 && saturation <= 100);
    ASSERT_TRUE(brightness >= 0 && brightness <= 100);
}

TEST_F(ColorTest, shouldGenerateHwb)
{
    const auto generatedHwbColor = hwb();
    const auto hwbValues = common::split(generatedHwbColor.substr(4, generatedHwbColor.size() - 1), " ");

    int hue, whiteness, blackness;

    std::from_chars(hwbValues[0].data(), hwbValues[0].data() + hwbValues[0].size(), hue);
    std::from_chars(hwbValues[1].data(), hwbValues[1].data() + hwbValues[1].size(), whiteness);
    std::from_chars(hwbValues[2].data(), hwbValues[2].data() + hwbValues[2].size(), blackness);

    ASSERT_TRUE(generatedHwbColor.starts_with("hwb("));
    ASSERT_TRUE(generatedHwbColor.ends_with(")"));
    ASSERT_TRUE(hue >= 0 && hue <= 360);
    ASSERT_TRUE(whiteness >= 0 && whiteness <= 100);
    ASSERT_TRUE(blackness >= 0 && blackness <= 100);
}

TEST_F(ColorTest, shouldGenerateYuv)
{
    const auto generatedYuvColor = yuv();
    const auto yuvValues = common::split(generatedYuvColor.substr(4, generatedYuvColor.size() - 1), " ");

    int luminance, chrominanceBlueColor, chrominanceRedColor;

    std::from_chars(yuvValues[0].data(), yuvValues[0].data() + yuvValues[0].size(), luminance);
    std::from_chars(yuvValues[1].data(), yuvValues[1].data() + yuvValues[1].size(), chrominanceBlueColor);
    std::from_chars(yuvValues[2].data(), yuvValues[2].data() + yuvValues[2].size(), chrominanceRedColor);

    ASSERT_TRUE(generatedYuvColor.starts_with("yuv("));
    ASSERT_TRUE(generatedYuvColor.ends_with(")"));
    ASSERT_TRUE(luminance >= 0 && luminance <= 255);
    ASSERT_TRUE(chrominanceBlueColor >= 0 && chrominanceBlueColor <= 255);
    ASSERT_TRUE(chrominanceRedColor >= 0 && chrominanceRedColor <= 255);
}

TEST_F(ColorTest, shouldGenerateColorSpace)
{
    const auto generatedColorSpace = space();

    const std::array<std::string_view, 20> expectedColorSpaces = {
        "sRGB", "Adobe RGB", "ProPhoto RGB", "DCI-P3",       "Rec. 709",        "Rec. 2020",       "CMYK",
        "XYZ",  "Lab",       "ACES",         "CIE 1931 XYZ", "CIE 1976 L*a*b*", "CIE 1976 L*u*v*", "CIEUVW",
        "Y'UV", "Y'IQ",      "Y'DbDr",       "YCC",          "YPbPr",           "YPbPr601"};

    ASSERT_TRUE(std::any_of(expectedColorSpaces.begin(), expectedColorSpaces.end(),
                            [generatedColorSpace](const std::string_view& colorSpace)
                            { return colorSpace == generatedColorSpace; }));
}