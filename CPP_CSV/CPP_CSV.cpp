#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>  // _getch() 用

// サンプル: CSV ファイルの各行を固定長の char[][] 配列に読み込む
// MSVC / VS2022 用

const int MAX_ROWS = 10;              // 最大行数
const int MAX_LINE_LENGTH = 512;     // 1 行あたりの最大文字数

int main() {
    // ヘッダ行をスキップするためのバッファ
    char header[MAX_LINE_LENGTH];

    // データ保存用の二次元配列
    char data[MAX_ROWS][MAX_LINE_LENGTH];

	char filename[] = "ai_timestudy_1K.csv"; // 読み込む CSV ファイル名

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ファイルオーブンに失敗しました : "<<filename << std::endl;
        return 1;
    }
    else
    {
		std::cout << "ファイルオープンに成功しました : " << filename << std::endl;
    }

    // ヘッダ行を読み込んで破棄
    file.getline(header, MAX_LINE_LENGTH);

    // データ行を読み込み
    int rowCount = 0;
    while (rowCount < MAX_ROWS && file.getline(data[rowCount], MAX_LINE_LENGTH)) 
    {
        ++rowCount;
    }
    file.close();

    std::cout << "ファイルを読み込みました。" << std::endl;
    // 読み込んだレコード数を表示
    std::cout << "読み込んだ行数 : " << rowCount <<std::endl;
    std::cout << "何かキーを押してください。" << std::endl;

    // データを表示する前にキー入力待ち
    _getch();


    // 各行の内容を表示
    for (int i = 0; i < rowCount; ++i) {
        std::cout << "Row " << i << ": " << data[i] << std::endl;
    }

    return 0;
}
