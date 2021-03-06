
#include <stdio.h>
#include <string.h>
#include "library.h"


static HashTable hashTable;

int hash(char * key,int base){
    int hashcode = 0;
    char c;
    int i = 0;
    while( (c=key[i++])!='\0'){
        hashcode += c;
    }
    return hashcode % 10007 % base;
}

// 初始化表
void init(){
    printf("初始化hashtable\n");
    hashTable.size = 100;
    hashTable.kv = (KeyValue**)malloc(sizeof(KeyValue*)*hashTable.size);
    int i = 0;
    for(i=0;i<hashTable.size;i++){
        hashTable.kv[i] = NULL;
    }
    hashTable.isInit = 1;

}
// 扩容
void extendCapcity(){
    printf("扩容\n");
    KeyValue ** kv = (KeyValue**)malloc(sizeof(KeyValue*)*2*hashTable.size);
    int i = 0;
    for(i=0;i<hashTable.length;i++){
        kv[hashTable.postions[i]] = hashTable.kv[hashTable.postions[i]];
    }
    hashTable.kv = kv;
}

static void putVal(char * key,char * value,int hashcode){
    if(hashcode > hashTable.size){
        extendCapcity();
    }
    hashTable.postions[hashTable.index] = hashcode;
    hashTable.index++;
    hashTable.length++;
    KeyValue * kv = (KeyValue*)malloc(sizeof(KeyValue));
    kv->key = (char *)malloc(sizeof(char)*strlen(key)+1);
    kv->value = (char *)malloc(sizeof(char)*strlen(value)+1);
    kv->next = NULL;
    strcpy(kv->key,key);
    strcpy(kv->value,value);
    hashTable.kv[hashcode] = kv;
}

// 解决哈希冲突
static void append(KeyValue * kv,char * key,char * value){
    printf("加链\n");
    KeyValue * newnode = (KeyValue*)malloc(sizeof(KeyValue));
    newnode->key = (char *)malloc(sizeof(char)*strlen(key)+1);
    newnode->value = (char *)malloc(sizeof(char)*strlen(value)+1);
    strcpy(newnode->key,key);
    strcpy(newnode->value,value);
    newnode->next = kv->next;
    kv->next = newnode;
}

static KeyValue * findKey(char * key,int hashcode){
    KeyValue * kv = NULL;
    KeyValue  * p = hashTable.kv[hashcode];
    kv = p->next;
    while(kv!=NULL){
        if(strcmp(kv->key,key)==0)
            return kv;
        kv = kv->next;
    }

    return kv;
}


void put(char * key,char * value){
    if(hashTable.isInit == 0){
        init();
    }
    int hashcode = hash(key,hashTable.size);
    KeyValue * kv = NULL;
    if(hashTable.kv[hashcode] == NULL){
        putVal(key,value,hashcode);
    } else if(strcmp(key,hashTable.kv[hashcode]->key)==0){
        printf("覆盖旧数据\n");
        strcpy(hashTable.kv[hashcode]->value,value);
    } else if( (kv=findKey(key,hashcode))!=NULL ){
        // 如果冲突的key存在
        printf("hash冲突\n");
        strcpy(kv->value,value);
    } else {
        // 如果hash冲突的key不存在，链起来
        append(hashTable.kv[hashcode],key,value);
    }
}

void delKV(char * key,int hashcode){
    KeyValue * p = hashTable.kv[hashcode];
    while(p!=NULL &&  strcmp(p->next->key,key))
        p = p->next;
    KeyValue * del = p->next;
    p->next = del->next;
    free(del);
}
char * get(char * key,int hashcode){
    if(hashTable.isInit == 0){
        init();
    }
    KeyValue * kv = NULL;
    // 如果这个hashcode没有存储过，直接返回NULL
    if(hashTable.kv[hashcode] == NULL){
        return NULL;
    } else {
        // 可能存在不同key 但是hashcode相同的情况
        if(strcmp(key,hashTable.kv[hashcode]->key)==0){
            return hashTable.kv[hashcode]->value;
        } else {
            if((kv=findKey(key,hashcode))!=NULL)
                return kv->value;
            else
                return NULL;
        }
    }
}

char * _get(char * key){
    if(hashTable.isInit == 0){
        init();
    }
    return get(key,hash(key,hashTable.size));
}
void removeKV(char * key){
    if(hashTable.isInit == 0){
        return;
    }
    int hashcode = hash(key,hashTable.size);
    if(get(key,hashcode)==NULL){
        return;
    } else {
        KeyValue * kv = NULL;
        if(strcmp(key,hashTable.kv[hashcode]->key)==0){
            kv = hashTable.kv[hashcode];
            hashTable.kv[hashcode] = kv->next;
            if(kv->next==NULL){
                // 这里需要再添加一个对positio的修改的功能
                hashTable.length--;
            }

            free(kv);
        } else if((kv=findKey(key,hashcode))!=NULL){
            delKV(key,hashcode);
        }
    }
}
void foreach(){
    printf("--------------------------------------------------\n");
    int i = 0;
    KeyValue * kv;
    for(i=0;i<hashTable.index;i++){
        kv = hashTable.kv[hashTable.postions[i]];
        if(kv==NULL)
            continue;
        printf("key:%s value:%s\n",hashTable.kv[hashTable.postions[i]]->key,
               hashTable.kv[hashTable.postions[i]]->value);
        KeyValue * nextnode = kv->next;
        while(nextnode!=NULL){
            printf("key:%s value:%s\n",nextnode->key,nextnode->value);
            nextnode = nextnode->next;
        }
    }
}
//int  main()
//{
//
//    init();
//    put("!#abc","huangsheng");
//    put("Dabc","liaili");
//    foreach();
//    put("!#abc","vv");
//    put("Dabc","aa");
//    put("abcD","gg");
//    put("acbD","ff");
//    put("name","fa");
//    put("email","11110@qq.com");
//    put("university","华东吃饭大学");
//    foreach();
//    printf("remove->\n");
//    removeKV("abcD");
//    foreach();
//    removeKV("!#abc");
//    foreach();
//    removeKV("!#abc");
//    printf("重复删除\n");
//    foreach();
//    removeKV("Dabc");
//    foreach();
//    removeKV("acbD");
//    foreach();
//    // printf("_get(Dabc)%s\n",_get("Dabc") );
//    // if(_get("name")==NULL){
//    //     printf("name is NULL\n");
//    //     put("name","hs");
//    // }
//    foreach();
//    // put("name","hanbindashadiao");
//    //foreach();
//    return 0;
//}