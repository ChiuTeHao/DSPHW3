#include"mymap.h"
MyMap::MyMap(Vocab &a,Vocab &b):zhuyin(a),big5(b)
{
        
}
void MyMap::print()
{
    FILE *fptr=fopen("output","w");
    for(map<VocabIndex,set<VocabIndex> >::iterator it=table.begin();it!=table.end();it++)
    {
        for(set<VocabIndex>::iterator it2=table[it->first].begin();it2!=table[it->first].end();it2++)
            fprintf(fptr,"%s %s\n",zhuyin.getWord(it->first),big5.getWord(*it2));
    }
    fclose(fptr);
}
void MyMap::read(File &file)
{
    char *content;
    FILE *fptr=fopen("output","w");
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
    for(it=table.begin();it!=table.end();it++)
    {
        VocabString key=zhuyin.getWord(it->first);
        set<VocabIndex> validx=it->second;
        for(set<VocabIndex>::iterator it2=validx.begin();it2!=validx.end();it2++)
            fprintf(fptr,"%u %s %u %s\n",it->first,key,*it2,big5.getWord(*it2));
    }
    table[zhuyin.getIndex("<s>")].insert(big5.getIndex("<s>"));
    table[zhuyin.getIndex("</s>")].insert(big5.getIndex("</s>"));
    fclose(fptr);
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
