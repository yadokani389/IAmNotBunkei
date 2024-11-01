#include <Siv3D.hpp>

#include "Connection.hpp"
#include "Question.hpp"

// 互いに素か判定
bool Coprime(int a, int b) {
  return Math::GCD(a, b) == 1;
}

// 互いに異なる2つの数字を出す a, bどちらかは奇数
std::pair<int, int> Rnd(int level) {
  int lim;
  if (level == 3) {
    lim = 9999;
  } else {
    lim = (level + level + 1) * 150;
  }
  int a = Random(2, lim);
  int b = Random(3, lim);
  if (IsEven(a)) {
    while (a == b || IsEven(b)) {
      b = Random(lim - 180, lim);
    }
  } else {
    while (a == b) {
      b = Random(lim - 180, lim);
    }
  }
  return std::make_pair(a, b);
}

void MakeCoprimeQuestions(Array<Array<Array<Question>>>& questions) {
  Array<Array<Question>> coprimeQuestions{4};
  for (size_t i = 0; i < 4; i++) {
    size_t j = 0;
    while (j < 10) {
      auto [a, b] = Rnd(i);
      if (!Coprime(a, b))
        continue;
      coprimeQuestions[i].push_back(Question{U"互いに素", Coprime(a, b), U"{} と {}"_fmt(a, b), 120, SecondsF{10}});
      j++;
    }

    j = 0;
    while (j < 10) {
      auto [a, b] = Rnd(i);
      if (Coprime(a, b))
        continue;
      coprimeQuestions[i].push_back(Question{U"互いに素", Coprime(a, b), U"{} と {}"_fmt(a, b), 120, SecondsF{10}});
      j++;
    }
  }
  questions.push_back(coprimeQuestions);
}

std::pair<String, Color> GetLevelInfo(int level) {
  if (level == 0)
    return std::make_pair(U"EASY", ColorF{0.85, 0.6, 0.73});
  else if (level == 1)
    return std::make_pair(U"NORMAL", ColorF{0.3, 0.56, 0.23});
  else if (level == 2)
    return std::make_pair(U"HARD", ColorF{0.68, 0.26, 0.15});
  else
    return std::make_pair(U"INSANE", ColorF{0.36, 0.06, 0.45});
}

// Button
bool Button(double x, double y, double width, double height, const Font& font, const String& txt) {
  RectF rect{Arg::center(x, y), width, height};
  rect.rounded(15).draw(ColorF{0.25, 0.5, 0.8, 0.8});
  rect.rounded(15).drawFrame(2, 0, Palette::White);

  RectF textRect = font(txt).region();
  double scaleFactor = Min((width * 0.8) / textRect.w, (height * 0.8) / textRect.h);
  int fontSize = static_cast<int>(font.fontSize() * scaleFactor);

  font(txt).drawAt(fontSize, x, y, Palette::White);

  return rect.leftClicked();
}

void StartMenu(int& level, const Font& boldFont, const Font& regularFont2, Server& server) {
  bool start = false;
  Timer timer{Seconds{1}};
  while (System::Update()) {
    server.update();
    if (Key1.down()) {
      level = 0;
    } else if (Key2.down()) {
      level = 1;
    } else if (Key3.down()) {
      level = 2;
    } else if (Key4.down()) {
      level = 3;
    }

    boldFont(GetLevelInfo(level).first).draw(100, Vec2{50, 60}, GetLevelInfo(level).second);

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
      start = true;
      timer.start();
    }

    // スタート後のアニメーション
    if (start) {
      Line{58, 285, 112, 345}.draw(10, Palette::Green);
      Line{210, 241, 112, 345}.draw(10, Palette::Green);
      if (timer.reachedZero())
        break;
    }
  }
}

