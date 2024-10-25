#include <Siv3D.hpp>

bool SelectButton(const int centerx, const int centery, const Texture& texture, const bool& Selected) {
  const int width = texture.width();
  const int height = texture.height();
  Rect buttonRect(centerx - width / 2, centery - height / 2, width, height);
  bool isMouseOver = buttonRect.mouseOver();
  bool isClicked = buttonRect.leftClicked();

  texture.drawAt({centerx, centery});

  if (isClicked || Selected) {
    buttonRect.draw(ColorF{1.0, 1.0, 1.0, 0.5});
    Line{centerx - width / 2 + 8, centery - height / 2 + 18, centerx - width / 2 + 15, centery - height / 2 + 25}
        .draw(3, Palette::Lightgreen);
    Line{centerx - width / 2 + 15, centery - height / 2 + 25, centerx - width / 2 + 32, centery - height / 2 + 8}
        .draw(3, Palette::Lightgreen);
  }

  return isClicked || Selected;
}

void Main() {
  Scene::SetResizeMode(ResizeMode::Keep);
  Window::SetStyle(WindowStyle::Sizable);
  Window::Resize(640, 480);
  // 背景の色を設定する | Set the background color
  Scene::SetBackground(Palette::White);
  // 太文字のフォントを作成する | Create a bold font with MSDF method
  const Font boldFont{FontMethod::MSDF, 48, Typeface::Bold};
  const Font regularFont1{FontMethod::MSDF, 48};          // Typeface::Regular
  const Font regularFont2{48, Typeface::CJK_Regular_JP};  // Typeface::Regular
  // テキストに含まれる絵文字のためのフォントを作成し、font に追加する | Create a font for emojis in text and add it to font as a fallback
  const Font emojiFont{48, Typeface::ColorEmoji};
  boldFont.addFallback(emojiFont);

  int type = 1;  // レベルの変数

  const double SeventInterval = 1.0;
  double SaccumulatedTime = 0.0;

  int start = 0;
  // スタート画面
  while (System::Update()) {
    if (Key1.down()) {
      type = 1;
    } else if (Key2.down()) {
      type = 2;
    } else if (Key3.down()) {
      type = 3;
    } else if (Key4.down()) {
      type = 4;
    }

    if (type == 1)
      boldFont(U"EASY").draw(100, Vec2{50, 60}, ColorF{0.85, 0.6, 0.73});
    else if (type == 2)
      boldFont(U"NORMAL").draw(100, Vec2{50, 60}, ColorF{0.3, 0.56, 0.23});
    else if (type == 3)
      boldFont(U"HARD").draw(100, Vec2{50, 60}, ColorF{0.68, 0.26, 0.15});
    else if (type == 4)
      boldFont(U"INSANE").draw(100, Vec2{50, 60}, ColorF{0.36, 0.06, 0.45});

    // スタート用
    RoundRect{40, 240, 650, 200, 10}.draw(ColorF{0.97, 0.97, 0.97});
    RoundRect{40, 240, 650, 200, 10}.drawFrame(3, 0, ColorF{0.7, 0.7, 0.7});
    RoundRect{75, 290, 80, 80, 10}.draw(ColorF{0.99});
    RoundRect{75, 290, 80, 80, 10}.drawFrame(5, 0, ColorF{0.6});
    regularFont2(U"私は文系ではありま\nせん").draw(30, Vec2{180, 290}, Palette::Black);
    Circle{565, 315, 33}.drawArc(270_deg, 90_deg, 7, 13, ColorF{0.36, 0.57, 0.9});
    Circle{565, 315, 33}.drawArc(0_deg, 90_deg, 7, 13, ColorF{0.1, 0.2, 0.56});
    Circle{565, 315, 33}.drawArc(120_deg, 150_deg, 7, 13, ColorF{0.65});
    Triangle{519, 350, 519, 314, 553, 314}.draw(ColorF{0.65});
    Triangle{529, 269, 564, 269, 564, 304}.draw(ColorF{0.36, 0.57, 0.9});
    Triangle{576, 315, 611, 315, 611, 280}.draw(ColorF{0.1, 0.2, 0.56});
    regularFont2(U"reCAPTCHA").draw(25, Vec2{495, 365}, ColorF{0.2});
    regularFont2(U"プライバシー・利用規約").draw(17, Vec2{465, 400}, ColorF{0.4});

    if (KeyEnter.down()) {
      start = 1;
    }
    if (start) {
      Line{58, 285, 112, 345}.draw(10, Palette::Green);
      Line{210, 241, 112, 345}.draw(10, Palette::Green);
      SaccumulatedTime += Scene::DeltaTime();
      if (SeventInterval <= SaccumulatedTime)
        break;
    }
  }

  Stopwatch stopwatch1{StartImmediately::Yes};
  int32 gameTime = 60;

  // テスト用
  String path = U"";
  Image image{path};
  Texture texture{image};
  bool selected = false;
  // テスト用

  while (0.0 < gameTime - stopwatch1.s()) {
    // 残り時間（秒）
    int32 leftTime = 10;
    Stopwatch stopwatch2{StartImmediately::Yes};
    while (System::Update()) {
      // 青い四角を描く | Draw a rectangle
      Rect{5, 5, 789, 180}.draw(HSV{220, 0.8, 0.9});
      // 指定した範囲内にテキストを描く | Draw text within a specified area
      regularFont1(U"の画像をすべて選択してください").draw(25, Vec2{30, 110}, Palette::White);

      // チェックマーク
      Rect{200, 190, 400, 400}.draw(ColorF{0.7});
      Circle{240, 230, 40}.draw(ColorF{0, 0.47, 0.78});
      Line{205, 216, 230, 250}.draw(6, Palette::White);
      Line{230, 250, 269, 207}.draw(6, Palette::White);

      boldFont(U"プログラミング言語のロゴ").draw(45, Vec2{30, 35}, Palette::White);
      if (0.0 < leftTime - stopwatch2.s()) {
        boldFont(leftTime - stopwatch2.s()).draw(60, 670, 50, Palette::White);
      }

      selected = SelectButton(420, 320, texture, selected);
      ClearPrint();
      Print << Cursor::Pos().x << Cursor::Pos().y;
    }
  }
}
