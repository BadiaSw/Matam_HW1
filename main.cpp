#include <fstream>
#include "Utilities.h"
#include "Transaction.h"
#include "BlockChain.h"
#include <iostream>

using std::string;
using std::ifstream;
using std::ofstream;
using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 4) {
        cout << getErrorMessage() << endl;
        return 0;
    }

    string Operator = argv[1];
    string sourceFile = argv[2];
    string targetFile = argv[3];

    if (Operator != "format" && Operator != "hash" && Operator != "compress" && Operator != "verify") {
        cout << getErrorMessage() << endl;
        return 0;
    }

    ifstream source(sourceFile);
    BlockChain blockChain = BlockChainLoad(source);

    if (Operator == "format") {
        ofstream target(targetFile);
        BlockChainDump(blockChain, target);
        target.close();

    } else {
        if (Operator == "hash") {
            ofstream target(targetFile);
            BlockChainDumpHashed(blockChain, target);
            target.close();

        } else {
            if (Operator == "compress") {
                ofstream target(targetFile);
                BlockChainCompress(blockChain);
                BlockChainDump(blockChain, target);
                target.close();
            } else {
                if (Operator == "verify") {
                    ifstream inputTarget(targetFile);
                    if (BlockChainVerifyFile(blockChain, inputTarget)) {
                        cout << "Verification passed" << endl;
                    } else {
                        cout << "Verification failed" << endl;
                    }
                    inputTarget.close();
                }
            }
        }
    
    
}
 deleteBlockChain(blockChain);
    source.close();
    return 0;
}
