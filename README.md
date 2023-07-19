# C-- Compiler
`C--` は C言語 をより単純に書ける **スーパーセット言語(笑)** で、  
`C-- Compiler` は そんな `C--` をコンパイルするツールです.

**授業程度であれば** 役に立たないこともないです.

C--構文 のエラー内容とエラー箇所も表示されるようになっています!!

## インストール方法
```shell
brew tap satooru65536/cmm
brew install cmm
```

## 実行方法
デフォルトでは、C言語のファイルを出力します.
```shell
cmm <options> [input file] [output file]
```
`[input file]` は入力するファイル名(必須)  
`[output file]` は出力するファイル名(省略可)

### オプション
- `-h` : ヘルプを表示します.
- `-r` : 実行までします. ファイルは出力されません.
- `-c` : clangでコンパイルします. -r オプションと同時に使用してください.

`-rc` のようにまとめて書くこともできます.

## 構文
### f文字列
文字列の前に `f` をつけると、文字列中で変数を展開できます.  
展開する変数は `{}` で囲み、型を `:` の後で指定します.
```c
printf(f"num={num:d}\n");
scan(f"{n:d} {m:d}");
```

### print文
`print` 文は `printf` と同じです.  
これでPython後でも書き間違えません!!  
もっと書く量を減らしたい場合は `p` でもOKです.
```c
print("Hello, World!\n");
p("Hello, World!\n");
```

### scan文
`scan` 文も `print` と同様です.
```c
scan("%d %d", &n, &m);
s("%d %d", &n, &m);
```

### main関数
ここまで来たら `main` 関数も省略したくなりますよね.
```c
Main(int argc, char const *argv[]) {
    // コード
}
```

main関数の引数も大体同じなので省略しましょう.
```c
Main(Arg) {
    // コード
}
```

### include
`include` も書くのが面倒!!  
授業程度であれば書かなくても(大体は)勝手に `#include <xxx.h>` を挿入してくれます.  
対応しているライブラリと関数(変数)は以下の通りです.

#### stdio.h
- p関数
- print関数
- print関数

#### stdlib.h
- rand関数
- random関数
- srand関数

#### string.h
- strlen関数
- strcmp関数
- strcpy関数
- strcat関数

#### math.h
- M_PI
- sin関数
- cos関数
- tan関数
- asin関数
- acos関数
- atan関数
- atan2関数
- exp関数
- log関数
- log10関数
- pow関数
- sqrt関数
- ceil関数
- floor関数

## サンプルコード
- [sample.c](sample.c)

# LICENSE
[MIT](./LICENSE)
