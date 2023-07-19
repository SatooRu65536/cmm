Main(Arg) {
  double r;
  double area;

  p("Input radius: ");
  s(f"{&r:lf");

  area = r * r * M_PI;
  print(f"Area = {area:lf}\n");

  return 0;
}