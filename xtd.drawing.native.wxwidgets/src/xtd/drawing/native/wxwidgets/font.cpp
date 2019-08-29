#include <xtd/drawing/native/font.hpp>
#include <wx/app.h>
#include <wx/dcscreen.h>
#include <wx/font.h>


namespace {
#if defined(__WXOSX__)
  float points_to_native_font_graphics_untit(float size) {
    float dpi = wxTheApp ? wxScreenDC().GetPPI().GetHeight() : 72.0f;
    return size / dpi * 96.0f;  // font is in pixels and not in points
  }

  float native_font_graphics_untit_to_points(float size) {
    float dpi = wxTheApp ? wxScreenDC().GetPPI().GetHeight() : 72.0f;
    return size / 96.0f * dpi;  // font is in pixels and not in points
  }
#else
  float points_to_native_font_graphics_untit(float size) {
    return size;  // font is in points
  }

  float native_font_graphics_untit_to_points(float size) {
    return size;  // font is in points
  }
#endif
}

using namespace xtd::drawing::native;

intptr_t font::create(const std::string& name, float em_size, bool bold, bool italic, bool underline, bool strikeout, uint8_t gdi_char_set, bool gdi_vertical_font) {
  wxFont* font = new wxFont(points_to_native_font_graphics_untit(em_size), wxFontFamily::wxFONTFAMILY_DEFAULT, italic ? wxFontStyle::wxFONTSTYLE_ITALIC : wxFontStyle::wxFONTSTYLE_NORMAL, bold ? wxFontWeight::wxFONTWEIGHT_BOLD : wxFontWeight::wxFONTWEIGHT_NORMAL, underline, name);
  font->SetPointSize(points_to_native_font_graphics_untit(em_size));
  font->SetStrikethrough(strikeout);
  return reinterpret_cast<intptr_t>(font);
}

intptr_t font::create_from_hdc(intptr_t hdc) {
  return reinterpret_cast<intptr_t>(new wxFont(reinterpret_cast<wxDC*>(hdc)->GetFont()));
}

intptr_t font::create_from_hfont(intptr_t hfont) {
  return reinterpret_cast<intptr_t>(new wxFont(*reinterpret_cast<wxFont*>(hfont)));
}

void font::destroy(intptr_t font) {
  delete reinterpret_cast<wxFont*>(font);
}

int32_t font::dpi() {
  if (!wxTheApp) return 0;
  wxScreenDC hdc;
  wxSize dpi = hdc.GetPPI();
  return dpi.GetHeight();
}

void font::get_information(intptr_t font, std::string& name, float& em_size, bool& bold, bool& italic, bool& underline, bool& strikeout, uint8_t& gdi_char_set, bool& gdi_vertical_font) {
  wxFont* wx_font = reinterpret_cast<wxFont*>(font);
  name = wx_font->GetFaceName();
  em_size = native_font_graphics_untit_to_points(static_cast<float>(wx_font->GetPointSize()));
  bold = wx_font->GetWeight() > wxFontWeight::wxFONTWEIGHT_NORMAL;
  italic = wx_font->GetStyle() > wxFontStyle::wxFONTSTYLE_NORMAL;
  underline = wx_font->GetUnderlined();
  strikeout = wx_font->GetStrikethrough();
  gdi_char_set = 0;
  gdi_vertical_font = false;
}

float font::height(intptr_t font) {
  if (!wxTheApp) return 0;
  wxScreenDC hdc;
  wxFont current_font = hdc.GetFont();
  hdc.SetFont(*reinterpret_cast<wxFont*>(font));
  wxFontMetrics metrics = hdc.GetFontMetrics();
  hdc.SetFont(current_font);
  return static_cast<float>(metrics.height);
}

float font::height(intptr_t font, intptr_t hdc) {
  if (!wxTheApp) return 0;
  wxFont current_font = reinterpret_cast<wxDC*>(hdc)->GetFont();
  reinterpret_cast<wxDC*>(hdc)->SetFont(*reinterpret_cast<wxFont*>(font));
  wxFontMetrics metrics = reinterpret_cast<wxDC*>(hdc)->GetFontMetrics();
  reinterpret_cast<wxDC*>(hdc)->SetFont(current_font);
  return static_cast<float>(metrics.height);
}

float font::height(intptr_t font, float dpi) {
  if (!wxTheApp) return 0;
  wxScreenDC hdc;
  wxSize current_dpi = hdc.GetPPI();
  wxFont current_font = hdc.GetFont();
  hdc.SetFont(*reinterpret_cast<wxFont*>(font));
  wxFontMetrics metrics = hdc.GetFontMetrics();
  hdc.SetFont(current_font);
  return static_cast<float>(metrics.height) / current_dpi.GetWidth() * dpi;
}
