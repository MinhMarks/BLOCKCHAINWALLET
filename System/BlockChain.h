#include <bits/stdc++.h>
#include "sha256.h"
#include <chrono>
#include <ctime>
#include <cstdint>
using namespace std;



// Because everyone can make a such transaction, so we need private and puclib key to
// sign-create a signature that can be verify. 


class Transaction{

public: 
    string fromAddress, toAddress, signature; 
    int amount; 
    Transaction(string _fromAddress, string _toAddress, int _amount){
        fromAddress = _fromAddress; 
        toAddress = _toAddress; 
        amount = _amount; 
    }

    string CaculateHash(){
        stringstream ss;
        ss << fromAddress << toAddress << amount ;
        return Hash = sha256(ss.str());
    }
     
    void signTransaction( pairOfKey userKey  ) {
        if ( userKey.public != fromAddress) {
            cout << "You dont have permission for this action" ; 
            return; 
        }

        signature = userKey.Sign(CaculateHash()); 
    }

    bool IsValid(){
        if ( fromAddress == "nowhere" ) return true; 
        
        if ( signature == "" ) return false;

        const userPublicKey = keyFromPublic(fromAddress, 'hex'); 
        return userPublicKey.verify(this.caculatoopnHash(), this.signature); 
    }
}; 

class Block{ 
private: 
    time_t timestamp;
    string Hash; // hash code of this block 

public:
    vector<Transaction> transactions;
    int nonce = 0; 
    string previousHash;
    Block(time_t _timestamp, vector<Transaction> _transactions, string _previousHash = "" ){
        timestamp = _timestamp; 
        transactions = _transactions; 
        previousHash = _previousHash; 
    } 

    void print(){
        cout << "Time: " << timestamp << "\n"; 
        cout << "Prehash: " << previousHash<<"\n";
        cout << "hash: " << Hash << "\n";
        // cout << transactions <<"\n";  
    }
    
    string CaculateHash(){
        stringstream ss;
        ss << timestamp << previousHash << nonce;
        for ( Transaction i : transactions ) 
            ss << i.fromAddress << i.toAddress << i.amount ; 
        return Hash = sha256(ss.str());
    }

    string GetHash(){
        return Hash; 
    }
    
    void MineBlock(int nDifficulty) {
        char cstr[nDifficulty + 1];
        for (uint32_t i = 0; i < nDifficulty; ++i) {
            cstr[i] = '0';
        }
        cstr[nDifficulty] = '\0';

        string str(cstr);

        do {
            nonce++;
            CaculateHash();
        } while ( GetHash().substr(0, nDifficulty) != str);

        cout << "Block mined: " << GetHash() << endl;
    }

}; 

class Blockchain{

private: 
    int miningReward = 100; 
    vector <Block> chain;
    vector<Transaction> pendingTransactions; 

    Block GetLastestBlock(){
        return chain.back();

    }


public: 
    int _nDifficulty = 3; 
    Blockchain(){
        chain.emplace_back(Block(0, vector<Transaction>() )); 
    } 


    void MinePendingTransactions(string miningRewardAddress){
        Block newBlock = Block(34, pendingTransactions) ; 

        newBlock.previousHash = GetLastestBlock().GetHash(); 
        newBlock.MineBlock(_nDifficulty); 

        chain.emplace_back(newBlock); 

        pendingTransactions = vector < Transaction >(1, Transaction("nowhere",miningRewardAddress,miningReward)) ; 
    }

    void AddTransaction( Transaction newT){
        if ( !newT.IsValid() ) return; 
        pendingTransactions.push_back(newT); 
    }

    int GetBalanceOfAddress( string Address) {
        int ret = 0; 

        for ( Block i : chain ) 
            for ( Transaction j : i.transactions ) {
                if (j.fromAddress == Address) ret -= j.amount; 
                if (j.toAddress == Address) ret += j.amount; 
            }
        
        return ret; 
    }
    void print(){
        for ( int i =1; i< chain.size(); i++ ) chain[i].print(); 
    }
 
    bool IsChainValid(){

        for ( int i =1; i< chain.size(); i++ ){
            Block currentBlock = chain[i]; 
            Block previousBlock = chain[i-1]; 
1
            if ( !currentBlock.isValid() ) 
                return false;
            if ( currentBlock.GetHash() != currentBlock.CaculateHash() ) 
                return false; 

            if ( currentBlock.previousHash != previousBlock.GetHash() ) 
                return false; 
        }
        return true;
    }
}; 
