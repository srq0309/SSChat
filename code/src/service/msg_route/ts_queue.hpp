#ifndef TS_QUEUE_H__
#define TS_QUEUE_H__

/*!
 * @file	ts_queue.hpp
 *
 * @author	skiller
 * @date	2018-8-29
 *
 * @brief   基于链表和互斥锁的线程安全队列
 */

#include <mutex>

template<typename _Ty>
struct ts_node
{
    std::shared_ptr<_Ty> data_;
    ts_node *next_;

    ts_node(const std::shared_ptr<_Ty>& data = nullptr, ts_node *next = nullptr)
        :data_(data), next_(next)
    {
    }
    ~ts_node()
    {
        data_.reset();
        delete next_;
    }
};

template<typename _Ty>
class ts_queue
{
public:
    /*！
     * @brief	初始化
     */
    ts_queue()
        :head_(new ts_node<_Ty>), tail_(head_)
    {
    }

    /*！
     * @brief	禁止拷贝构造
     */
    ts_queue(const ts_queue&) = delete;

    /*！
     * @brief	禁止赋值运算
     */
    ts_queue& operator=(const ts_queue&) = delete;

    /*！
     * @brief	析构整个链表
     */
    ~ts_queue()
    {
        delete head_;
    }

    /*！
     * @brief	入队
     * @param	智能指针包裹的数据
     */
    void push(std::shared_ptr<_Ty> pData)
    {
        auto new_tail = new ts_node<_Ty>;
        std::lock_guard<std::mutex> lk(mu_tail_);
        tail_->data_ = pData;
        tail_->next_ = new_tail;
        tail_ = new_tail;
    }

    /*！
     * @brief	非等待出队
     * @return	成功返回智能指针包裹的数据，失败返回空指针
     */
    std::shared_ptr<_Ty> pop()
    {
        ts_node<_Ty> *old_head = nullptr;   // 待出队的旧节点
        {
            std::lock_guard<std::mutex> lk(mu_head_);
            if (head_ == _get_tail()) {
                return nullptr;
            }
            old_head = head_;
            head_ = old_head->next_;
        }
        auto pData = std::move(old_head->data_);
        old_head->next_ = nullptr;
        delete old_head;
        return pData;
    }

    /*！
     * @brief	判断空
     * @return	队列为空返回true，否则返回false
     */
    bool empty()
    {
        std::lock_guard<std::mutex> lk(mu_head_);
        return head_ == _get_tail();
    }

protected:
    inline ts_node<_Ty> *_get_tail()
    {
        std::lock_guard<std::mutex> lk(mu_tail_);
        return tail_;
    }

private:
    ts_node<_Ty> *head_;
    std::mutex mu_head_;

    ts_node<_Ty> *tail_;
    std::mutex mu_tail_;
};


#endif // TS_QUEUE_H__
