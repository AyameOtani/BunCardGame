#include "Master.h"


// playerの見た目を入れるための箱 ポインタじゃないからnullptrだと外部出る。
//std::string Master::msPlayerModelName = "";

int Master::mpTurnCount = 1; // はじめは1ターン

//スコアに使うやつの実態
int Master::mpSaveHp = 0;
int Master::mpSaveCardCount = 0;
int Master::mpSaveItemCount = 0;