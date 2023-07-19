Main(Arg) {
  int heightsCm[5] = {0};
  double heightsMeter[5] = {0.0};

  for (int i = 0; i < 5; i++) {
    p(f"{i + 1:d}人目の身長? ");
    s(f"{&heightsCm[i]:d}");
  }

  p("--- Convert Meter ---\n");

  for (int i = 0; i < 5; i++) {
    heightsMeter[i] = heightsCm[i] / 100.0;
  }

  for (int i = 0; i < 5; i++) {
    print(f"{i + 1:d}人目の身長: {heightsMeter[i]:f} m\n");
  }

  return 0;
}