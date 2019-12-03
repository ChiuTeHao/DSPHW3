#include"mymap.h"
MyMap::MyMap(Vocab &a,Vocab &b):zhuyin(a),big5(b)
{
        
}
MyMap::MyMap()
{
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
            fprintf(fptr,"%s %s\n",key,big5.getWord(*it2));
    }
    fclose(fptr);
}
MyMapIter::MyMapIter(MyMap &mymap,VocabIndex idx)
{
    for(set<VocabIndex>::iterator it=table[idx].begin();it!=table[idx].end();it++)
        vec.push_back(*it);
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
