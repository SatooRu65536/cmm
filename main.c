#include <stdio.h>
#include <string.h>

void write2File(FILE *outputFile, char *word);
void put2File(FILE *outputFile, char c);
void evalWord(char *word, int isFunc, FILE *outputFile);
int parseLine(char *line, FILE *outputFile);
int readFile(FILE *fp, FILE *outputFile);
int setupFile(FILE **fp, FILE **outputFile, int argc, char const *argv[]);

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

// ファイルに文字列を書き込む
void write2File(FILE *outputFile, char *word) {
  fprintf(outputFile, "%s", word);
}

// ファイルに文字を書き込む
void put2File(FILE *outputFile, char c) { fprintf(outputFile, "%c", c); }

// 文字列の変数名をフォーマット指定子に置き換える
int replaceString(char *word, char *str) {
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
        if (c == ':') break;
      }

      char f = 0;
      char format[5] = {'\0'};

      while (1) {
        c = word[i];
        format[f] = c;

        if (c == '}') break;
        if (c == '\0') return 100;

        f++;
        i++;
      }

      str[j] = '%';
      for (int k = 0; k < f; k++) str[j + k + 1] = format[k];
      j += f + 1;
    }
    i++;
  }

  return 0;
}

// 文字列から変数名を取り出す
int pickupVariable(char *word, char *vars) {
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

            if (c == '\0') return 100;
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

  return 0;
}

// f文字列を処理する
int processFString(char *word, FILE *outputFile) {
  char str[256] = {'\0'};
  replaceString(word, str);
  write2File(outputFile, str);

  char vars[256] = {'\0'};
  pickupVariable(word, vars);
  write2File(outputFile, vars);

  return 0;
}

// 単語を評価する
void evalWord(char *word, int isFunc, FILE *outputFile) {
  // f文字列の場合
  if (word[0] == 'f' && word[1] == '"') {
    processFString(word, outputFile);
    return;
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
int parseLine(char *line, FILE *outputFile) {
  int i = 0;
  char word[256] = {'\0'};

  while (1) {
    char c = *line;
    int isFunc = 0;

    switch (c) {
      case '\0':
        word[i] = '\0';
        evalWord(word, 0, outputFile);
        return 0;

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

  return 0;
}

// ファイルを読み込む
int readFile(FILE *fp, FILE *outputFile) {
  // 1行ずつ読み込む
  char line[256];
  while (fgets(line, sizeof(line), fp) != NULL) {
    parseLine(line, outputFile);
  }
  return 0;
}

// ファイルを開く
int setupFile(FILE **fp, FILE **outputFile, int argc, char const *argv[]) {
  // ファイル名が指定されていない場合は終了
  if (argc < 2) {
    printf("ファイル名を指定してください\n");
    return -1;
  }

  // ファイルを開く
  *fp = fopen(argv[1], "r");
  if (*fp == NULL) {
    printf("\"%s\" ファイルを開けませんでした\n", argv[1]);
    return -1;
  }

  // 出力ファイルを開く
  if (argc < 3)
    *outputFile = fopen("out.c", "w");
  else
    *outputFile = fopen(argv[2], "w");

  // 出力ファイルを開けなかった場合は終了
  if (*outputFile == NULL) {
    printf("\"%s\" 出力ファイルを開けませんでした\n", argv[1]);
    return -1;
  }
  return 0;
}
