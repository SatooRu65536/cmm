#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

void outputError(int errNum, char *word)； void write2File(FILE *outputFile,
                                                           char *word);
void put2File(FILE *outputFile, char c);
void evalWord(char *word, int isFunc, FILE *outputFile);
void parseLine(char *line, FILE *outputFile);
void readFile(FILE *fp, FILE *outputFile);
void setupFile(FILE **fp, FILE **outputFile, int argc, char const *argv[]);

int lineNum = 0;
int charNum = 0;

int main(int argc, char const *argv[]) {
  FILE *fp;
  FILE *outputFile;

  // 入出力ファイルを開く
  setupFile(&fp, &outputFile, argc, argv);

  // ファイルを読み込む
  readFile(fp, outputFile);

  // ファイルを閉じる
  fclose(fp);
  fclose(outputFile);

  return 0;
}

// エラーを出力して終了する
void outputError(int errNum, char *word) {
  printf(RED);
  switch (errNum) {
    case 100:
      printf("Error: f文字列中の {} が閉じられていません.\n");
      break;

    case 101:
      printf("Error: f文字列中の {} 内に : がありません.\n");
      break;

    case 102:
      printf("Error: f文字列中でフォーマット演算子が指定されていません.\n");
      break;

    case 400:
      printf("Error: ファイルが開けません.\n");
      printf("       ファイル名: %s\n", word);
      break;

    case 401:
      printf("Error: 出力ファイルが開けません.\n");
      printf("       ファイル名: %s\n", word);
      break;

    default:
      printf("Error: エラー内容が未定義です.\n");
      break;
  }

  if (lineNum != 0) {
    printf("line %d:%d\n", lineNum, charNum);
    printf(RESET "  %s\n\n", word);
  } else {
    printf(RESET "\n");
  }

  exit(1);
}

// ファイルに文字列を書き込む
void write2File(FILE *outputFile, char *word) {
  fprintf(outputFile, "%s", word);
}

// ファイルに文字を書き込む
void put2File(FILE *outputFile, char c) { fprintf(outputFile, "%c", c); }

// 文字列の変数名をフォーマット指定子に置き換える
void replaceString(char *word, char *str) {
  // f"{hoge:d} {fuga:lf}" -> "%d %d"
  int i = 1;
  int j = 0;

  while (1) {
    char c = word[i];
    if (c == '\0') break;

    if (c != '{') {
      str[j] = c;
      j++;
    } else {
      i++;
      while (1) {
        c = word[i];
        i++;
        if (c == '}') outputError(101, word);
        if (c == ':') break;
      }

      char f = 0;
      char format[5] = {'\0'};

      if (word[i] == '}') outputError(102, word);
      while (1) {
        c = word[i];
        format[f] = c;

        if (c == '}') break;
        if (c == '\0') outputError(100, word);

        f++;
        i++;
      }

      str[j] = '%';
      for (int k = 0; k < f; k++) str[j + k + 1] = format[k];
      j += f + 1;
    }
    i++;
  }
}

// 文字列から変数名を取り出す
void pickupVariable(char *word, char *vars) {
  // f"{hoge:d} {fuga:lf}" -> , hoge, fuga
  int i = 0;
  int j = 0;

  while (1) {
    char c = word[i];
    if (c == '\0') {
      break;
    }

    if (c == '{') {
      i++;
      vars[j] = ',';
      vars[j + 1] = ' ';
      j += 2;
      while (1) {
        c = word[i];

        if (c == ':') {
          while (c != '}') {
            c = word[i];
            i++;

            if (c == '\0') outputError(100, word);
          }
          break;
        }

        vars[j] = c;
        i++;
        j++;
      }
    }
    i++;
  }
}

// f文字列を処理する
void processFString(char *word, FILE *outputFile) {
  char str[256] = {'\0'};
  replaceString(word, str);
  write2File(outputFile, str);

  char vars[256] = {'\0'};
  pickupVariable(word, vars);
  write2File(outputFile, vars);
}

// 単語を評価する
void evalWord(char *word, int isFunc, FILE *outputFile) {
  // f文字列の場合
  if (word[0] == 'f' && word[1] == '"') {
    processFString(word, outputFile);
  }

  // 関数ではないとき
  if (!isFunc) {
    write2File(outputFile, word);
    return;
  }

  // 関数名を変換する
  if (strcmp(word, "p") == 0 || strcmp(word, "print") == 0) {
    write2File(outputFile, "printf");
  } else if (strcmp(word, "s") == 0 || strcmp(word, "scan") == 0) {
    write2File(outputFile, "scanf");
  } else {
    write2File(outputFile, word);
  }
}

// 1行をパースする
void parseLine(char *line, FILE *outputFile) {
  int i = 0;
  char word[256] = {'\0'};

  while (1) {
    char c = *line;
    int isFunc = 0;

    switch (c) {
      case '\0':
        word[i] = '\0';
        evalWord(word, 0, outputFile);
        return;

      case '"':
        // 次の " までを文字列として扱う
        word[i] = c;
        i++;
        line++;
        while (1) {
          c = *line;
          word[i] = c;
          i++;
          if (c == '"') {
            break;
          }
          line++;
        }
        break;

      case '(':
      case ')':
        isFunc = 1;
      case ' ':
      case '{':
      case '}':
      case ',':
        word[i] = '\0';
        evalWord(word, isFunc, outputFile);
        put2File(outputFile, c);
        i = 0;
        break;

      default:
        word[i] = c;
        i++;
        break;
    }
    line++;
  }
}

// ファイルを読み込む
void readFile(FILE *fp, FILE *outputFile) {
  // 1行ずつ読み込む
  char line[256];
  while (fgets(line, sizeof(line), fp) != NULL) {
    lineNum++;
    parseLine(line, outputFile);
  }
}

// ファイルを開く
void setupFile(FILE **fp, FILE **outputFile, int argc, char const *argv[]) {
  // ファイル名が指定されていない場合は終了
  if (argc < 2) {
    char *filename = (char *)argv[1];
    outputError(400, filename);
  }

  // ファイルを開く
  *fp = fopen(argv[1], "r");
  if (*fp == NULL) {
    char *filename = (char *)argv[1];
    outputError(400, filename);
  }

  // 出力ファイルを開く
  if (argc < 3)
    *outputFile = fopen("out.c", "w");
  else
    *outputFile = fopen(argv[2], "w");

  // 出力ファイルを開けなかった場合は終了
  if (*outputFile == NULL) {
    char *filename = (char *)argv[1];
    outputError(401, filename);
  }
  return;
}
