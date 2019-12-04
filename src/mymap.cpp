#include"mymap.h"
MyMap::MyMap(Vocab &a,Vocab &b):zhuyin(a),big5(b)
{
        
}
void MyMap::read(File &file)
{
    char *content;
    while((content=file.getline())!=NULL)
    {
        VocabString words[3000];
        unsigned int wordcnt=Vocab::parseWords(content,&words[0],3000);
        VocabIndex key=zhuyin.addWord(words[0]);
        for(int i=1;i<wordcnt;i++)
        {
            VocabIndex val=big5.addWord(words[i]);
            table[key].insert(val);
        }
    }
    map<VocabIndex,set<VocabIndex> >::iterator it;
    table[zhuyin.getIndex("<s>")].insert(big5.getIndex("<s>"));
    table[zhuyin.getIndex("</s>")].insert(big5.getIndex("</s>"));
}
vector<VocabIndex> MyMap::getVal(VocabIndex zhuyinidx)
{
    vector<VocabIndex> v;
    for(set<VocabIndex>::iterator it=table[zhuyinidx].begin();it!=table[zhuyinidx].end();it++)
        v.push_back(*it);
    return v;
}
MyMapIter::MyMapIter(MyMap &mymap,VocabIndex idx)
{
    vec=mymap.getVal(idx);
    cnt=0;
}
bool MyMapIter::next(VocabIndex &idx,Prob &p)
{
    if(cnt<vec.size())
    {
        idx=vec[cnt];
        cnt+=1;
        return true;
    }
    else
        return false;
}
