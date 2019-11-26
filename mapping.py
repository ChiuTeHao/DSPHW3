#-*- coding : Big5 -*-
import sys

if __name__=='__main__':
    with open(sys.argv[1],'r',encoding='big5hkscs') as f:
        lines=f.readlines()
        big5tozhuying=[]
        zhuyingtobig5dic=dict()
        for line in lines:
            print(line)
            word,zhuyings=line.split()
            zhuyinglist=zhuyings.split('/')
            initials=[]
            for zhuying in zhuyinglist:
                initial=zhuying[0]
                if initial not in zhuyingtobig5dic:
                    zhuyingtobig5dic[initial]=[word]
                else:
                    zhuyingtobig5dic[initial].append(word)
    with open(sys.argv[2],'w',encoding='big5hkscs') as f:
        for initial,words in zhuyingtobig5dic.items():
            f.write(initial)
            for word in words:
                f.write(' '+word)
            f.write('\n')       
