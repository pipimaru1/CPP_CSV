// CPP_CSV4.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>

/*
EventID,Category,Location,StreamURL,Timestamp,Index_per_frame,index_max_frame,idx,ClassID,Confidence,ClassName,ScoreThreshold,NmsThreshold,ConfidenceThreshold,x0,y0,x1,y1,xc,yc,Width,Height,OnnxFileName,NamesFileName,ImageWidth,ImageHeight,LinkID
2251104,AI,組立第１工場K棟9番ホイールエンジンフード,10.3.45.87,2025-04-21 11:00:00.2391441,0,8,98,3,0.927734,Move,0.1,0.5,0.1,586,341,663,591,624,466,77,250,time_study.onnx,time_study.names,1280,960,0A032D57g.04LB0000
*/
/* CSVデータサンプル
[9][0] : 2251113
[9][1] : AI
[9][2] : 組立第１工場F棟4番クローラ組立エンジン付近
[9][3] : 10.3.45.89
[9][4] : 2025-04-21 11:00:00.3921934
[9][5] : 1
[9][6] : 4
[9][7] : 18
[9][8] : 2
[9][9] : 0.929199
[9][10] : Bring_Handling_Parts
[9][11] : 0.1
[9][12] : 0.5
[9][13] : 0.1
[9][14] : 1143
[9][15] : 574
[9][16] : 1270
[9][17] : 828
[9][18] : 1206
[9][19] : 701
[9][20] : 127
[9][21] : 254
[9][22] : time_study.onnx
[9][23] : time_study.names
[9][24] : 1280
[9][25] : 960
[9][26] : 0A032D59g.04LB0000
*/

struct CsvData
{
    //std::wstring    table;
    long long       EventID;    //0
    std::wstring    Category;   //1
    std::wstring    Location;   //2
	std::wstring    StreamURL;  //3 IPアドレス
    std::wstring    Timestamp;  //4

    int             Index_per_frame;//5
    int             index_max_frame;//6
	int             idx;            //7
    int             ClassID;        //8
    float           Confidence;     //9
    std::wstring    ClassName;      //10
    float           ScoreThreshold; //11
    float           NmsThreshold;   //12
    float           ConfidenceThreshold; //13
    int             x0;             //14
    int             y0;             //15
    int             x1;             //16
    int             y1;             //17
    int             xc;             //18
    int             yc;             //19
    int             Width;          //20
    int             Height;         //21
    std::wstring    OnnxFileName;   //22
    std::wstring    NamesFileName;  //23
    int             ImageWidth;     //24
    int             ImageHeight;    //25
    std::wstring    LinkID;         //26

};

////////////////////////////////////////////////////////////////
// 関数宣言
int load_csv(std::vector<CsvData>& _record, std::wifstream& _wif);

////////////////////////////////////////////////////////////////
// main 関数
// コマンドライン引数を受け取るために wmain を使用
// Windows では、wmain を使用して UTF-16 文字列を受け取ることができます。
// Linux では、main を使用して UTF-8 文字列を受け取ることができます。
int wmain(int argc, wchar_t* argv[])
{
	// 標準出力を UTF-16 に設定
	if (_setmode(_fileno(stdout), _O_U16TEXT) != 0)
	{
		std::wcerr << L"標準出力のモード設定に失敗しました。" << std::endl;
		return 1;
	}

    ///////////////////////////////////////////
    // 引数チェック
    if (argc < 2) {
        std::wcerr << L"使い方: " << argv[0] << L" <CSVファイル名>" << std::endl;
        return 1;
    }

    ///////////////////////////////////////////
    // コマンドライン引数からファイル名を取得
	const wchar_t* filename = argv[1]; // 引数１個目 0個目はプログラム名が入っている。

    /////////////////////////////////////////////
    std::wifstream wif{ filename, std::ios::binary };
    wif.imbue(std::locale{
        std::locale::classic(),
        new std::codecvt_utf8<wchar_t>
        });
    if (!wif) {
        std::wcerr << L"ファイルオープン失敗: " << filename << std::endl;
        return 1;
    }
	std::wcout << L"ファイルオープン成功: " << filename << std::endl;

    // CSVデータ格納用ベクタ
    std::vector<CsvData> records;
	// CSVファイル読み込み
    int _load_rows_num = load_csv(records, wif);

    std::wcout << L"読み込んだレコード数: " << _load_rows_num << L" 行" << std::endl;

    ////////////////////////////////////////////////////////////////////
	// 課題1: 1行分のデータが何バイトか調べてください。
    // 課題2: CsvDataを表示する関数を作成して、CsvData配列の任意の行のデータを表示してください。
    // ┣方法1: 構造体にデータを表示するメソッドを作成
    // ┗方法2: 構造体を引数に取って表示す関数を作成
    // 課題3: 「任意の行」はコマンドラインから指定できるようにしてください。
    // 課題4: 読み込んだデータを CSV ファイルに書き出してください
    // 課題5: [上級]CSVデータの要素の足りない行がある場合の処理を追加してください。
	// 課題6: [上級]データ要素の型が合わない場合の処理を追加してください。

	// 課題7: 巨大データを読み込んで、任意の数値データの平均を計算してください。
    // ┣100万行データ
    // ┣1億行データ
    // ┗パソコン限界(フリーズ、ブルースクリーンになるこがあるので、不要なアプリケーションは全て停止)
	//　　システムモニターなどでメモリ、ページング、CPUの使用率を確認してください。

	return 0;
}

