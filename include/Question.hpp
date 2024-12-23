#pragma once

#include <Siv3D.hpp>

struct Question {
  Question(const String& _question, bool _answer, const String& _string, double _stringSize)
      : question(_question),
        answer(_answer),
        string(_string),
        stringSize(_stringSize),
        mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}),
        subFont(Font{FontMethod::MSDF, 48}) {}
  Question(const String& _question, bool _answer, const Texture& _texture)
      : question(_question),
        answer(_answer),
        texture(_texture.resized(400)),
        mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}),
        subFont(Font{FontMethod::MSDF, 48}) {}

  const String question;
  const bool answer;
  const String string;
  const double stringSize = 0;
  const TextureRegion texture;
  const Font mainFont;
  const Font subFont;
  bool isSelected = false;
  int changeSize = 0;
  Timer timer{SecondsF{10}, StartImmediately::No};

  inline void draw() const {
    int textureSize = 1;
    // 青い四角を描く | Draw a rectangle
    Rect{5, 5, 789, 180}.draw(HSV{220, 0.8, 0.9});
    // 残り時間を描く | Draw the remaining time
    mainFont(timer.s()).draw(60, 670, 50, Palette::White);
    // 指定した範囲内にテキストを描く | Draw text within a specified area
    mainFont(question).draw(45, Vec2{30, 35}, Palette::White);
    subFont(U"の画像をすべて選択してください").draw(25, Vec2{30, 110}, Palette::White);

    if (string.isEmpty()) {
      texture.scaled(textureSize - (changeSize * 0.01)).draw(Arg::center(400, 390));
    } else {
      mainFont(string).draw(stringSize - changeSize, Arg::center(400, 390), Palette::Black);
    }

    // 選択した際の描画処理
    if (isSelected) {
      // グレーの四角の描画
      Point rectTopLeft;
      if (string.isEmpty()) {
        Rect rect{texture.scaled(textureSize - (changeSize * 0.01)).size.asPoint()};
        rect.setCenter(400, 390).draw(ColorF{0.5, 0.5, 0.5, 0.5});
        rectTopLeft = rect.tl();
      } else {
        const auto textRegion = mainFont(string).region(stringSize - changeSize);
        Rect rect{static_cast<int>(textRegion.w) + 7, static_cast<int>(textRegion.h) + 7};
        rect.setCenter(400, 390).draw(ColorF{0.5, 0.5, 0.5, 0.5});
        rectTopLeft = rect.tl();
      }
      // チェックマークの描画
      Point checkCenter{rectTopLeft.x, rectTopLeft.y};
      Circle{checkCenter, 50}.draw(ColorF{0, 0.47, 0.78});
      Line{checkCenter.movedBy(-55, -24), checkCenter.movedBy(-10, 20)}.draw(7, Palette::White);
      Line{checkCenter.movedBy(-10, 20), checkCenter.movedBy(60, -43)}.draw(7, Palette::White);
    }
  }

  inline void start() {
    timer.setRemaining(SecondsF{10});
    timer.start();
    isSelected = false;
  }

  inline void update() {
    if (KeyEnter.down()) {
      isSelected = true;
      changeSize = 10;
      if (0.7 < timer.sF())
        timer.setRemaining(SecondsF{0.7});
    }
  }

  inline bool isCorrect() {
    return answer == isSelected;
  }
};
