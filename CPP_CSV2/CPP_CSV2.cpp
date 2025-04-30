#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>
#include <locale> 
#include <codecvt>
#include <windows.h>

// サンプル: CSV ファイルの各行を固定長の wchar_t[][] 配列に読み込む
// MSVC / VS2022 用

const int MAX_ROWS = 10;              // 最大行数
const int MAX_LINE_LENGTH = 512;     // 1 行あたりの最大文字数

int main() 
{
    ////////////////////////////////////////////////////////////
    // 標準入出力の出力を UTF-16 に設定
    _setmode(_fileno(stdout), _O_U16TEXT);

   
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // wchar_t は実装依存のワイド文字型：Windows では 2 バイト（UTF-16）、Linux では 4 バイト（UTF-32）
    // Unicode のコードポイントを直接保持でき、ひらがな・漢字・絵文字などを扱える   
    //////////////////////////////////////////////////////////////////////////////////////////////////

    // ヘッダ行をスキップするためのバッファ
    wchar_t header[MAX_LINE_LENGTH];

    // データ保存用の二次元配列
    wchar_t data[MAX_ROWS][MAX_LINE_LENGTH];

    wchar_t filename[] = L"ai_timestudy_1K.csv"; // 読み込む CSV ファイル名

    ////////////////////////////////////////////////////////////
    // バイナリモードで開いて，UTF-8→UTF-16 facet を設定

    std::wifstream file(filename, std::ios::binary);
    file.imbue(std::locale(std::locale::classic(),
        new std::codecvt_utf8<wchar_t>));

    if (!file) 
    {
        std::wcerr << L"ファイルオーブンに失敗しました : " << filename << std::endl;
        return 1;
    }
    else
    {
        std::wcout << L"ファイルオープンに成功しました : " << filename << std::endl;
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

    std::wcout << L"ファイルを読み込みました。" << std::endl;
    // 読み込んだレコード数を表示
    std::wcout << L"読み込んだ行数 : " << rowCount << std::endl;
    std::wcout << L"何かキーを押してください。" << std::endl;

    // データを表示する前にキー入力待ち
    _getch();

    // 各行の内容を表示
    for (int i = 0; i < rowCount; ++i) {
        std::wcout << L"Row " << i << ": " << data[i] << std::endl;
    }

    return 0;
}
