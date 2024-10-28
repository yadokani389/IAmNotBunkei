#include <Siv3D.hpp>

#include "Question.hpp"

// 互いに素か判定
bool Coprime(int a, int b) {
  return Math::GCD(a, b) == 1;
}

// 互いに異なる2つの数字を出す a, bどちらかは奇数
std::pair<int, int> Rnd(int level) {
  int lim;
  lim = level * level * 200;
  int a = Random(2, lim);
  int b = Random(2, lim);
  int b = Random(2, lim);
  if (IsEven(a)) {
    while (a == b || IsEven(b)) {
      b = Random(3, 200);
    }
  } else {
    while (a == b) {
      b = Random(lim - 180, lim);
    }
  }

  return std::make_pair(a, b);

  return std::make_pair(a, b);
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

  int level = 1;  // レベルの変数

  const double SeventInterval = 1.0;
  double SaccumulatedTime = 0.0;

  int start = 0;
  // スタート画面
  while (System::Update()) {
    if (Key1.down()) {
      level = 1;
    } else if (Key2.down()) {
      level = 2;
    } else if (Key3.down()) {
      level = 3;
    } else if (Key4.down()) {
      level = 4;
    }

    if (level == 1)
      boldFont(U"EASY").draw(100, Vec2{50, 60}, ColorF{0.85, 0.6, 0.73});
    else if (level == 2)
      boldFont(U"NORMAL").draw(100, Vec2{50, 60}, ColorF{0.3, 0.56, 0.23});
    else if (level == 3)
      boldFont(U"HARD").draw(100, Vec2{50, 60}, ColorF{0.68, 0.26, 0.15});
    else if (level == 4)
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

  Array<Question> elementEasyQuestions = {
      Question{U"希ガス", true, U"He ヘリウム", 120},
      Question{U"希ガス", true, U"Ne ネオン", 120},
      Question{U"希ガス", true, U"Kr クリプトン", 120},
      Question{U"希ガス", true, U"Xe キセノン", 120},
      Question{U"希ガス", true, U"Rm ラドン", 120},
      Question{U"希ガス", true, U"Rm アルゴン", 120},

      Question{U"希ガス", false, U"H 水素", 120},
      Question{U"希ガス", false, U"Ti チタン", 120},
      Question{U"希ガス", true, U"W タングステン", 120},
      Question{U"希ガス", false, U"Li リチウム", 120},
      Question{U"希ガス", false, U"C 炭素", 120},
      Question{U"希ガス", false, U"N 窒素", 120},
      Question{U"希ガス", false, U"Na ナトリウム", 120},
      Question{U"希ガス", false, U"Ca カルシウム", 120},
      Question{U"希ガス", false, U"Al アルミニウム", 120},
  };
  Array<Question> elementNomalQuestions = {
      Question{U"アルカリ金属(１族)", true, U"Li リチウム", 120},
      Question{U"アルカリ金属(１族)", true, U"Na ナトリウム", 120},
      Question{U"アルカリ金属(１族)", true, U"K カリウム", 120},
      Question{U"アルカリ金属(１族)", true, U"Rb ルビジウム", 120},
      Question{U"アルカリ金属(１族)", true, U"Cs セシウム", 120},
      Question{U"アルカリ金属(１族)", true, U"Fr フランシウム", 120},

      Question{U"アルカリ金属(１族)", false, U"H 水素", 120},
      Question{U"アルカリ金属(１族)", false, U"Mg マグネシウム", 120},
      Question{U"アルカリ金属(１族)", false, U"Ba バリウム", 120},
      Question{U"アルカリ金属(１族)", false, U"Nb ニオブ", 120},
      Question{U"アルカリ金属(１族)", false, U"Po ポロニウム", 120},
      Question{U"アルカリ金属(１族)", false, U"Nh ニホニウム", 120},
      Question{U"アルカリ金属(１族)", false, U"Sb アンチモン", 120},
  };
  Array<Question> elementHardQuestions = {
      Question{U"ランタノイド元素", true, U"Ce セリウム", 120},
      Question{U"ランタノイド元素", true, U"Pr プラセオジム", 120},
      Question{U"ランタノイド元素", true, U"Nd ネオジム", 120},
      Question{U"ランタノイド元素", true, U"Pm プロメチウム", 120},
      Question{U"ランタノイド元素", true, U"Sm サマリウム", 120},
      Question{U"ランタノイド元素", true, U"Eu ユウロビウム", 120},
      Question{U"ランタノイド元素", true, U"Gd ガドリニウム", 120},
      Question{U"ランタノイド元素", true, U"Tb テルビウム", 120},
      Question{U"ランタノイド元素", true, U"Dy ジスプロニウム", 120},
      Question{U"ランタノイド元素", true, U"Ho ホルミウム", 120},
      Question{U"ランタノイド元素", true, U"Er エルビウム", 120},
      Question{U"ランタノイド元素", true, U"Er ツリウム", 120},
      Question{U"ランタノイド元素", true, U"Yb イッテルビウム", 120},
      Question{U"ランタノイド元素", true, U"Er ルテチウム", 120},

      Question{U"ランタノイド元素", false, U"Np ネプツニウム", 120},
      Question{U"ランタノイド元素", false, U"Am アメリシウム", 120},
      Question{U"ランタノイド元素", true, U"Cf カリホルニウム", 120},
      Question{U"ランタノイド元素", false, U"Fm フェルミウム", 120},
      Question{U"ランタノイド元素", false, U"Lr ローレンシウム", 120},
      Question{U"ランタノイド元素", false, U"Md メンデレビウム", 120},
      Question{U"ランタノイド元素", false, U"Lv リバボリウム", 120},
      Question{U"ランタノイド元素", false, U"Rf ラザホージウム", 120},
      Question{U"ランタノイド元素", false, U"Ds ダームスタチウム", 120},
  };
  Array<Question> primeEasyQuestions = {
    Question{U"素数", true, Texture{U"resources/assets/5.jpg"}},
    Question{U"素数", true, Texture{U"resources/assets/7.png"}},
    Question{U"素数", true, Texture{U"resources/assets/19.jpg"}},
    Question{U"素数", true, Texture{U"resources/assets/31.png"}},
    Question{U"素数", true, Texture{U"resources/assets/47.png"}},
    Question{U"素数", true, Texture{U"resources/assets/107.jpg"}},
    Question{U"素数", true, Texture{U"resources/assets/109.jpg"}},

    Question{U"素数", false, Texture{U"resources/assets/51(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/1(not).jpg"}},
  };
  Array<Question> primeNormalQuestions = {
    Question{U"素数", true, Texture{U"resources/assets/109.jpg"}},
    Question{U"素数", true, Texture{U"resources/assets/413.png"}},
    Question{U"素数", true, Texture{U"resources/assets/599.png"}},

    Question{U"素数", false, Texture{U"resources/assets/141(not).png"}},
    Question{U"素数", false, Texture{U"resources/assets/253(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/279(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/513(not).png"}},
  };
  Array<Question> primeHardQuestions = {
    Question{U"素数", true, Texture{U"resources/assets/599.png"}},
    Question{U"素数", true, Texture{U"resources/assets/6101.jpg"}},
    Question{U"素数", true, Texture{U"resources/assets/7103.jpg"}},
    Question{U"素数", true, Texture{U"resources/assets/8849.png"}},
    Question{U"素数", false, Texture{U"resources/assets/15739.jpg"}},

    Question{U"素数", false, Texture{U"resources/assets/513(not).png"}},
    Question{U"素数", false, Texture{U"resources/assets/4653(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/5201(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/7119(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/7203(not).jpg"}},
    Question{U"素数", false, Texture{U"resources/assets/8201(not).jpg"}},

  };

Question testQuestion{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/rust_logo.png"}};

auto [a, b] = Rnd(level);
Question coprimeQuestion{U"互いに素", Coprime(a, b), U"{} と {}"_fmt(a, b), 120};

Stopwatch stopwatch1{StartImmediately::Yes};
int32 gameTime = 60;

while(0.0 < gameTime - stopwatch1.s()) {
  // 残り時間（秒）
  int32 leftTime = 10;
  Stopwatch stopwatch2{StartImmediately::Yes};
  while (System::Update()) {
    elementEasyQuestions[0].draw();
    elementEasyQuestions[0].update();

    if (0.0 < leftTime - stopwatch2.s()) {
      boldFont(leftTime - stopwatch2.s()).draw(60, 670, 50, Palette::White);
    }
    ClearPrint();
    Print << Cursor::Pos().x << Cursor::Pos().y;
  }
}
}