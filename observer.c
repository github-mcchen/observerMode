#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Observer_t
{
    char name[32];
    bool (*adapter)(char *themeName);
    bool (*update_proc)(struct Observer_t *observerObj, char *themeName);
}Observer_t;


typedef struct Subject_t
{
    char name[32];
    struct Observer_t *register_list[10];
    bool (*add)(struct Subject_t *subjectobj, struct Observer_t * observerobj);
    bool (*del)(struct Subject_t *subjectobj, struct Observer_t * observerobj);
    bool (*notify)(struct Subject_t * subjectObj, char *event);
}Subject_t;

bool adapter(char *themeName)
{
    if (0 == strcasecmp(themeName, "csdn-blog"))
        return true;
    else
        return true;
}

bool update_proc(Observer_t *observerObj, char *themeName)
{
    printf("[%s]收到[%s]主题跟新通知.....\n",        observerObj->name, themeName);

    return true;
}

bool add(Subject_t *subjectObj, Observer_t *observerObj)
{
    
    for(int i = 0; i < (sizeof(subjectObj->register_list)/sizeof(subjectObj->register_list[0])); i++)
    {
        if (subjectObj->register_list[i] == NULL)
        {
            subjectObj->register_list[i] = observerObj;
            return true;
        }
    }
    
    return false;
}

bool del(Subject_t * subjectObj, Observer_t *observerObj)
{
    for (int i = 0; i< (sizeof(subjectObj->register_list)/sizeof(subjectObj->register_list[0])); i++)
    {
        if (subjectObj->register_list[i] == observerObj)
        {
            subjectObj->register_list[i] = NULL;
            free(observerObj);
            return true;
        }
    }
    
    return false;
}

bool notify(Subject_t * subjectObj, char *event)
{
    for(int i = 0; i< (sizeof(subjectObj->register_list)/sizeof(subjectObj->register_list[0])); i++)
    {
        if (NULL == subjectObj->register_list[i])
            continue;
        
        if (subjectObj->register_list[i]->adapter(event))
            subjectObj->register_list[i]->update_proc(subjectObj->register_list[i], \
                                                      subjectObj->name);
    }
    
    return true;
}
Observer_t* Observer_new(char *name)
{
    Observer_t * p;
    
    p = malloc(sizeof(*p));
    if (!p)
        return NULL;
    
    memcpy(p->name, name, strlen(name));
    p->adapter = adapter;
    p->update_proc = update_proc;
    return p;
}

Subject_t* Subject_new(char * name)
{
    Subject_t * p;

    p = malloc(sizeof(*p));
    if (!p)
        return NULL;
    memset(p, 0x00, sizeof(*p));
    memcpy(p->name, name, strlen(name));
    p->add = add;
    p->del = del;
    p->notify = notify;
}

int main(void)
{
    Subject_t *subject;
    Observer_t *observerA;
    Observer_t *observerB;
    Observer_t *observerC;
    
    printf("start to create subjecter and observer.....\n");
    // 创建一个主题
    subject = Subject_new("creater");
    //创建多个消费者
    observerA = Observer_new("readerA");
    observerB = Observer_new("readerB");
    observerC = Observer_new("readerC");

    // 添加消费者到观察者列表
    subject->add(subject, observerA);
    subject->add(subject, observerB);
    subject->add(subject, observerC);
    
    // 通知给所有读者
    subject->notify(subject, "csdn-blog");
}