bool LoadingMenu(Server& server, const Font& largeFont, const Font& smallFont) {
  const auto ipAddresses = Network::EnumerateIPv4Addresses();
  IPv4Address myIp;

  for (const auto& ip : ipAddresses) {
    if (ip != IPv4Address::Localhost()) {
      myIp = ip;
    }
  }
  while (System::Update()) {
    server.update();

    largeFont(Format(U"参加台数: ", server.sessionIds.size())).drawAt(Scene::Center(), Palette::Darkgray);
    smallFont(Format(U"IP: "), myIp.str()).drawAt(Vec2(Scene::Center().x, Scene::Center().y + 100), Palette::Darkgray);
    smallFont(U"Press Space To Start").drawAt(Vec2(Scene::Center().x, Scene::Center().y + 200), Palette::Darkgray);

    if (KeySpace.pressed()) return true;

    if (Button(Scene::Width() - 100, 50, 100, 35, smallFont, U"Cancel")) return false;

    if (KeyW.down())
      Window::SetFullscreen(true);
  }

  return false;
}
struct ScoreEffect : IEffect {
  Vec2 m_start;

  int32 m_score;

  Font m_font;

  ScoreEffect(const Vec2& start, int32 score, const Font& font)
      : m_start{start}, m_score{score}, m_font{font} {}

  bool update(double t) override {
    Color color;
    String txt = Format(m_score);
    if (m_score < 0) {
      color = HSV(Random(0.0, 360.0), Random(0.3, 0.6), 0.5).toColor();
    } else {
      txt = U"+" + Format(m_score);
      color = HSV(Random(0.0, 360.0), Random(0.6, 1.0), 0.8).toColor();
    }
    m_font(txt).drawAt(m_start.movedBy(0, t * -120), color);
    return (t < 0.5);
  }
};

struct SelectEffect : IEffect {
  struct Bubble {
    Vec2 offset;
    double startTime;
    double scale;
    ColorF color;
    bool correct;
  };
  Vec2 pos;

  Array<Bubble> bubbles;

  SelectEffect(const Vec2& pos, bool correct)
      : pos{pos} {
    for (int32 i = 0; i < 20; ++i) {
      Bubble bubble{
          .offset = RandomVec2(Circle{30}),
          .startTime = Random(-0.3, 0.3),
          .scale = Random(0.2, 2.5),
          .correct = correct,
      };
      bubbles << bubble;
    }
  }

  bool update(double t) override {
    for (const auto& bubble : bubbles) {
      const double t2 = (bubble.startTime + t);

      if (not InRange(t2, 0.0, 1.0)) {
        continue;
      }

      Color color = HSV(Random(0.0, 360.0), Random(0.3, 0.6), 1.0).toColor();

      const double e = EaseOutExpo(t2);

      if (bubble.correct) {
        Circle{(pos + bubble.offset + (bubble.offset * 10 * t)), (e * 40 * bubble.scale)}.draw(ColorF{color, 0.15}).drawFrame((30.0 * (1.0 - e) * bubble.scale), color);

      } else {
        Vec2 center = pos + bubble.offset + (bubble.offset * 10 * t);

        double size = e * 40 * bubble.scale;

        double thickness = 60.0 * (1.0 - e) * bubble.scale;

        Line{center - Vec2(size, size), center + Vec2(size, size)}
            .draw(thickness, color);
        Line{center + Vec2(size, -size), center + Vec2(-size, size)}
            .draw(thickness, color);
      }
    }

    return (t < 1.3);
  }
};

