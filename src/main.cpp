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
}

std::pair<String, Color> GetLevelInfo(int level) {
  if (level == 1)
    return std::make_pair(U"EASY", ColorF{0.85, 0.6, 0.73});
  else if (level == 2)
    return std::make_pair(U"NORMAL", ColorF{0.3, 0.56, 0.23});
  else if (level == 3)
    return std::make_pair(U"HARD", ColorF{0.68, 0.26, 0.15});
  else
    return std::make_pair(U"INSANE", ColorF{0.36, 0.06, 0.45});
}

void StartMenu(int& level, const Font& boldFont, const Font& regularFont2) {
  bool start = false;
  Timer timer{Seconds{1}};
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

  // スタート画面
  StartMenu(level, boldFont, regularFont2);

  // Audio関係
  const Audio CorrectSound = Audio(U"resources/sounds/Quiz-Correct_Answer01-1.mp3");
  const Audio WrongSound = Audio(U"resources/sounds/Quiz-Wrong_Buzzer02-2.mp3");
  // for mac
  // const Audio CorrectSound = Audio(U"engine/resources/sounds/Quiz-Correct_Answer01-1.mp3");
  // const Audio WrongSound = Audio(U"engine/resources/sounds/Quiz-Wrong_Buzzer02-2.mp3");

  Array<Array<Array<Question> > > questions = {
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
              Question{U"存在する元素", true, U"Cf カリホルニウム", 90},
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
          // programing
          {
              // programing language easy
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/rust-logo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/python-logo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/ruby-logo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/julia-logo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/java-logo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/bash-logo.png"}},
              Question{U"プログラミング言語のロゴ", true, Texture{U"resources/assets/c-logo.png"}},

              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/firebase-logo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/scratch-logo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/ubuntu-logo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/v-logo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/vsc-logo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/roblox-logo.png"}},
              Question{U"プログラミング言語のロゴ", false, Texture{U"resources/assets/overwatch-logo.png"}},
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
              Question{U"1以上", true, Texture{U"resources/assets/N1.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/N3.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/N4.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/N5.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/N9.png"}},
              Question{U"1以上", true, Texture{U"resources/assets/N10.png"}},

              Question{U"1以上", false, Texture{U"resources/assets/N2(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/N6(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/N7(not).png"}},
              Question{U"1以上", false, Texture{U"resources/assets/N8(not).png"}},
          },
      }};

  {
    Array<Array<Question> > coprimeQuestions{4};
    for (size_t i = 0; i < 4; i++) {
      auto [a, b] = Rnd(level);
      coprimeQuestions[i].push_back(Question{U"互いに素", Coprime(a, b), U"{} と {}"_fmt(a, b), 120});
    }
    questions.push_back(coprimeQuestions);
  }

  Array<Array<std::deque<size_t> > > indexes;
  for (const auto& q : questions) {
    Array<std::deque<size_t> > temp{q.size()};
    for (size_t i = 0; i < q.size(); i++) {
      temp[i] = std::deque<size_t>(q[i].size());
      std::iota(temp[i].begin(), temp[i].end(), 0);
      Shuffle(temp[i].begin(), temp[i].end());
    }
    indexes.push_back(temp);
  }
  Array<size_t> categoryIndexes(questions.size());
  std::iota(categoryIndexes.begin(), categoryIndexes.end(), 0);
  Shuffle(categoryIndexes.begin(), categoryIndexes.end());
  Console << categoryIndexes;

  Timer gameTimer{Seconds{90}, StartImmediately::Yes};

  // ゲーム画面
  int32 point = 0;
  size_t category = 0;
  int32 categoryUpdate = gameTimer.s() / questions.size();
  int32 nextCategoryUpdate = gameTimer.s() - categoryUpdate;
  while (!gameTimer.reachedZero() && category < questions.size()) {
    if (gameTimer.s() < nextCategoryUpdate) {
      category++;
      nextCategoryUpdate -= categoryUpdate;
    }
    size_t nowCategory = categoryIndexes[category];
    if (indexes[nowCategory].size() <= level - 1 || indexes[nowCategory][level - 1].empty()) {
      category++;
      nextCategoryUpdate -= categoryUpdate;
      continue;
    }
    size_t nowIndex = indexes[nowCategory][level - 1].front();
    indexes[nowCategory][level - 1].pop_front();
    auto& question = questions[nowCategory][level - 1][nowIndex];

    question.start();
    while (System::Update()) {
      question.draw();
      question.update();

      if (question.timer.reachedZero())
        break;
    }

    if (question.isCorrect()) {
      point += 10;
      CorrectSound.playOneShot();
      if (question.isSelected) {
        point += 10;
      } else {
        point += 15;
      }
    } else {
      WrongSound.playOneShot();
      if (question.isSelected) {
        point -= 7;
      } else {
        point -= 12;
      }
    }

    Console << U"次の問題へ";
  }

  Console << U"リザルト画面";
  // リザルト画面
  while (System::Update()) {
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
      rankText = U"伝説の理系";
    else
      rankText = U"TOP OF 理系";

    auto xAdvance = static_cast<int>(boldFont(U"難易度:  ").getXAdvances(50).sum());
    boldFont(U"難易度:  ").draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4), Palette::Black);
    boldFont(GetLevelInfo(level).first).draw(50, Arg::leftCenter(Scene::Height() / 4 + xAdvance, Scene::Height() / 4), GetLevelInfo(level).second);
    boldFont(U"スコア:  ", point).draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4 * 2), Palette::Black);
    boldFont(U"ランク:  ", rankText).draw(50, Arg::leftCenter(Scene::Height() / 4, Scene::Height() / 4 * 3), Palette::Black);
  }
}