////////////////////////////////////////////////////////////////////
// ここから下は関数定義
////////////////////////////////////////////////////////////////////
int load_csv(std::vector<CsvData>& _records, std::wifstream& _wif)
{
    std::wstring line;

    // ヘッダ読み飛ばし
    std::getline(_wif, line);

    // 行数カウント
    std::size_t rowCount = 0;
    while (std::getline(_wif, line)) 
    {
        ++rowCount;
    }

    /////////////////////////////
    // メモリ確保
	_records.clear();
    _records.reserve(rowCount);

    /////////////////////////////////////////
    // 再度ファイル先頭へ
    _wif.clear();
    _wif.seekg(0, std::ios::beg);

    /////////////////////////////////////////
    // ヘッダ再読み飛ばし
    std::getline(_wif, line);

    /////////////////////////////
    // 各行をパースして構造体に格納
    while (std::getline(_wif, line)) {
        std::wistringstream iss{ line };

        //構造体を一つ分作成
        CsvData rec;

        //要素一つ分のデータを格納するの文字列
        std::wstring cell;

        // 0: EventID
        std::getline(iss, cell, L',');
        rec.EventID = std::wcstoll(cell.c_str(), nullptr, 10);

        // 1: Category
        std::getline(iss, rec.Category, L',');

        // 2: Location
        std::getline(iss, rec.Location, L',');

        // 3: StreamURL
        std::getline(iss, rec.StreamURL, L',');

        // 4: Timestamp
        std::getline(iss, rec.Timestamp, L',');

        // 5: Index_per_frame
        std::getline(iss, cell, L',');
        rec.Index_per_frame = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));

        // 6: index_max_frame
        std::getline(iss, cell, L',');
        rec.index_max_frame = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));

        // 7: idx
        std::getline(iss, cell, L',');
        rec.idx = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));

        // 8: ClassID
        std::getline(iss, cell, L',');
        rec.ClassID = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));

        // 9: Confidence
        std::getline(iss, cell, L',');
        rec.Confidence = static_cast<float>(std::wcstod(cell.c_str(), nullptr));

        // 10: ClassName
        std::getline(iss, rec.ClassName, L',');

        // 11: ScoreThreshold
        std::getline(iss, cell, L',');
        rec.ScoreThreshold = static_cast<float>(std::wcstod(cell.c_str(), nullptr));

        // 12: NmsThreshold
        std::getline(iss, cell, L',');
        rec.NmsThreshold = static_cast<float>(std::wcstod(cell.c_str(), nullptr));

        // 13: ConfidenceThreshold
        std::getline(iss, cell, L',');
        rec.ConfidenceThreshold = static_cast<float>(std::wcstod(cell.c_str(), nullptr));

        // 14–21: x0, y0, x1, y1, xc, yc, Width, Height
        auto readInt = [&](int& target) {
            std::getline(iss, cell, L',');
            target = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));
            };
        readInt(rec.x0);
        readInt(rec.y0);
        readInt(rec.x1);
        readInt(rec.y1);
        readInt(rec.xc);
        readInt(rec.yc);
        readInt(rec.Width);
        readInt(rec.Height);

        // 22: OnnxFileName
        std::getline(iss, rec.OnnxFileName, L',');

        // 23: NamesFileName
        std::getline(iss, rec.NamesFileName, L',');

        // 24: ImageWidth
        std::getline(iss, cell, L',');
        rec.ImageWidth = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));

        // 25: ImageHeight
        std::getline(iss, cell, L',');
        rec.ImageHeight = static_cast<int>(std::wcstol(cell.c_str(), nullptr, 10));

        // 26: LinkID
        std::getline(iss, rec.LinkID, L',');

        _records.push_back(std::move(rec));
    }

    return _records.size() ;
}