void Main() {
  Scene::SetResizeMode(ResizeMode::Keep);
  Window::SetStyle(WindowStyle::Sizable);
  Window::Resize(640, 480);
  // 背景の色を設定する | Set the background color
  Scene::SetBackground(Palette::White);
  // 太文字のフォントを作成する | Create a bold font with MSDF method
  const Font boldFont{FontMethod::MSDF, 48, Typeface::Bold};
  const Font smallBoldFont{FontMethod::MSDF, 24, Typeface::Bold};
  const Font regularFont1{FontMethod::MSDF, 48};          // Typeface::Regular
  const Font regularFont2{48, Typeface::CJK_Regular_JP};  // Typeface::Regular
  // テキストに含まれる絵文字のためのフォントを作成し、font に追加する | Create a font for emojis in text and add it to font as a fallback
  const Font emojiFont{48, Typeface::ColorEmoji};
  boldFont.addFallback(emojiFont);
  // Effect関係
  Effect effect;
  const Font effectFont{FontMethod::MSDF, 96, Typeface::Bold};

  int level = 0;  // レベルの変数
  // 通信関係

  Server server;

  // ここにIPアドレスとポート番号を入力
  IPv4Address serverAddress;
  uint16 port = 80;

  while (System::Update()) {
    // hostとしてスタート
    const Vec2 center = Scene::Center();
    if (Button(center.x, center.y - 50, 200, 75, regularFont1, U"Be Host")) {
      TextEditState inputPort;

      while (System::Update()) {
        SimpleGUI::TextBox(inputPort, Vec2(Scene::Center().x - 150, Scene::Center().y - 23.5), 300);
        if (Button(Scene::Center().x, Scene::Center().y + 100, 200, 75, smallBoldFont, U"Start")) {
          port = Parse<uint16>(inputPort.text);
          break;
        }
      }

      server.startServer(port);

      if (LoadingMenu(server, boldFont, smallBoldFont))
        break;
      else
        continue;
    }

    // clientとしてスタート
    if (Button(center.x, center.y + 50, 200, 75, regularFont1, U"Be Client")) {
      TextEditState inputAddress;
      TextEditState inputPort;

      while (System::Update()) {
        SimpleGUI::TextBox(inputAddress, Vec2(Scene::Center().x - 150, Scene::Center().y - 23.5), 300);
        SimpleGUI::TextBox(inputPort, Vec2(Scene::Center().x - 150, Scene::Center().y + 23.5), 300);
        if (Button(Scene::Center().x, Scene::Center().y + 150, 200, 75, smallBoldFont, U"接続")) {
          serverAddress = IPv4Address(inputAddress.text);
          port = Parse<uint16>(inputPort.text);
          break;
        }
      }

      bool done = false;
      while (System::Update()) {
        if (server.connectToServer(serverAddress, port)) {
          done = true;
          break;
        }
        if (Button(Scene::Center().x, Scene::Center().y + 100, 200, 75, smallBoldFont, U"キャンセル")) {
          done = false;
          break;
        }
        boldFont(U"Connecting...").drawAt(Scene::Center(), Palette::Darkgray);
        System::Sleep(10);
      }
      if (done)
        break;
    }
  }

  // Audio関係
  const Audio CorrectSound = Audio(U"resources/sounds/Quiz-Correct_Answer01-1.mp3");
  const Audio WrongSound = Audio(U"resources/sounds/Quiz-Wrong_Buzzer02-2.mp3");
  // for mac
  // const Audio CorrectSound = Audio(U"engine/resources/sounds/Quiz-Correct_Answer01-1.mp3");
  // const Audio WrongSound = Audio(U"engine/resources/sounds/Quiz-Wrong_Buzzer02-2.mp3");

  Array<Array<Array<Question>>> questions = {
      {
          // element
          {
              // element easy
              Question{U"存在する元素", true, U"Se セレン", 120},
              Question{U"存在する元素", true, U"P リン", 120},
              Question{U"存在する元素", true, U"Xe キセノン", 120},
              Question{U"存在する元素", true, U"Ne ネオン", 120},
              Question{U"存在する元素", true, U"Nb ニオブ", 120},
              Question{U"存在する元素", true, U"Nh ニホニウム", 110},
              Question{U"存在する元素", true, U"Cf カリホルニウム", 85},
              Question{U"存在する元素", true, U"Pr プラセオジム", 100},

              Question{U"存在する元素", false, U"Sr セミリウム", 120},
              Question{U"存在する元素", false, U"Cn カレニウム", 120},
              Question{U"存在する元素", false, U"Gs ガリレシウム", 100},
              Question{U"存在する元素", false, U"Ic アイスクリウム", 90},
              Question{U"存在する元素", false, U"Kn カニウム", 120},
              Question{U"存在する元素", false, U"M マメジウム", 110},
          },
          {
              // element normal
              Question{U"希ガス", true, U"He ヘリウム", 120},
              Question{U"希ガス", true, U"Ne ネオン", 120},
              Question{U"希ガス", true, U"Kr クリプトン", 110},
              Question{U"希ガス", true, U"Xe キセノン", 120},
              Question{U"希ガス", true, U"Rm ラドン", 120},
              Question{U"希ガス", true, U"Ar アルゴン", 120},

              Question{U"希ガス", false, U"H 水素", 120},
              Question{U"希ガス", false, U"Ti チタン", 120},
              Question{U"希ガス", false, U"W タングステン", 100},
              Question{U"希ガス", false, U"Li リチウム", 120},
              Question{U"希ガス", false, U"C 炭素", 120},
              Question{U"希ガス", false, U"N 窒素", 120},
              Question{U"希ガス", false, U"Na ナトリウム", 110},
              Question{U"希ガス", false, U"Ca カルシウム", 110},
              Question{U"希ガス", false, U"Al アルミニウム", 100},
          },
          {
              // element hard
              Question{U"アルカリ金属(１族)", true, U"Li リチウム", 120},
              Question{U"アルカリ金属(１族)", true, U"Na ナトリウム", 110},
              Question{U"アルカリ金属(１族)", true, U"K カリウム", 120},
              Question{U"アルカリ金属(１族)", true, U"Rb ルビジウム", 110},
              Question{U"アルカリ金属(１族)", true, U"Cs セシウム", 120},
              Question{U"アルカリ金属(１族)", true, U"Fr フランシウム", 100},

              Question{U"アルカリ金属(１族)", false, U"H 水素", 120},
              Question{U"アルカリ金属(１族)", false, U"Mg マグネシウム", 100},
              Question{U"アルカリ金属(１族)", false, U"Ba バリウム", 120},
              Question{U"アルカリ金属(１族)", false, U"Nb ニオブ", 120},
              Question{U"アルカリ金属(１族)", false, U"Po ポロニウム", 110},
              Question{U"アルカリ金属(１族)", false, U"Nh ニホニウム", 110},
              Question{U"アルカリ金属(１族)", false, U"Sb アンチモン", 110},
              Question{U"アルカリ金属(１族)", false, U"Kr クリプトン", 120},
              Question{U"アルカリ金属(１族)", false, U"Fe 鉄", 120},
              Question{U"アルカリ金属(１族)", false, U"Ti チタン", 120},
              Question{U"アルカリ金属(１族)", false, U"Os オスミウム", 120},
              Question{U"アルカリ金属(１族)", false, U"W タングステン", 120},
          },
          {
              // element insane
              Question{U"ランタノイド元素", true, U"Ce セリウム", 120},
              Question{U"ランタノイド元素", true, U"Pr プラセオジム", 100},
              Question{U"ランタノイド元素", true, U"Nd ネオジム", 120},
              Question{U"ランタノイド元素", true, U"Pm プロメチウム", 100},
              Question{U"ランタノイド元素", true, U"Sm サマリウム", 110},
              Question{U"ランタノイド元素", true, U"Eu ユウロビウム", 100},
              Question{U"ランタノイド元素", true, U"Gd ガドリニウム", 100},
              Question{U"ランタノイド元素", true, U"Tb テルビウム", 110},
              Question{U"ランタノイド元素", true, U"Dy ジスプロニウム", 85},
              Question{U"ランタノイド元素", true, U"Ho ホルミウム", 110},
              Question{U"ランタノイド元素", true, U"Er エルビウム", 110},
              Question{U"ランタノイド元素", true, U"Tm ツリウム", 120},
              Question{U"ランタノイド元素", true, U"Yb イッテルビウム", 85},
              Question{U"ランタノイド元素", true, U"Lu ルテチウム", 110},

              Question{U"ランタノイド元素", false, U"Np ネプツニウム", 100},
              Question{U"ランタノイド元素", false, U"Am アメリシウム", 100},
              Question{U"ランタノイド元素", false, U"Cf カリホルニウム", 85},
              Question{U"ランタノイド元素", false, U"Fm フェルミウム", 100},
              Question{U"ランタノイド元素", false, U"Lr ローレンシウム", 75},
              Question{U"ランタノイド元素", false, U"Md メンデレビウム", 85},
              Question{U"ランタノイド元素", false, U"Lv リバモリウム", 100},
              Question{U"ランタノイド元素", false, U"Rf ラザホージウム", 85},
              Question{U"ランタノイド元素", false, U"Ds ダームスタチウム", 85},
          },
      },
      {
          // prime
          {
              // prime easy
              Question{U"素数", true, Texture{U"resources/assets/5.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/7.png"}},
              Question{U"素数", true, Texture{U"resources/assets/19.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/31.png"}},
              Question{U"素数", true, Texture{U"resources/assets/47.png"}},
              Question{U"素数", true, Texture{U"resources/assets/107.jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/51(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/1(not).jpg"}},
          },
          {
              // prime normal
              Question{U"素数", true, Texture{U"resources/assets/19.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/31.png"}},
              Question{U"素数", true, Texture{U"resources/assets/47.png"}},
              Question{U"素数", true, Texture{U"resources/assets/109.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/107.jpg"}},

              Question{U"素数", false, Texture{U"resources/assets/51(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/1(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/141(not).png"}},
              Question{U"素数", false, Texture{U"resources/assets/253(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/279(not).jpg"}},
          },
          {
              // prime hard
              Question{U"素数", true, Texture{U"resources/assets/109.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/107.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/413.png"}},
              Question{U"素数", true, Texture{U"resources/assets/599.png"}},

              Question{U"素数", false, Texture{U"resources/assets/141(not).png"}},
              Question{U"素数", false, Texture{U"resources/assets/253(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/279(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/513(not).png"}},
          },
          {
              // prime insane
              Question{U"素数", true, Texture{U"resources/assets/599.png"}},
              Question{U"素数", true, Texture{U"resources/assets/6101.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/7103.jpg"}},
              Question{U"素数", true, Texture{U"resources/assets/8849.png"}},
              Question{U"素数", true, Texture{U"resources/assets/17539.jpg"}},

              Question{U"素数", false, Texture{U"resources/assets/513(not).png"}},
              Question{U"素数", false, Texture{U"resources/assets/4653(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/5201(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/7119(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/7203(not).jpg"}},
              Question{U"素数", false, Texture{U"resources/assets/8201(not).jpg"}},
          },
      },
      {
          // programming language
          {
              // programming language easy
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/java.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Python-Emblem.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/c-gengo.png"}},

              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Apex.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/EA.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Valorant.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/OW.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Norton.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Carp.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/itoyokado.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/LarryBird.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Linux.png"}},
          },
          {
              // programing language normal
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/java.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Python-Emblem.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/c-gengo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/cpp.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Csharp.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/JavaScript.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Ruby.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Rust.png"}},

              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Linux.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Notion.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Roblox.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/RustGame.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Winzip.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/carp-2.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Carp.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/chuo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Reds.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/KC.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/LS.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/SD.png"}},
          },
          {
              // programing language hard
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/JavaScript.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Ruby.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Rust.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/go.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Perl.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/TypeScript.png"}},

              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Linux.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Entraid.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Firebase.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Linux.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Notion.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/plumbob.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Slack.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Ubuntu.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Winrar.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/Winzip.png"}},
          },
          {
              // programing language insane
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Rust.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/go.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/Perl.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/progenLogo/TypeScript.png"}},

              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/progenLogo/plumbob.png"}},

          },
      },
      {
          // calculation
          {
              // calculation easy
              Question{U"1以上", true, Texture{U"resources/assets/E2.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/E5.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/E6.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/E7.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/E9.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/E11.png"}},

              Question{U"1以上", false, Texture{U"resources/assets/E1(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/E3(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/E4(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/E8(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/E10(not).png"}},
          },
          {
              // calculation normal
              Question{U"1以上", true, Texture{U"resources/assets/N1.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/N3.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/N4.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/N5.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/N9.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/N10.png"}, SecondsF{10}},

              Question{U"1以上", false, Texture{U"resources/assets/N2(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/N6(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/N7(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/N8(not).png"}, SecondsF{10}},
          },
          {
              // calculation hard
              Question{U"1以上", true, Texture{U"resources/assets/H1.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/H5.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/H6.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/H7.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/H9.png"}, SecondsF{10}},
              Question{U"1以上", true, Texture{U"resources/assets/H11.png"}, SecondsF{10}},

              Question{U"1以上", false, Texture{U"resources/assets/H2(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/H3(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/H4(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/H8(not).png"}, SecondsF{10}},
              Question{U"1以上", false, Texture{U"resources/assets/H10(not).png"}, SecondsF{10}},
          },
          {
              // calculation insane
              Question{U"未解決問題", true, U"ビール予想", 100},
              Question{U"式を満たすpが,10以下", true, Texture{U"resources/assets/I5.png"}},
              Question{U"解がe⁶", true, Texture{U"resources/assets/I6.png"}},
              Question{U"式を満たす最小の自然数mが,m>10", true, U"m≡4(mod5)かつm≡1(mod2)", 50},
              Question{U"既に証明済み", true, U"素数は無限個存在する", 60},
              Question{U"近似値が1以上", false, Texture{U"resources/assets/I9.png"}},

              Question{U"未解決問題", false, U"円積問題", 100},
              Question{U"解がe", false, Texture{U"resources/assets/I8(not).png"}},
              Question{U"既に証明済み", false, U"双子素数は無限個存在する", 60},
              Question{U"式を満たす実数Mが,M>1700", false, U"904³¹⁹ ≡ M(mod2627)", 60},
              Question{U"既に証明済み", false, Texture{U"resources/assets/I7(not).png"}},
          },
      }};

  MakeCoprimeQuestions(questions);

  Array<Array<Array<size_t>>> indexes;
  for (const auto& q : questions) {
    Array<Array<size_t>> temp{q.size()};
    for (size_t i = 0; i < q.size(); i++) {
      temp[i] = Array<size_t>(q[i].size());
      std::iota(temp[i].begin(), temp[i].end(), 0);
    }
    indexes.push_back(temp);
  }
  Array<size_t> categoryIndexes(questions.size());
  std::iota(categoryIndexes.begin(), categoryIndexes.end(), 0);

  while (System::Update()) {
    // 問題の生成
    questions.pop_back();
    MakeCoprimeQuestions(questions);

    // 問題をシャッフル
    for (auto& index : indexes)
      for (auto& i : index)
        Shuffle(i.begin(), i.end());
    Shuffle(categoryIndexes.begin(), categoryIndexes.end());
    Console << categoryIndexes;

    // スタート画面
    if (server.isHost) {
      StartMenu(level, boldFont, regularFont2, server);
    }

    if (server.isHost) {
      server.sendStart(level, indexes, categoryIndexes);
    } else {
      while (System::Update()) {
        boldFont(U"Waiting for game start...").drawAt(Scene::Center(), Palette::Darkgray);
        if (server.receiveStart(level, indexes, categoryIndexes)) {
          Console << U"Game start received!";
          break;
        }

        server.update();
        System::Sleep(10);

        if (KeyW.down())
          Window::SetFullscreen(true);
      }
    }

    Timer gameTimer{Seconds{90}, StartImmediately::Yes};

    // ゲーム画面
    ssize_t point = 0;
    ssize_t deltaPoint = 0;
    size_t category = 0;
    const int32 categoryUpdate = gameTimer.s() / categoryIndexes.size();
    int32 nextCategoryUpdate = gameTimer.s() - categoryUpdate;
    bool shouldQuit = false;
    size_t endSession = 0;
    while (System::Update() && !gameTimer.reachedZero() && category < categoryIndexes.size()) {
      if (gameTimer.s() < nextCategoryUpdate) {
        category++;
        nextCategoryUpdate -= categoryUpdate;
        continue;
      }
      size_t nowCategory = categoryIndexes[category];
      if (indexes[nowCategory].size() <= level || indexes[nowCategory][level].empty()) {
        category++;
        nextCategoryUpdate -= categoryUpdate;
        continue;
      }

      size_t nowIndex = 0;
      if (server.isHost) {
        nowIndex = indexes[nowCategory][level].front();
        indexes[nowCategory][level].pop_front();
      } else {
        nowIndex = indexes[nowCategory][level].back();
        indexes[nowCategory][level].pop_back();
      }
      server.sendPop(nowCategory);
      Console << nowCategory << U" " << level << U" " << nowIndex << U" " << point;
      auto& question = questions[nowCategory][level][nowIndex];

      question.start();
      do {
        question.draw();
        question.update();
        // ゲームタイマーの残りを描画
        Rect{5, 5, static_cast<int>(789 * gameTimer.progress1_0()), 10}.draw(Palette::Greenyellow);
        effect.update();
        if (question.timer.reachedZero())
          break;

        if (KeyQ.down()) {
          shouldQuit = true;
          break;
        }

        {
          auto counts = server.receivePointAndPop(point, endSession);
          for (auto& [id, count] : counts) {
            while (count != 0 && !indexes[id][level].empty()) {
              if (server.isHost)
                indexes[id][level].pop_back();
              else
                indexes[id][level].pop_front();
              count--;
            }
          }
          if (indexes[nowCategory][level].empty()) {
            category++;
            nowCategory = categoryIndexes[category];
            nextCategoryUpdate -= categoryUpdate;
            continue;
          }
        }

        ClearPrint();
        Print << point;
      } while (System::Update());

      if (shouldQuit)
        break;

      if (question.isCorrect()) {
        effect.add<SelectEffect>(Scene::Center(), true);
        CorrectSound.playOneShot();
        if (question.isSelected)
          deltaPoint = 10;
        else
          deltaPoint = 15;
      } else {
        effect.add<SelectEffect>(Scene::Center(), false);
        WrongSound.playOneShot();
        if (question.isSelected)
          deltaPoint = -7;
        else
          deltaPoint = -12;
      }

      point += deltaPoint;
      server.sendPoint(deltaPoint);
      effect.add<ScoreEffect>(Scene::Center(), deltaPoint, effectFont);
      Console << U"次の問題へ";
    }
    if (shouldQuit)
      continue;

    if (server.isHost) {
      while (endSession < server.sessionIds.size()) {
        server.update();
        System::Sleep(10);
        server.receivePointAndPop(point, endSession);
      }
    } else {
      server.update();
      server.sendEnd();
    }

    Console << U"リザルト画面";
    // リザルト画面
    while (System::Update()) {
      if (KeySpace.down())
        break;

      if (server.isHost) {
        server.update();
        String rankText = U"";

        if (point < 50)
          rankText = U"文系？";
        else if (point < 100)
          rankText = U"ちょっと理系";
        else if (point < 150)
          rankText = U"まぁまぁ理系";
        else if (point < 200)
          rankText = U"理系";
        else if (point < 250)
          rankText = U"理系 OF 理系";
        else
          rankText = U"伝説の理系";

        auto xAdvance = static_cast<int>(boldFont(U"難易度:  ").getXAdvances(50).sum());
        boldFont(U"難易度:  ").draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4), Palette::Black);
        boldFont(GetLevelInfo(level).first).draw(50, Arg::leftCenter(Scene::Height() / 4 + xAdvance, Scene::Height() / 4), GetLevelInfo(level).second);
        boldFont(U"スコア:  ", point).draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4 * 2), Palette::Black);
        boldFont(U"ランク:  ", rankText).draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4 * 3), Palette::Black);
      } else {
        boldFont(U"Thanks for playing").draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4 * 2), Palette::Black);
      }
    }
    server.clear();
  }
}
