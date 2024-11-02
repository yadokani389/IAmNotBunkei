#pragma once

#include <Siv3D.hpp>

struct Question {
  Question(const String& _question, bool _answer, const String& _string, double _stringSize)
      : question(_question),
        answer(_answer),
        string(_string),
        stringSize(_stringSize),
        mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}),
        subFont(Font{FontMethod::MSDF, 48}),
        soloFont(Font{FontMethod::MSDF, 48, Typeface::Bold}),
        first(true) {}
  Question(const String& _question, bool _answer, const Texture& _texture)
      : question(_question),
        answer(_answer),
        texture(_texture.resized(400)),
        mainFont(Font{FontMethod::MSDF, 48, Typeface::Bold}),
        subFont(Font{FontMethod::MSDF, 48}),
        soloFont(Font{FontMethod::MSDF, 48, Typeface::Bold}),
        first(true) {}

  const String question;
  const bool answer;
  const String string;
  const double stringSize = 0;
  const TextureRegion texture;
  const Font mainFont;
  const Font subFont;
  const Font soloFont;
  bool isSelected = false;
  int changeSize = 0;
  RectF clickableArea;
  Timer timer{SecondsF{10}, StartImmediately::No};
  mutable bool first;
  

  inline void draw(const Vec2& center) const {
    double textureSize = 1.0 / 2.0;
    double soloStringSize = 32.0;

    // 青い四角を描く | Draw a rectangle

    // 残り時間を描く | Draw the remaining time
    // mainFont(timer.s()).draw(60, 670, 50, Palette::White);

    // 指定した範囲内にテキストを描く | Draw text within a specified area

    // 変えない

    if (center == Vec2(175, 300)) {
      Rect{5, 5, 789, 180}.draw(HSV{220, 0.8, 0.9});
      mainFont(question).draw(45, Vec2{30, 35}, Palette::White);
      subFont(U"の画像をすべて選択してください").draw(25, Vec2{30, 110}, Palette::White);
    }
    //

    if (string.isEmpty()) {
      texture.scaled(textureSize - (changeSize * 0.01)).draw(Arg::center(center));
    } else {
      // mainFont(string).draw(stringSize - changeSize, Arg::center(center), Palette::Black);
      soloFont(string).draw(soloStringSize - changeSize, Arg::center(center), Palette::Black);
    }

    // 選択した際の描画処理
    if (isSelected) {
      // グレーの四角の描画
      Point rectTopLeft;
      if (string.isEmpty()) {
        Rect rect{texture.scaled(textureSize - (changeSize * 0.01)).size.asPoint()};
        rect.setCenter(center.x, center.y).draw(ColorF{0.5, 0.5, 0.5, 0.5});
        rectTopLeft = rect.tl();
      } else {
        const auto textRegion = soloFont(string).region(soloStringSize - changeSize);
        Rect rect{static_cast<int>(textRegion.w) + 7, static_cast<int>(textRegion.h) + 7};
        rect.setCenter(center.x, center.y).draw(ColorF{0.5, 0.5, 0.5, 0.5});
        rectTopLeft = rect.tl();
      }
      // チェックマークの描画
      Point checkCenter{rectTopLeft.x, rectTopLeft.y};
      // Circle{checkCenter, 50}.draw(ColorF{0, 0.47, 0.78});
      // Line{checkCenter.movedBy(-55, -24), checkCenter.movedBy(-10, 20)}.draw(7, Palette::White);
      // Line{checkCenter.movedBy(-10, 20), checkCenter.movedBy(60, -43)}.draw(7, Palette::White);

      Circle{checkCenter, 25}.draw(ColorF{0, 0.47, 0.78});
      Line{checkCenter.movedBy(-27, -12), checkCenter.movedBy(-5, 10)}.draw(3.5, Palette::White);
      Line{checkCenter.movedBy(-5, 10), checkCenter.movedBy(30, -21)}.draw(3.5, Palette::White);
    }
  }

  inline void start() {
    // timer.setRemaining(SecondsF{10});
    // timer.start();
    isSelected = false;
  }

  inline void update(const Vec2& center) {
    if (MouseL.down()) {
      const Vec2 mousePos = Cursor::Pos();

      if (first) {
        if (string.isEmpty()) {
          clickableArea = texture.scaled(1.0 / 2.0 - (changeSize * 0.01)).region().setCenter(center.x, center.y);
        } else {
          const auto textRegion = soloFont(string).region(32.0 - changeSize);
          clickableArea = RectF(static_cast<int>(textRegion.w) + 20, static_cast<int>(textRegion.h) + 20).setCenter(center.x, center.y);
        }
        first = false;
      }

      if (clickableArea.leftClicked()) {
        isSelected = !isSelected;
        changeSize = isSelected ? 10 : 0;
        // if (0.7 < timer.sF())
        //     timer.setRemaining(SecondsF{0.7});
      }
    }
  }

  inline bool isCorrect() {
    return answer == isSelected;
  }
};
