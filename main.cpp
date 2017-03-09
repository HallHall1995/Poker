//
//  main.cpp
//  Tpoker
//
//  Created by 田中　悠生 on 2015/01/02.
//
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
using namespace std;

struct card{
    int Num; //カードのナンバー
    int Mark;//記号、スペード(0) クラブ(1) ダイヤ(2) ハート(3)
    int St;//ストレート判定時使用
    bool operator<(const struct card & r) const {
        if(St == r.St){
            return Mark < r.Mark;
        }else{
            return St < r.St;
        }
    }
};


void makeDeck(vector<card> &d){//デッキ作り
    int i, j, r=0;
    card c;
    for (i=1; i<14; i++){
        for (j=0; j<4; j++){
            c.Num = i;
            c.Mark = j;
            if(i==1){
                c.St = 14;
            }else{
                c.St = i;
            }
            d.push_back(c);
            r += 1;
        }
    }
    //ジョーカー用
    c.Num = 14;
    c.Mark = 4;
    c.St = 15;
    d.push_back(c);
}


void deal(int maisu, vector<card> &target, vector<card> &d){//カードを配る
    int i;
    for(i=0; i<maisu; i++){
        target.push_back(d.back());
        sort(target.begin(),target.end());
        d.pop_back();
    }
}


void show(vector<card> target){//手札表示
    int i;
    char M[4][12] = {"Spade","Clubs","Diamonds","Hearts"};
    for(i=0; i<target.size(); i++){
        if(target[i].Mark == 4){
            printf("No%d: JOKER\n", i+1);
        }else{
            printf("No%d: %d of %s\n", i+1, target[i].Num, M[target[i].Mark]);
        }
    }
    printf(" \n");
}


void trade(vector<card> &target, vector<card> &d){//カード交換
    int maisu, koukan, i;
    printf("何枚カードを交換しますか？（数字のみ入力）\n");
    scanf("%d",&maisu);
    for (i=0; i<maisu; i++){
        printf("どのNoのカードを交換しますか？(一つ入力するごとにEnterを押してください）\n");
        scanf("%d",&koukan);
        target[koukan-1] = d.back();
        d.pop_back();
    }
    sort(target.begin(),target.end());
}


void draw_yaku(int yaku){
    char hyoji[10][45] = {"ノーペア","ワンペア","ツーペア","スリーカード","ストレート","フラッシュ","フルハウス","フォーカード","ストレートフラッシュ","ロイヤルストレートフラッシュ"};
    printf("あなたのカードの組合わせは%sです\n",hyoji[yaku]);
}


int yaku_hantei(int P, int S){//役判定judg内で使用
    if(S>=3){//ストレートフラッシュ以上の場合
        return S+5;
    }
    if(P==6){//フォーカード
        return 7;
    }else if(P==4){//フルハウス
        return 6;
    }
    if(S>0){//ストレート以上
        return S+3;
    }else{//それ以外
        return P;
    }
    return 0;
}


int judg(const vector<card> target){//役判定
    int joker=0, i, j;
    vector<card> copy(target);
    
    int yaku=0;
    //ワンペア：１　ツーペア：２　スリーカード：３　ストレート：４　フラッシュ：５
    //フルハウス：６　フォーカード：７　ストレートフラッシュ：８　ロイヤルストレートフラッシュ：９
    
    int P=0;//ペア判定 ワンペア：１ ツーペア：2 スリーカード：３ フルハウス：４　フォーカード：６
    int S=0;//ストレート,フラッシュ判定　ストレート：１　フラッシュ：２　ストレートフラッシュ：３　ロイヤル：４
    
    for(i=0; i<target.size(); i++){//ジョーカー判定
        if(target[i].Mark == 4){
            joker = 1;
            copy.erase(copy.begin() + i);
            break;
        }
    }
    
    //ペアチェック
    for(i=0; i<copy.size(); i++){
        for(j=i+1; j<copy.size(); j++){
            if(copy[i].Num == copy[j].Num){
                P += 1;
                //
                printf("pcheck:%d\n",P);
                //
            }
        }
    }
        
    //ストレートチェック
    if(P<1){//ペアがない
        if(copy.back().St - copy.front().St == copy.size()-1){
            S = 1;
        }
        if(joker == 1){
            if((copy.back().St - copy.front().St == copy.size())or(copy.back().St - copy.front().St == copy.size()-1)){
                S = 1;
            }
        }
    }
        
    //フラッシュチェック
    for(i=1; i<copy.size(); i++){
        if(copy[0].Mark!=copy[i].Mark){
            break;
        }
        if(i==copy.size()-1){
            if(S==1){
                S = 3;
            }else{
                S = 2;
            }
        }
    }
    
    //ロイヤルストレートフラッシュ判定
    
    if(S == 3){
        if(joker){
            if((copy.front().St == 10)or(copy.front().St==11)){
                S =4;
            }
        }else{
            if(copy.front().St == 10){
                S = 4;
            }
        }
    }
    
    if(joker){
        switch (P){//ジョーカー補正
            case 0:
                P = 1;
                break;
            case 1:
                P = 3;
                break;
            case 2:
                P = 4;
                break;
            case 3:
                P = 6;
                break;
        }
    }

    yaku = yaku_hantei(P,S);
    draw_yaku(yaku);
    return yaku;
}


void katimake(int p1_rank, int p2_rank){
    if(p1_rank > p2_rank){
        printf(" \n");
        printf("player1の勝利です\n");
        printf(" \n");
    }else if(p1_rank < p2_rank){
        printf(" \n");
        printf("player2の勝利です\n");
        printf(" \n");
    }else{
        printf(" \n");
        printf("引き分けです\n");
        printf(" \n");
    }
}


int main(){
    int a=1;
    
    while (a==1) {
        
        int n,i;
        int player1_rank,player2_rank;
        int owari;
        
        vector<card> deck; //デッキ
        vector<card> player1;//手札１
        vector<card> player2;//手札２
        
        //下準備
        makeDeck(deck);
        srand((unsigned)time(NULL));
        n = rand()%100+1;
        for(i=0; i<n; i++){
            random_shuffle(deck.begin(), deck.end());
        }
        deal(5,player1,deck);
        deal(5,player2,deck);
            //プレイ
        printf(" \n");
        printf("player1の番です\n");
        printf(" \n");
        show(player1);
        trade(player1,deck);
        show(player1);
        player1_rank = judg(player1);
    
        printf(" \n\n\n");
        printf("player2の番です\n");
        printf(" \n");
        show(player2);
        trade(player2,deck);
        show(player2);
        player2_rank = judg(player2);
    
        katimake(player1_rank,player2_rank);
        
        printf("終わる場合は 0 を入力してください");
        scanf("%d",&a);
        
    }
    return 0;
}