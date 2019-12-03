#include"Prob.h"
#include<map>
#include<Vocab.h>
#include<File.h>
#include<set>
#include<vector>
using namespace std;
class MyMap
{
    public:
        MyMap(Vocab &voc1,Vocab &voc2);
        MyMap();
        void read(File &file);        
    protected:
        map<VocabIndex,set<VocabIndex> > table;
        Vocab zhuyin,big5;
};
class MyMapIter:public MyMap
{
    public:
        MyMapIter(MyMap &mymap,VocabIndex idx);
        bool next(VocabIndex &idx,Prob &p);
    private:
        vector<VocabIndex> vec;
        int cnt;
};
