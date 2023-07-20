# C-- Language / Compiler
`C--` は C言語 をより単純に書ける **スーパーセット言語(笑)** で、  
`C-- Compiler` は そんな `C--` をコンパイルするツールです.

C--の特徴は以下の通りです.
- `f文字列` が使える
- `printf` `scanf` を `print` `scan` と簡略化できる
- main関数を簡略化できる
- (一部) `#include` が自動で挿入される

**授業程度であれば** 役に立たないこともないです.

## セットアップ
`make` コマンドか `brew` でインストールしてください.

```shell
make
# or 
brew tap satooru65536/cmm
brew install cmm
```

## 実行方法
以下のような C-- で書かれた `sample.c` を用意します.
```c:sample.c
Main(Arg) {
  double r;
  double area;

  p("Input radius: ");
  s(f"{&r:lf}");

  area = r * r * M_PI;
  print(f"Area = {area:lf}\n");

  return 0;
}
```

コマンドを実行すると
```shell
cmm sample.c out.c
```

以下のような c言語 のファイル(out.c) を生成します.
```c:out.c
#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[]) {
  double r;
  double area;

  printf("Input radius: ");
  scanf("%lf", &r);

  area = r * r * M_PI;
  printf("Area = %lf\n", area);

  return 0;
}
```

また `-r` オプションをつけると実行まで行えます.
```shell
cmm -r sample.c
```
> Input radius: 10  
> Area = 314.159265  

## 詳細
詳細は [DETAIL.md](DETAIL.md) を参照してください.

## ライセンス
[MIT License](LICENSE)
