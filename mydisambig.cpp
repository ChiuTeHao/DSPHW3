#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Ngram.h"
#include"Vocab.h"
#include"File.h"
#include"VocabMap.h"
const int MaxLengthPerLine=256;
const int MaxNumCandidate=1024;
const VocabIndex emptycontext[]={Vocab_None};
const LogP delta=-100;
LogP Viterbi(VocabMap &map,Ngram &lm,VocabString *words,unsigned int wordcnt,Vocab &voc,Vocab &vocz,Vocab &vocb);
VocabIndex getIndex(Vocab &vocab,VocabString w)
{
    VocabIndex idx=vocab.getIndex(w);
    if(idx==Vocab_None)
        return vocab.getIndex(Vocab_Unknown);
    else
        return idx;
}
int main(int argc,char* argv[])
{
    const int ngram_order=2;
    Vocab voc,vocz,vocb;
    Ngram lm(voc,ngram_order);
    VocabMap map(vocz,vocb);

    File lmfile(argv[3],"r");
    lm.read(lmfile);
    File mapfile(argv[2],"r");
    map.read(mapfile);
    File testfile(argv[1],"r");
    char *content;
    while((content=testfile.getline())!=NULL)
    {
        VocabString words[3000];
        unsigned int wordcnt=Vocab::parseWords(content,&words[1],3000);
        words[0]="<s>";
        words[wordcnt+1]="</s>";
        LogP prob=Viterbi(map,lm,words,wordcnt+2,voc,vocz,vocb);
    }
}
LogP Viterbi(VocabMap &map,Ngram &lm,VocabString *words,unsigned int wordcnt,Vocab &voc,Vocab &vocz,Vocab &vocb)
{
    LogP prob[MaxLengthPerLine][MaxNumCandidate]={};
    VocabIndex backtrack[MaxLengthPerLine][MaxNumCandidate]={};
    int num_candidate[MaxLengthPerLine]={};
    VocabIndex big5idx[MaxLengthPerLine][MaxNumCandidate]={};
    VocabIndex wbig5idx;
    Prob p;
    VocabMapIter iter(map,getIndex(voc,words[0]));
    for(int i=0;iter.next(wbig5idx,p);i++,num_candidate[0]++)
    {
        LogP currentprob=lm.wordProb(getIndex(voc,vocb.getWord(wbig5idx)),emptycontext);
        if(currentprob==LogP_Zero)
            currentprob=delta;
        prob[0][i]=currentprob;
        backtrack[0][i]=0;
        big5idx[0][i]=wbig5idx;
    }
    for(int t=1;t<wordcnt;t++)
    {
        VocabIndex wbig5idx;
        Prob p;
        for(int i=0;iter.next(wbig5idx,p);i++,num_candidate[t]++)
        {
            prob[t][i]=-1.0/0.0;
            backtrack[t][i]=-1;
            for(int j=0;j<num_candidate[t-1];j++)
            {
                VocabString lastword=vocb.getWord(big5idx[t-1][j]);
                VocabIndex context[]={getIndex(voc,lastword),Vocab_None};
                LogP currentprob=lm.wordProb(getIndex(voc,vocb.getWord(wbig5idx)),context);
                LogP unigramprob=lm.wordProb(getIndex(voc,vocb.getWord(wbig5idx)),emptycontext);
                if(currentprob==LogP_Zero&&unigramprob==LogP_Zero)
                    currentprob=delta;
                if(currentprob+prob[t-1][j]>=prob[t][i])
                {
                    prob[t][i]=currentprob+prob[t-1][j];
                    backtrack[t][i]=j;
                }
            }
            big5idx[t][i]=wbig5idx;
        }
    }
    int maxpath[MaxLengthPerLine]={};
    LogP maxprob=prob[wordcnt-1][0];
    int bestcandidateidx=0;
    for(int i=1;i<num_candidate[wordcnt-1];i++)
    {
        if(prob[wordcnt-1][i]>maxprob)
        {
            maxprob=prob[wordcnt-1][i];
            bestcandidateidx=i;
        }
    }
    maxpath[wordcnt-1]=bestcandidateidx;
    for(int i=wordcnt-2;i>=0;i--)
       maxpath[i]=backtrack[i+1][maxpath[i+1]];
    for(int t=0;t<wordcnt;t++)
    {
        printf("%s",vocb.getWord(big5idx[t][maxpath[t]]));
        if(t<wordcnt-1)
            printf(" ");
        else
            printf("\n");
    }
    return prob[wordcnt-1][maxpath[wordcnt]];
}
