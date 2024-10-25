#pragma once

#include <Siv3D.hpp>

class Question {
 public:
  Question(const String& question, bool answer, const String& string)
      : m_question(question), m_answer(answer), m_string(string), m_mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}), m_subFont(Font{FontMethod::MSDF, 48}) {}
  Question(const String& question, bool answer, const Texture& texture)
      : m_question(question), m_answer(answer), m_texture(texture), m_mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}), m_subFont(Font{FontMethod::MSDF, 48}) {}

  const String m_question;
  const bool m_answer;
  const String m_string;
  const Texture m_texture;
  const Font m_mainFont;
  const Font m_subFont;

  inline void draw() const {
    // 青い四角を描く | Draw a rectangle
    Rect{5, 5, 789, 180}.draw(HSV{220, 0.8, 0.9});
    // 指定した範囲内にテキストを描く | Draw text within a specified area
    m_mainFont(m_question).draw(45, Vec2{30, 35}, Palette::White);
    m_subFont(U"の画像をすべて選択してください").draw(25, Vec2{30, 110}, Palette::White);

    if (m_string.isEmpty()) {
      m_texture.draw(Arg::center(400, 390));
    } else {
      m_mainFont(m_string).draw(45, Arg::center(400, 390), Palette::Black);
    }

    // チェックマーク
    if (m_isSelected) {
      Circle{240, 230, 40}.draw(ColorF{0, 0.47, 0.78});
      Line{205, 216, 230, 250}.draw(6, Palette::White);
      Line{230, 250, 269, 207}.draw(6, Palette::White);
    }
  }

  inline void update() {
    if (KeyEnter.down()) {
      m_isSelected = true;
    }
  }

 private:
  bool m_isSelected = false;
};
