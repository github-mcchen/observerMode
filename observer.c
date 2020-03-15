#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief 观察者抽象类
 * @note adapter 适配接口,适配主题 update_proc 更新处理接口
 */
typedef struct Observer_t
{
    char name[32];
    bool (*adapter)(char *themeName);
    bool (*update_proc)(struct Observer_t *observerObj, char *themeName);
}Observer_t;

/**
 * @brief 主题抽象类
 * @note register_list 观察者列表 add 观察者注册接口 del 注销观察者接口
 * @note notify 通知观察者接口,这里可以实现为同步或者异步通知
 */
typedef struct Subject_t
{
    char name[32];
    struct Observer_t *register_list[10];
    bool (*add)(struct Subject_t *subjectobj, struct Observer_t * observerobj);
    bool (*del)(struct Subject_t *subjectobj, struct Observer_t * observerobj);
    bool (*notify)(struct Subject_t * subjectObj, char *event);
}Subject_t;

/**
 * @brief adapter
 * @param themeName 主题名
 * @return true适配成功 otherwise false 适配失败
 * @note 对主题过滤,对适配的主题进行通知
 */
bool adapter(char *themeName)
{
    if (0 == strcasecmp(themeName, "csdn-blog"))
        return true;
    else
        return true;
}

/**
 * @brief update_proc
 * @param observerObj 观察者 themeName 主题
 * @return true
 * @note 更新主题,或者处理对应事件
 */
bool update_proc(Observer_t *observerObj, char *themeName)
{
    printf("[%s]收到[%s]主题跟新通知.....\n",        observerObj->name, themeName);

    return true;
}

/**
 * @brief add
 * @param subjectObj 主题对象
 * @param observerObj 观察者对象
 * @return true 注册进观察者列表成功,else false 注册失败
 * @note 观察者对象注册
 */
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

/**
 * @brief del
 * @param subjectObj 主题对象
 * @param observerObj 观察者对象
 * @return true 注销进观察者列表成功,else false 注册失败
 * @note 观察者对象注销
 */
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

/**
 * @brief notify
 * @param subjectObj 主题对象
 * @param event 通知事件
 * @return true
 * @note 通知到所有观察者
 */
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

/**
 * @brief Observer_new
 * @param name 对象名字
 * @return observerobj
 * @note 创建一个观察者对象
 */
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

/**
 * @brief Subject_new
 * @param name 对象名字
 * @return subjectobj
 * @note 创建一个主题对象
 */
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
