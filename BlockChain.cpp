#include <string>
#include <fstream>
#include "BlockChain.h"
#include <iostream>
using namespace std;

/**
 * BlockChainGetSize - returns the number of Blocks in the BlockChain
 *
 * @param blockChain - BlockChain to measure
 *
 * @return Number of Blocks in the BlockChain
*/
int BlockChainGetSize(const BlockChain& blockChain){
        int counter=0;
        const Block *current;
        current=blockChain.Head;
        while(current!=nullptr){
            counter++;
            current=current->next;
        }
return counter;
}


/**
 * BlockChainPersonalBalance - returns the balance of a given person, relative to a given BlockChain
 *
 * @param blockChain - BlockChain to calculate the balance from
 * @param name - Name of the person to calculate the balance for
 *
 * @return Balance of the person
*/
int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name){
    Block *curr;
    int balance=0,received=0,spent=0;
    curr=blockChain.Head;
    while( curr!=nullptr){
        if (curr->transaction.sender==name)
        {
            spent=spent+curr->transaction.value;
        }
        if(curr->transaction.receiver==name){
            received=received+curr->transaction.value;

        }
        curr=curr->next;
    }
    balance=received-spent;
    return balance;
}

/**
 * BlockChainAppendTransaction - creates and appends a new transaction to the BlockChain
 *
 * @param blockChain BlockChain to append the transaction to
 * @param value Value of the transaction
 * @param sender Name of the sender
 * @param receiver Name of the receiver
 * @param timestamp String that holds the time the transaction was made
*/
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
){
    Transaction  transaction={value,sender,receiver};
    Block *newBlock= new Block(transaction,timestamp);
    newBlock->next=blockChain.Head;
    blockChain.Head=newBlock;
    }


/**
 * BlockChainAppendTransaction - appends a copy of a given transaction to the BlockChain
 *
 * @param blockChain BlockChain to append the transaction to
 * @param transaction Transaction we want to append
 * @param timestamp String that holds the time the transaction was made
*/
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
){
    Block *newBlock= new Block(transaction,timestamp);
    newBlock->next=blockChain.Head;
    blockChain.Head=newBlock;
}


/**
 * BlockChainLoad - Reads data from a file and creates a new block chain
 *
 * @param file Data file to read from
 *
 * @return BlockChain created from the file
 *
*/
BlockChain BlockChainLoad(ifstream& file){
string time;
BlockChain newBlockChain = {nullptr};
 string sender, receiver, timestamp;
 unsigned int value;
 Transaction transaction;
  while (file >> sender >> receiver >> value >> timestamp) {
        transaction.sender = sender;
        transaction.receiver = receiver;
        transaction.value = value;
        time=timestamp;
        newBlockChain=addBlock(newBlockChain,transaction,time);
  }
  return newBlockChain;

}
BlockChain addBlock(BlockChain blockChain,
         Transaction& transaction,
         string& timestamp){
    Block *new_Block = new Block(transaction, timestamp);
    if (blockChain.Head == nullptr) {
        blockChain.Head = new_Block;
    } else {
        Block *current = blockChain.Head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new_Block;
    }
    return blockChain;
}



/**
 * BlockChainDump - Prints the data of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 *
 * BlockChain info:
 * <n>.
 * Sender Name: <name>
 * Receiver Name: <name>
 * Transaction Value: <value>
 * Transaction Timestamp: <time>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
void BlockChainDump(const BlockChain& blockChain, ofstream& file){

 const Block *current = blockChain.Head;
    int num = 0;
    file << "BlockChain Info:" << endl;
    while (current != nullptr) {
        num++;
        file << num << "." << endl;
        file << "Sender Name: " << current->transaction.sender << endl;
        file << "Receiver Name: " << current->transaction.receiver << endl;
        file << "Transaction Value: " << current->transaction.value << endl;
        file << "Transaction timestamp: " << current->time << endl;
        current = current->next;
    }


};



/**
 * BlockChainDumpHashed - Prints the *hashed data* of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 * <hashed message>
 * <hashed message>
 * ...
 * <hashed message>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file){
   const Block *current = blockChain.Head;
    while (current != nullptr) {
        file << TransactionHashedMessage(current->transaction);
        if (current->next != nullptr) {
            file << endl;
        }
        current = current->next;
    }
}


/**
 * BlockChainVerifyFile - verifies that the file contains correct hashed messages of the given BlockChain
 *
 * Input file is expected to contain data in the following format:
 * <hashed message>
 * <hashed message>
 * ...
 * <hashed message>
 *
 * @param blockChain BlockChain to verify
 * @param file File to read from
 *
 * @return true if the file is valid, false otherwise
*/
bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file){
const Block *current = blockChain.Head;
    string hash;
    while (current != nullptr && file >> hash) {
        if (!TransactionVerifyHashedMessage(current->transaction, hash)) {
            return false;
        }
        current = current->next;
    }
    if (file.eof() == false || current != nullptr) {
        return false;
    }
    return true;
}


/**
 * BlockChainCompress - Compresses the given block chain based on the transaction's data.
 * All consecutive blocks with the same sender and receiver will be compressed to one Block.
 *
 * @param blockChain BlockChain to compress
*/
void BlockChainCompress(BlockChain& blockChain){
 Block *current = blockChain.Head;

    string timestamp;
    while (current != nullptr) {
        Block *iterator = current->next;
         timestamp = current->time;
 
        while (iterator != nullptr) {
            if (iterator->transaction.sender == current->transaction.sender &&
                iterator->transaction.receiver == current->transaction.receiver) {
                current->transaction.value += iterator->transaction.value;
                current->next = iterator->next;
                delete iterator;
                iterator = current->next;
            } else {
                 break;       
                 
            }
        }
        current->time = timestamp;
         current = current->next;
    }


}


/**
 * BlockChainTransform - Update the values of each transaction in the BlockChain
 *
 * @param blockChain BlockChain to update
 * @param function a pointer to a transform function
*/
void BlockChainTransform(BlockChain& blockChain, updateFunction function){
 Block *current = blockChain.Head;
    while (current != nullptr) {
        current->transaction.value = function(current->transaction.value);
        current = current->next;
    }


}

void deleteBlockChain(BlockChain blockChain) {
    Block *current = blockChain.Head;
    while (current != nullptr) {
        Block *temp = current;
        current = temp->next;
        delete temp;
    }
    blockChain.Head = nullptr;
}