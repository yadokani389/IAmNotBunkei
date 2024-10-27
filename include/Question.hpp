#pragma once

#include <Siv3D.hpp>

struct Question {
  Question(const String& question, bool answer, const String& string, double stringSize)
      : m_question(question), m_answer(answer), m_string(string), m_stringSize(stringSize), m_mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}), m_subFont(Font{FontMethod::MSDF, 48}) {}
  Question(const String& question, bool answer, const Texture& texture)
      : m_question(question), m_answer(answer), m_texture(texture), m_mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}), m_subFont(Font{FontMethod::MSDF, 48}) {}

  const String m_question;
  const bool m_answer;
  const String m_string;
  const double m_stringSize = 0;
  const Texture m_texture;
  const Font m_mainFont;
  const Font m_subFont;
  bool m_isSelected = false;
  Timer m_timer{SecondsF{10}, StartImmediately::No};

  inline void draw() const {
    // 青い四角を描く | Draw a rectangle
    Rect{5, 5, 789, 180}.draw(HSV{220, 0.8, 0.9});
    // 残り時間を描く | Draw the remaining time
    Rect{5, 5, static_cast<int>(789 * m_timer.progress1_0()), 10}.draw(Palette::Greenyellow);
    m_mainFont(m_timer.s()).draw(60, 670, 50, Palette::White);
    // 指定した範囲内にテキストを描く | Draw text within a specified area
    m_mainFont(m_question).draw(45, Vec2{30, 35}, Palette::White);
    m_subFont(U"の画像をすべて選択してください").draw(25, Vec2{30, 110}, Palette::White);

    if (m_string.isEmpty()) {
      m_texture.draw(Arg::center(400, 390));
    } else {
      m_mainFont(m_string).draw(m_stringSize, Arg::center(400, 390), Palette::Black);
    }

    // 選択した際の描画処理
    if (m_isSelected) {
      // グレーの四角の描画
      Point rectTopLeft;
      if (m_string.isEmpty()) {
        const Size textureSize = m_texture.size();
        Rect rect{textureSize};
        rect.setCenter(400, 390).draw(ColorF{0.5, 0.5, 0.5, 0.5});
        rectTopLeft = rect.tl();
      } else {
        const auto textRegion = m_mainFont(m_string).region(m_stringSize);
        Rect rect{static_cast<int>(textRegion.w) + 20, static_cast<int>(textRegion.h) + 20};
        rect.setCenter(400, 390).draw(ColorF{0.5, 0.5, 0.5, 0.5});
        rectTopLeft = rect.tl();
      }
      // チェックマークの描画
      Point checkCenter{rectTopLeft.x, rectTopLeft.y};
      Circle{checkCenter, 50}.draw(ColorF{0, 0.47, 0.78});
      Line{checkCenter.movedBy(-35, -24), checkCenter.movedBy(-10, 20)}.draw(7, Palette::White);
      Line{checkCenter.movedBy(-10, 20), checkCenter.movedBy(29, -43)}.draw(7, Palette::White);
    }
  }

  inline void start() {
    m_timer.setRemaining(SecondsF{10});
    m_timer.start();
  }

  inline void update() {
    if (KeyEnter.down()) {
      m_isSelected = true;
      if (0.5 < m_timer.sF())
        m_timer.setRemaining(SecondsF{0.5});
    }
  }

  inline bool isCorrect() {
    return m_answer == m_isSelected;
  }
};