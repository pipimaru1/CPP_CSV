#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <conio.h>
#include <codecvt>
#include <cstring>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

/////////////////////////////////////////////////
// メモリの違い
// 1. スタック領域（Stack）
// 自動変数（ローカル変数）：関数呼び出しのたびに確保され、関数を抜けると破棄される領域
// 引数：関数に渡されるパラメータも多くの環境でスタック上に配置される
// 戻り先アドレス／フレームポインタ：関数呼び出しのメタデータ
//
// 2. ヒープ領域（Heap）
// new / malloc などで動的確保する領域
// 明示的に delete / free しない限り生き続ける
// 大きなバッファやオブジェクトを関数のスコープ外でも参照したいときに使う
//
// 3. スタティック・静的領域（Static / Data セグメント）
// グローバル変数 や static 指定のローカル変数
// プログラム開始時に確保され，プログラム終了まで生存
// 初期化済みデータ領域（Dataセグメント） と 未初期化データ領域（BSSセグメント） に分かれる
// int g = 42; → Data セグメント
// static int s; → BSS セグメント

//////////////////////////////////////////
// 普段は意識しなくていい
// 大きなデータはヒープ領域に確保する
// 16KB以上のデータはヒープと覚えておく

/////////////////////////////////////////////////
// 課題
// 1. 一部文字列が尻切れトンボで表示されないので直してください
// 2. 読み込める行数を増やしてください 100k行くらい。
// 3. 数値型のデータを読み込んで、平均を計算してください
// 4. 読み込んだデータを CSV ファイルに書き出してください
// 5. [中級] 列数を無意味に増やしたら、どうなりますか？ 何が見えているか説明してください。(セキュリティ上の問題)
// 6. [上級] 年、月、日、時、分、秒を分けて表示してください
// 7. [上級] それぞれの値を int 型に変換して、時間差を計算して表示してください
// 8. [上級] 曜日を表示してください 

const int MAX_ROWS = 10;             // 最大行数
const int MAX_COLS = 27;             // 最大列数
const int MAX_LINE_LENGTH = 1024;    // 1 行あたりの最大文字数
const int MAX_CELL_LENGTH = 32;       // 1 セルあたりの最大文字数

//#define USE_HEAP

int main()
{
    // 標準出力を UTF-16 に設定
    _setmode(_fileno(stdout), _O_U16TEXT);

    // CSV ファイル名
    const wchar_t filename[] = L"ai_timestudy_10K.csv";

    // ファイルをバイナリモードで開き、UTF-8→UTF-16 変換を指定
    std::wifstream file(filename, std::ios::binary);
    file.imbue(std::locale(
        std::locale::classic(),
        new std::codecvt_utf8<wchar_t>));

    if (!file) {
        std::wcerr << L"ファイルオープンに失敗しました: " << filename << std::endl;
        return 1;
    }
    std::wcout << L"ファイルオープンに成功しました: " << filename << std::endl;

    // ヘッダ行を読み飛ばす
    wchar_t headerLine[MAX_LINE_LENGTH];
    file.getline(headerLine, MAX_LINE_LENGTH);

    ///////////////////////////////////////////////////////////
    // データ格納用 3 次元配列
	// 行数 x 列数 x 文字列

#ifndef USE_HEAP
    wchar_t data[MAX_ROWS][MAX_COLS][MAX_CELL_LENGTH];
#else
    // 型宣言：data は「MAX_COLS×MAX_CELL_LENGTH の配列がN個並ぶ領域」を指すポインタ
    wchar_t (*data)[MAX_COLS][MAX_CELL_LENGTH];

	// ヒープ領域にメモリ確保
    data = new wchar_t[MAX_ROWS][MAX_COLS][MAX_CELL_LENGTH];

#endif

    int rowCount = 0;
    wchar_t lineBuf[MAX_LINE_LENGTH];
    while (rowCount < MAX_ROWS && file.getline(lineBuf, MAX_LINE_LENGTH)) 
    {
        // 行を wstring に変換
        std::wstring line(lineBuf);
        std::wistringstream iss(line);
        std::wstring cell;

        // 各セルをカンマで分割して配列にコピー
        for (int col = 0; col < MAX_COLS; ++col) 
        {
            if (std::getline(iss, cell, L',')) 
            {
                // セルの長さが上限を超えないようにコピー
                size_t len = cell.copy(data[rowCount][col], MAX_CELL_LENGTH - 1);
                data[rowCount][col][len] = L'\0';
            }
            else 
            {
                // 値が足りない列は空文字
                data[rowCount][col][0] = L'NULL';
            }
        }
        ++rowCount;
    }
    file.close();

    std::wcout << L"読み込んだ行数: " << rowCount << L" 行" << std::endl;
    std::wcout << L"何かキーを押すと内容を表示します..." << std::endl;
    _getch();

    // 格納した内容を表示
    for (int i = 0; i < rowCount; ++i) {
        std::wcout << L"Row " << i << std::endl;
        for (int j = 0; j < MAX_COLS; ++j) 
        {
            if (data[i][j][0] != L'\0') 
            {
				// 行と列のインデックスを表示
                std::wcout << L"[" << i << L"][" << j << L"] : ";
				// セルの内容を表示
                std::wcout << data[i][j] << std::endl;
            }
            //if (j < MAX_COLS - 1) {
            //    std::wcout << L",";
            //}
        }
        std::wcout << std::endl;
    }
#ifdef USE_HEAP
	// ヒープ領域のメモリを解放
    // 自動的に解放されるので delete[] が無くても問題ないが、new[] したら delete[] するクセをつける
	// メモリを解放しないと、長時間使うアプリケーションだとメモリリークが発生する。メモリリークはシステムダウンやパフォーマンス低下を起こす。
    delete[] data;
#endif
    return 0;
 }
