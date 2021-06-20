#include <bits/stdc++.h>
using namespace std;

#define MAX_WT 4000000

unordered_map<string, int> blockHash;
unordered_set<int> includedBlocks = unordered_set<int>();
vector<int> printedBlocks = vector<int>();
int dp[MAX_WT + 1] = {0};
bool visited[5216]={false};

struct Block
{
  string txid;
  int fee, wt;
  vector<string> pTxids = vector<string>();
  int netWt = 0, netFee = 0;
  unordered_set<int> ancestors = unordered_set<int>();

  Block(string txidIn, int feeIn, int wtIn, vector<string> &pTxidsIn)
  {
    txid = txidIn;
    fee = feeIn;
    wt = wtIn;
    pTxids = pTxidsIn;
  }

  string toString()
      const
  {
    string par = "";
    string anc = "";

    for (auto p : pTxids)
    {
      par += (p + " ; ");
    }
    for (auto a : ancestors)
    {
      anc += (to_string(a) + " ");
    }

    return "tx_id : " + txid + "\n" +
           "fee : " + to_string(fee) + "\n" +
           "weight : " + to_string(wt) + "\n" +
           "parents : " + (par == "" ? "NULL" : par) + "\n" +
           "netFee : " + to_string(netFee) + "\n" +
           "netWeight : " + to_string(netWt) + "\n" +
           "ancestors : " + anc + "\n\n";
  }
};

void DFS(int indx, vector<Block> &blocks)
{
  int pSize = blocks[indx].pTxids.size();

  blocks[indx].netWt = blocks[indx].wt;
  blocks[indx].netFee = blocks[indx].fee;

  for (int i = 0; i < pSize; ++i)
  {
    DFS(blockHash[blocks[indx].pTxids[i]], blocks);
    blocks[indx].netWt += blocks[blockHash[blocks[indx].pTxids[i]]].netWt;
    blocks[indx].netFee += blocks[blockHash[blocks[indx].pTxids[i]]].netFee;
    blocks[indx].ancestors.insert(blockHash[blocks[indx].pTxids[i]]);
  }
}

void dfsBlocks(vector<Block> &blocks)
{

  for (int i = 0; i < blocks.size(); ++i)
  {
    blockHash[blocks[i].txid] = i;
  }

  for (int i = 0; i < blocks.size(); ++i)
  {
    if (!blocks[i].netWt)
      DFS(i, blocks);
  }
}

void DFSInclude(int indx,vector<Block> &blocks)
{
  int pSize = blocks[indx].pTxids.size();


  for (int i = 0; i < pSize; ++i)
  {
    DFS(blockHash[blocks[indx].pTxids[i]], blocks);
    printedBlocks.push_back(blockHash[blocks[indx].pTxids[i]]);
    visited[blockHash[blocks[indx].pTxids[i]]]=true;

  }
}
  void dfsBlocksInclude(vector<Block> & blocks)
  {

    for (int i = 0; i < blocks.size(); ++i)
    {
      if(!visited[i])
        {DFSInclude(i, blocks);
        visited[i]=true;
        }
    }
  }

  int knapSack(vector<Block> & blocks)
  {
    int nBlocks = blocks.size();

    for (int i = 0; i < nBlocks; ++i)
    {
      if (includedBlocks.find(i) == includedBlocks.end())
      {
        for (int j = MAX_WT; j >= blocks[i].netWt; --j)
        {
          if (dp[j] < blocks[i].netFee + dp[j - blocks[i].netWt])
          {
            dp[j] = blocks[i].netFee + dp[j - blocks[i].netWt];
            includedBlocks.insert(i);
          }
        }
      }
    }
    return dp[MAX_WT];
  }

  int main()
  {

    ifstream fin("mempool.csv");

    vector<Block> blocks;

    string row, word;
    vector<string> tokens(4);
    vector<string> parents = vector<string>();
    stringstream ss;

    while (getline(fin, row))
    {
      tokens.clear();
      parents.clear();
      row.append(";");
      ss.clear();
      ss.str(row);

      while (getline(ss, word, ','))
      {
        tokens.push_back(word);
      }
      if (tokens[0] != "tx_id")
      {
        ss.clear();
        ss.str(tokens[3]);

        while (getline(ss, word, ';'))
        {

          if (word.size())
            parents.push_back(word);
        }

        blocks.push_back(Block(tokens[0], stoi(tokens[1]), stoi(tokens[2]), parents));
      }
    }
    fin.close();
    dfsBlocks(blocks);

    // for (auto block : blocks)
    // {
    //   cout << block.toString();
    // }

    knapSack(blocks);

    dfsBlocksInclude(blocks);

    ofstream fout("MempoolOutput.txt");
    for (auto i : printedBlocks)
    {
      fout<<blocks[i].txid<<"\n";
    }

    // system("pause");
    return 0;
  }
